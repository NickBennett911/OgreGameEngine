#include <stdafx.h>
#include <light_component.h>
#include <game_object.h>
#include <application.h>

ssuge::LightComponent::LightComponent(LightType type, ssuge::GameObject * owner) : Component(owner)
{
	mLight = APPLICATION->get_scene_manager()->createLight(owner->get_name() + "_light");
	owner->get_scene_node()->attachObject(mLight);
	if (type == LightType::DIRECTIONAL)
		mLight->setType(Ogre::Light::LT_DIRECTIONAL);
	else if (type == LightType::POINT)
		mLight->setType(Ogre::Light::LT_POINT);
	else if (type == LightType::SPOT)
		mLight->setType(Ogre::Light::LT_SPOTLIGHT);
}


ssuge::LightComponent::~LightComponent()
{
	if (mLight)
	{
		if (mLight->getParentSceneNode())
			mLight->getParentSceneNode()->detachObject(mLight);
		APPLICATION->get_scene_manager()->destroyLight(mLight);
	}
}