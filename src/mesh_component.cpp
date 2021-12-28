#include <stdafx.h>
#include <mesh_component.h>
#include <game_object.h>
#include <application.h>

ssuge::MeshComponent::MeshComponent(std::string fname, ssuge::GameObject * owner) : Component(owner)
{
	mEntity = APPLICATION->get_scene_manager()->createEntity(owner->get_name() + "_entity", fname);
	owner->get_scene_node()->attachObject(mEntity);\
}


ssuge::MeshComponent::~MeshComponent()
{
	if (mEntity)
	{
		if (mEntity->getParentSceneNode())
			mEntity->getParentSceneNode()->detachObject(mEntity);
		APPLICATION->get_scene_manager()->destroyEntity(mEntity);
	}
}


void ssuge::MeshComponent::set_material(int sub_entity, std::string mat_name)
{
	Ogre::SubEntity * se = mEntity->getSubEntity(sub_entity);
	se->setMaterialName(mat_name);
}

void ssuge::MeshComponent::play_animation(std::string anim, bool looping)
{
	mAnimation = mEntity->getAnimationState(anim);
	mAnimation->setLoop(looping);
	mAnimation->setEnabled(true);
}

void ssuge::MeshComponent::update(float dt) 
{
	if (mAnimation) {
		mAnimation->addTime(dt);
	}
}
