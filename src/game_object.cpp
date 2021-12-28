#include <stdafx.h>
#include <game_object.h>
#include <application.h>
#include <box_collider.h>


ssuge::GameObject::GameObject(std::string name, int tag, GameObject* parent) : mName(name), mTag(tag) {
	mParent = parent;
	if (parent != NULL)
		mSceneNode = parent->get_scene_node()->createChildSceneNode();
	else
		mSceneNode = APPLICATION->get_scene_manager()->getRootSceneNode()->createChildSceneNode();
}

ssuge::GameObject::~GameObject() {
	// Remove our scene node from the scene
	mSceneNode->getParentSceneNode()->removeChild(mSceneNode);
	APPLICATION->get_scene_manager()->destroySceneNode(mSceneNode);

	std::map<ssuge::ComponentType, ssuge::Component*>::iterator it;
	for (it = mComponents.begin(); it != mComponents.end(); ++it) {
		delete it->second;
	}

}


void ssuge::GameObject::set_parent(GameObject* parent) {
	// Remove this scene node from it's current parent scene node
	mSceneNode->getParentSceneNode()->removeChild(mSceneNode);

	// Now make it a parent of parent's scene node
	parent->get_scene_node()->addChild(mSceneNode);
}

ssuge::MeshComponent* ssuge::GameObject::create_mesh_component(std::string fname)
{
	MeshComponent* newComp = new MeshComponent(fname, this);
	mComponents[ssuge::ComponentType::MESH] = newComp;
	return newComp;
}

ssuge::CameraComponent* ssuge::GameObject::create_camera_component()
{
	CameraComponent* newComp = new CameraComponent(this);
	mComponents[ssuge::ComponentType::CAMERA] = newComp;
	return newComp;
}

ssuge::LightComponent* ssuge::GameObject::create_light_componenet(LightType type)
{
	LightComponent* newComp = new LightComponent(type, this);
	mComponents[ssuge::ComponentType::LIGHT] = newComp;
	return newComp;
}

ssuge::BoxCollider* ssuge::GameObject::create_box_collider(float xdim, float ydim, float zdim, float mass, bool is_kinematic) {
	BoxCollider* newComp = new BoxCollider(this, xdim, ydim, zdim, mass, is_kinematic);
	mComponents[ssuge::ComponentType::BOX_PHYS] = newComp;
	return newComp;
}

ssuge::SphereCollider* ssuge::GameObject::create_sphere_collider(float radius, float mass, bool is_kinematic) {
	SphereCollider* newComp = new SphereCollider(this, radius, mass, is_kinematic);
	mComponents[ssuge::ComponentType::SPHERE_PHYS] = newComp;
	return newComp;
}

bool ssuge::GameObject::has_component_type(ssuge::ComponentType type)
{
	if (mComponents[type])
		return true;
	return false;
}

void ssuge::GameObject::update(float dt) {
	std::map<ssuge::ComponentType, ssuge::Component*>::iterator comp_it;
	for (comp_it = mComponents.begin(); comp_it != mComponents.end(); comp_it++) {
		comp_it->second->update(dt);
	}

	if (mTwin) {
		PyObject* args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyFloat_FromDouble((double)dt));

		//calls some run_method funciton
		SCRIPTMANAGER->run_method(mTwin, "update", args);

		Py_DecRef(args);
	}
}

void ssuge::GameObject::action_pressed(std::string action) {
	if (mTwin) {
		PyObject* args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyUnicode_FromString(action.c_str()));

		//calls some run_method funciton
		SCRIPTMANAGER->run_method(mTwin, "action_pressed", args);

		Py_DecRef(args);
	}
}

void ssuge::GameObject::action_released(std::string action) {
	if (mTwin) {
		PyObject* args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyUnicode_FromString(action.c_str()));

		//calls some run_method funciton
		SCRIPTMANAGER->run_method(mTwin, "action_released", args);

		Py_DecRef(args);
	}
}

void ssuge::GameObject::axis_updated(std::string axis, float value) {
	if (mTwin) {
		PyObject* args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, PyUnicode_FromString(axis.c_str()));
		PyTuple_SetItem(args, 1, PyFloat_FromDouble(value));
		
		//calls some run_method funciton
		SCRIPTMANAGER->run_method(mTwin, "axis_updated", args);

		Py_DecRef(args);
	}
}

void ssuge::GameObject::collision_happened(ssuge::Collider* other) {

	if (mTwin) {
 		PyObject* args = PyTuple_New(1);
		if (other->get_owner()->mTwin)
			PyTuple_SetItem(args, 0, other->get_owner()->mTwin);
		else
			PyTuple_SetItem(args, 0, PyLong_FromDouble(other->get_owner()->mTag));
		//calls method
		SCRIPTMANAGER->run_method(mTwin, "colliding", args);

		Py_IncRef(other->get_owner()->mTwin);
		Py_DecRef(args);
	}
}

ssuge::GameObject* ssuge::GameObject::get_gobj() {
	return this;
}