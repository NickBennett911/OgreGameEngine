#include <stdafx.h>
#include <camera_component.h>
#include <game_object.h>
#include <application.h>

ssuge::CameraComponent::CameraComponent(ssuge::GameObject * owner) : Component(owner)
{
	mCamera = APPLICATION->get_scene_manager()->createCamera(owner->get_name() + "_camera");
	owner->get_scene_node()->attachObject(mCamera);
	mCamera->setAutoAspectRatio(true);
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(1000.0f);
}


ssuge::CameraComponent::~CameraComponent()
{
	if (mCamera)
	{
		if (mCamera->getParentSceneNode())
			mCamera->getParentSceneNode()->detachObject(mCamera);
		APPLICATION->get_scene_manager()->destroyCamera(mCamera);
	}
}


Ogre::Camera* ssuge::CameraComponent::get_camera()
{
	return mCamera;
}

Ogre::Ray ssuge::CameraComponent::get_screen_ray(Ogre::Vector2 v)
{
	return mCamera->getCameraToViewportRay(v.x, v.y);
}