#pragma once
#include <stdafx.h>
#include <log_manager.h>
#include <singleton.h>
#include <game_object_manager.h>
#include <script_game_object.h>
#include <script_manager.h>
#include <input_manager.h>
#include <physics_manager.h>

#define APPLICATION ssuge::Application::get_singleton_ptr()

/// <summary>
/// This is a basic application class for our ogre and rotating it left and right
/// </summary>
namespace ssuge
{
	class Application : public OgreBites::ApplicationContext, public Singleton<Application> {
		/*---ATTRIBUTES---*/
	protected:
		int data;
		Ogre::SceneManager* mSceneManager;
		ssuge::LogManager* mLogManager;
		ssuge::GameObjectManager* mGameObjManager;
		ssuge::ScriptManager* mScriptManager;
		ssuge::InputManager* mInputManager;
		ssuge::PhysicsManager* mPhysicsManager;

		//****** TEMP ATTRIBS **********
		bool toggleVisible;
		bool isMainCamera;
	/*---CONSTRUCTORS/DESCTRUCTORS---*/
	public:
		Application();
		Application(int init_data);
		~Application();

	/*---GETTERS/SETTERS---*/
	public:
		Ogre::SceneManager* get_scene_manager();
		void setSkyBox(bool active, const char* name, int dist);

	/*---OTHER METHODS---*/
	protected:
		void setup(void);
		void setup_sceen();

		bool frameStarted(const Ogre::FrameEvent& evt) override;
	
	public:
		void run();
		void set_active_camera(ssuge::CameraComponent* target);
		void quit_app();


	};
}

