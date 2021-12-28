#include <stdafx.h>
#include <game_object_manager.h>
#include <game_object.h>
#include <application.h>

#pragma warning(disable : 4996)

ssuge::GameObjectManager* ssuge::Singleton<ssuge::GameObjectManager>::msSingleton = NULL;		/// this allows to set the msSingleton in singleton class 

ssuge::GameObjectManager::GameObjectManager() {
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		it2 = it->second.begin();
		while (it2 != it->second.end())
		{
			it2->second = NULL;
		}
		++it;
	}
}

ssuge::GameObjectManager::~GameObjectManager() {
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::string group_name = it->first;
		it2 = it->second.begin();
		while (it2 != it->second.end())
		{
			it2 = it->second.erase(it2);	/// erase the game object pointer
		}
		++it;
	}
	mGameObjects.clear();
}

void ssuge::GameObjectManager::update_game_objects(float dt) {
	GAME_OBJECT_MANAGER->purge_destruction_queue();

	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		it2 = it->second.begin();
		while (it2 != it->second.end())
		{
			it2->second->update(dt);	/// erase the game object pointer
			++it2;
		}
		++it;
	}
}

ssuge::GameObject* ssuge::GameObjectManager::create_game_object(std::string group_name, std::string object_name, GameObject* parent, unsigned int tag) {
	ssuge::GameObject* newObj = new GameObject(object_name, tag, parent);
	newObj->mName = object_name;
	if (parent != NULL)
	{
		newObj->set_parent(parent);
	}
	newObj->mTag = tag;
	mGameObjects[group_name][object_name] = newObj;
	return newObj;

	
}

ssuge::GameObject* ssuge::GameObjectManager::get_game_object(std::string game_object_name)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		
		it2 = it->second.begin();
		while (it2 != it->second.end())
		{

			std::string gameObjName = it2->first;
			if (gameObjName == game_object_name)
			{
				return it2->second;
			}
			++it2;
		}
		++it;
	}
	return nullptr;
}

ssuge::GameObject* ssuge::GameObjectManager::get_game_object(std::string game_object_name, std::string group_name) {

	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::string g_name = it->first;
		if (g_name == group_name) 
		{
			it2 = it->second.begin();
			while (it2 != it->second.end())
			{

				std::string gameObjName = it2->first;
				if (gameObjName == game_object_name) 
				{
					return it2->second;
				}
				++it2;
			}
			
		}
		++it;
		
	}
	return nullptr;
}


bool ssuge::GameObjectManager::destroy_game_object(std::string group_name, std::string gobj_name) {
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::string g_name = it->first;
		if (g_name == group_name)
		{
			it2 = it->second.begin();
			while (it2 != it->second.end())
			{

				std::string gameObjName = it2->first;
				if (gameObjName == gobj_name)
				{
					it2 = it->second.erase(it2);
					return true;
				} else 
				{
					++it2;
				}
			}
		}
		++it;
	}
	return false;
}

bool ssuge::GameObjectManager::destroy_game_object(std::string gobj_name)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		it2 = it->second.begin();
		while (it2 != it->second.end())
		{

			std::string gameObjName = it2->first;
			if (gameObjName == gobj_name)
			{
				ssuge::GameObject* obj = it2->second;
				if (obj->has_component_type(ssuge::ComponentType::BOX_PHYS)) {
					ssuge::BoxCollider* b = obj->get_component<ssuge::BoxCollider>(ssuge::ComponentType::BOX_PHYS);
					PHYSICS_MANAGER->remove_rigid_body(b->mRigidBody);
				}
				if (obj->has_component_type(ssuge::ComponentType::SPHERE_PHYS)) {
					ssuge::SphereCollider* b = obj->get_component<ssuge::SphereCollider>(ssuge::ComponentType::SPHERE_PHYS);
					PHYSICS_MANAGER->remove_rigid_body(b->mRigidBody);
				}
				delete it2->second;
				it2 = it->second.erase(it2);
				return true;
			}
			else
			{
				++it2;
			}
		}
		++it;
	}
	return false;
}

bool ssuge::GameObjectManager::group_destroy(std::string group_name, bool destroy_group)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::string g_name = it->first;
		if (g_name == group_name)
		{
			it2 = it->second.begin();
			while (it2 != it->second.end())
			{
				it2 = it->second.erase(it2);
			}
			if (destroy_group)
			{
				it = mGameObjects.erase(it);
			}
			return true;
			
		}
		++it;
	}
	return false;

}

std::vector<ssuge::GameObject*> ssuge::GameObjectManager::get_game_objects(std::string group_name)
{
	std::vector<ssuge::GameObject*> return_vector;

	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::string g_name = it->first;
		if (g_name == group_name)
		{
			it2 = it->second.begin();
			while (it2 != it->second.end())
			{
				return_vector.push_back(it2->second);
				++it2;
			}
			return return_vector;
		}
		++it;
	}
	return return_vector;
}

bool ssuge::GameObjectManager::has_group(std::string group_name)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::string g_name = it->first;
		if (g_name == group_name)
		{
			return true;
		}
		++it;
	}
	return false;
}

void ssuge::GameObjectManager::set_visibility(std::string group_name, bool is_visible)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::string g_name = it->first;
		if (g_name == group_name)
		{
			it2 = it->second.begin();
			while (it2 != it->second.end())
			{
				ssuge::GameObject* curObj = it2->second;
				curObj->get_scene_node()->setVisible(is_visible);
				++it2;
			}
		}
		++it;
	}
}

tinyxml2::XMLError ssuge::GameObjectManager::load_scene_file(std::string group_name, std::string fname) {
	tinyxml2::XMLDocument doc;
	const char* charname = fname.c_str();

	tinyxml2::XMLError result =  doc.LoadFile(charname);
	if (result != tinyxml2::XMLError::XML_SUCCESS) {
		LOG_MANAGER->log("couldnt load scene file: " + fname, 5);
		return tinyxml2::XMLError::XML_ERROR_FILE_COULD_NOT_BE_OPENED;
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	if (root == nullptr) return tinyxml2::XML_ERROR_FILE_READ_ERROR;

	ssuge::GameObjectManager::process_XML_node(root, nullptr);
	return tinyxml2::XMLError::XML_SUCCESS;
}

void ssuge::GameObjectManager::process_XML_node(tinyxml2::XMLElement* elem, GameObject* cur_parent) {
	
	for (; elem != nullptr; elem = elem->NextSiblingElement()) {
		std::string name = (std::string)elem->Name();
		tinyxml2::XMLElement* next_el = elem->FirstChildElement();
		const char* entname;
		elem->QueryStringAttribute("name", &entname);
		if (!name.compare("node") && next_el != nullptr) {
			// this is where we look ahead for a script property
			// this function will look ahead and if so then it will make that instance of that script which makes the game object aswell
			// we should be able to ask game object manager for elem->Attribute("name") and give us the object that was created
			// the object that then returns is what will be our new_obj that is passed down
			ssuge::GameObject* new_obj;
			if (ssuge::GameObjectManager::has_class_property(next_el, cur_parent, elem->Attribute("name"))) {
				new_obj = GAME_OBJECT_MANAGER->get_game_object(elem->Attribute("name"));
			}
			else {
				std::string group = "temp";
				if (elem->Attribute("group")) {
					group = elem->Attribute("group");
				}
				new_obj = GAME_OBJECT_MANAGER->create_game_object(group, elem->Attribute("name"), cur_parent);
			}
			LOG_MANAGER->log((std::string)elem->Attribute("name"));
			ssuge::GameObjectManager::process_XML_node(next_el, new_obj);
		}else if (next_el != nullptr) {
			LOG_MANAGER->log((std::string)elem->Name());
			ssuge::GameObjectManager::process_XML_node(next_el, cur_parent);
		}

		//LOG_MANAGER->log(name);
		if (!name.compare("position")) {
			cur_parent->set_position(Ogre::Vector3(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
		}
		if (!name.compare("rotation")) {
			cur_parent->rotate_local(Ogre::Quaternion(elem->FloatAttribute("qw"), elem->FloatAttribute("qx"), elem->FloatAttribute("qy"), elem->FloatAttribute("qz")));
		}
		if (!name.compare("scale")) {
			cur_parent->set_scale(Ogre::Vector3(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
		}
		if (!name.compare("property")) {
			ssuge::GameObjectManager::process_PROPERTY_node(elem, cur_parent, "", false);
		}
		if (!name.compare("entity")) {
			const char* mesh; 
			elem->QueryStringAttribute("meshFile", &mesh);
			cur_parent->create_mesh_component((std::string)mesh);
		}
		if (!name.compare("collider")) {
			std::string group = (std::string)elem->Attribute("type");
			if (!group.compare("box")) {
				cur_parent->create_box_collider(elem->FloatAttribute("x"), 100, 100, 10.0, true);
				PHYSICS_MANAGER->register_listener(cur_parent);
			}
		}
		if (!name.compare("light")) {
			cur_parent->create_light_componenet(ssuge::LightType::POINT);
			process_LIGHT_node(elem, cur_parent);
		}
		if (!name.compare("camera")) {
			ssuge::CameraComponent* cam = cur_parent->create_camera_component();
			APPLICATION->set_active_camera(cam);
		}
	}
}

void ssuge::GameObjectManager::process_LIGHT_node(tinyxml2::XMLElement* elem, GameObject* cur_parent) {
	
	for (; elem != nullptr; elem = elem->NextSiblingElement()) {
		std::string name = (std::string)elem->Name();
		tinyxml2::XMLElement* next_el = elem->FirstChildElement();
		ssuge::LightComponent* light = cur_parent->get_component<ssuge::LightComponent>(ssuge::ComponentType::LIGHT);
		if (next_el != nullptr) {
			process_LIGHT_node(next_el, cur_parent);
		}
		if (!name.compare("colourDiffuse")) {
			light->set_diffuse_color(elem->FloatAttribute("r"), elem->FloatAttribute("g"), elem->FloatAttribute("b"));
		}
		if (!name.compare("colourSpecular")) {
			light->set_diffuse_color(elem->FloatAttribute("r"), elem->FloatAttribute("g"), elem->FloatAttribute("b"));
		}
		LOG_MANAGER->log(name);
	}
}

bool ssuge::GameObjectManager::has_class_property(tinyxml2::XMLElement* elem, GameObject* cur_parent, std::string objname) {

	for (; elem != nullptr; elem = elem->NextSiblingElement()) {
		std::string name = (std::string)elem->Name();
		tinyxml2::XMLElement* next_el = elem->FirstChildElement();
		const char* entname;
		elem->QueryStringAttribute("name", &entname);
		
		if (next_el != nullptr) {
			LOG_MANAGER->log((std::string)elem->Name());
			return ssuge::GameObjectManager::has_class_property(next_el, cur_parent, objname);
		}

		if (!name.compare("property")) {
			bool was_script = ssuge::GameObjectManager::process_PROPERTY_node(elem, cur_parent, objname, true);
			if (was_script)
				return true;
		}
		
	}
	return false;
}



bool ssuge::GameObjectManager::process_PROPERTY_node(tinyxml2::XMLElement* elem, GameObject* cur_parent, std::string objname, bool handle_as_property_check) {
	std::string name = elem->Attribute("name");
	if (!handle_as_property_check) {
		if (!name.compare("tag")) {
			cur_parent->set_tag(elem->IntAttribute("data"));
		}
	}
	else if (handle_as_property_check) {
		if (!name.compare("class")) {	// only care for the purpose of this check if a property element speciefies a class aka script
			const char* data;
			elem->QueryStringAttribute("data", &data);
			PyObject* module = SCRIPTMANAGER->get_main_module();
			int result = PyObject_HasAttrString(module, data);
			if (result > 0) {
				PyObject* attrib = PyObject_GetAttrString(module, data);

				PyObject* args = PyTuple_New(3);
				PyTuple_SetItem(args, 0, PyUnicode_FromString(objname.c_str()));
				if (elem->Attribute("group"))
					PyTuple_SetItem(args, 1, PyUnicode_FromString(elem->Attribute("group")));
				else
					PyTuple_SetItem(args, 1, PyUnicode_FromString("temp"));
				PyTuple_SetItem(args, 2, PyLong_FromDouble(35));		// place holder temp tag value
				PyObject* r = PyObject_Call(attrib, args, NULL);
				if (r == NULL) {
					SCRIPTMANAGER->handle_error();
				}

			}
			LOG_MANAGER->log((std::string)elem->Name() + "wants to add script" + data);
			return true;
		}
	}
	return false;
}

std::string ssuge::GameObjectManager::get_group(std::string gobj_name) {
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{

		it2 = it->second.begin();
		while (it2 != it->second.end())
		{

			std::string gameObjName = it2->first;
			if (gameObjName == gobj_name)
			{
				return it->first;
			}
			++it2;
		}
		++it;
	}
	return nullptr;
}

btRigidBody* ssuge::GameObjectManager::find_rigid_body(btRigidBody body) {
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it;
	std::map<std::string, GameObject*>::iterator it2;
	it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{

		it2 = it->second.begin();
		while (it2 != it->second.end())
		{
			ssuge::GameObject* curobj = it2->second;
			if (curobj->has_component_type(ssuge::ComponentType::BOX_PHYS)) {
				ssuge::BoxCollider* comp = curobj->get_component<ssuge::BoxCollider>(ssuge::ComponentType::BOX_PHYS);
				btRigidBody* tempb = comp->mRigidBody;
				if (tempb->getWorldTransform() == body.getWorldTransform());
			}
			if (curobj->has_component_type(ssuge::ComponentType::SPHERE_PHYS)) {
				ssuge::SphereCollider* comp = curobj->get_component<ssuge::SphereCollider>(ssuge::ComponentType::SPHERE_PHYS);
				return comp->mRigidBody;
			}
			++it2;
		}
		++it;
	}
}

void ssuge::GameObjectManager::purge_destruction_queue() {
	for (ssuge::GameObject* obj : mDestructionQueue) {
		GAME_OBJECT_MANAGER->destroy_game_object(obj->mName);
	}
	mDestructionQueue.clear();
}
