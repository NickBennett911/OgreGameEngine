#include <stdafx.h>
#include <Application.h>
#include <iostream>


ssuge::Application* ssuge::Singleton<ssuge::Application>::msSingleton = NULL;

ssuge::Application::Application() : OgreBites::ApplicationContext("OgreTutorialApp") {
    toggleVisible = true;
    isMainCamera = true;
}

ssuge::Application::Application(int initial_data) : data(initial_data) {
	std::cout << "in overloaded constructor" << std::endl;
    data = 0;
}

ssuge::Application::~Application() {
    delete mLogManager;
    delete mGameObjManager;
    delete mScriptManager;
    delete mInputManager;
}

void ssuge::Application::quit_app() {
    mRoot->queueEndRendering();
}



void ssuge::Application::setup(void)
{
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // register for input events
    //addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    mSceneManager = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(mSceneManager);

    mSceneManager->addRenderQueueListener(getOverlaySystem());

    mGameObjManager = new GameObjectManager();
    mLogManager = new ssuge::LogManager("../log.txt", 50);  // places my log.txt inside of my bin folder
    mScriptManager = new ssuge::ScriptManager();
    mInputManager = new ssuge::InputManager("../media/input_bindings.xml");
    mPhysicsManager = new ssuge::PhysicsManager();
   
    this->setup_sceen();
}

void ssuge::Application::setup_sceen()
{
    // sets what kind of lighting the sceen will use
    mSceneManager->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
    mScriptManager->run_script("..\\media\\init.py");
    
}

void ssuge::Application::setSkyBox(bool active, const char* name, int dist) {
    mSceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 300);
}



void ssuge::Application::set_active_camera(ssuge::CameraComponent* target) 
{
    unsigned short viewports = getRenderWindow()->getNumViewports();
    Ogre::Camera* cam = target->get_camera();
    if (viewports > 0) {   // there is already a viewport
        getRenderWindow()->getViewport(0)->setCamera(cam);
    }
    else {
        getRenderWindow()->addViewport(cam);
    }
}

bool ssuge::Application::frameStarted(const Ogre::FrameEvent& evt)
{

    float dt = evt.timeSinceLastFrame;

    mPhysicsManager->update(dt);
    mGameObjManager->update_game_objects(dt);
    mInputManager->update(dt);
    LOG_MANAGER->update_log_manager(dt);
    return true;
}

void ssuge::Application::run() {
    APPLICATION->getRoot()->startRendering();
}

Ogre::SceneManager* ssuge::Application::get_scene_manager()
{
    return mSceneManager;
}
