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
		Ogre::SceneManager* get_scene_manager();
		/// sets our skybox
		void setSkyBox(bool active, const char* name, int dist);

	/*---OTHER METHODS---*/
	protected:
		/// sets some important Ogre things as well as initializes all of our managers
		void setup(void);
		/// runs the init.py script file to setup the scene
		void setup_sceen();
		/// update method that calles all the required managers updates and passes in the delta time
		bool frameStarted(const Ogre::FrameEvent& evt) override;
	
	public:
		void run();
		/// <summary>
		/// sets the active camera to the one given
		/// </summary>
		/// <param name="target"></param> desired camera component
		void set_active_camera(ssuge::CameraComponent* target);
		/// closes the application
		void quit_app();


	};
}

