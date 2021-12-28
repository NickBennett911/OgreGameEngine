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
    //GAME_OBJECT_MANAGER->load_scene_file("blah", "..\\Media\\invader_media\\main_scene.scene");
    // without light we would just get a black screen    
    //Ogre::Light* light = mSceneManager->createLight("MainLight");
    //ssuge::GameObject* LightObject = GAME_OBJECT_MANAGER->create_game_object("temporary", "light");  ///make a game obj with group temporary and name light
    //LightObject->create_light_componenet(LightType::POINT);
    //LightObject->set_position_world(Ogre::Vector3(3, 12, 4.5));

    // finally something to render
    /*ssuge::GameObject* OgreObj = GAME_OBJECT_MANAGER->create_game_object("temporary", "OgreObj");
    OgreObj->create_mesh_component("Sinbad.mesh");
    OgreObj->set_position_world(Ogre::Vector3(0, 5, 0));

    //Ogre::Entity* pent = mSceneManager->createEntity("the_penguin", "penguin.mesh");
    ssuge::GameObject* PenguinObj = GAME_OBJECT_MANAGER->create_game_object("temporary", "PenguinObj");
    PenguinObj->create_mesh_component("penguin.mesh");
    PenguinObj->set_parent(OgreObj);
    PenguinObj->set_position_world(Ogre::Vector3(1.5, 8.1, 0));
    PenguinObj->scale(Ogre::Vector3(0.03, 0.03, 0.03));
    PenguinObj->set_orientation(Ogre::Quaternion(Ogre::Degree(20), Ogre::Vector3::NEGATIVE_UNIT_Z));
    PenguinObj->get_component<ssuge::MeshComponent>(ssuge::ComponentType::MESH)->play_animation("amuse", true);
    */
    // make a camera
    /*ssuge::GameObject* CameraObj = GAME_OBJECT_MANAGER->create_game_object("blah", "MainCamera");
    ssuge::CameraComponent* cam = CameraObj->create_camera_component();
    CameraObj->set_position_world(Ogre::Vector3(0, -7, 4.0));
    CameraObj->set_orientation(Ogre::Quaternion(0.789047, 0.614333, 0, 0));
    //APPLICATION->set_active_camera(CameraObj->get_component<ssuge::CameraComponent>(ssuge::ComponentType::CAMERA));
    getRenderWindow()->addViewport(cam->get_camera());
    CameraObj->set_parent(GAME_OBJECT_MANAGER->get_game_object("Ship"));
    */
    // and tell it to render into the main window
    //getRenderWindow()->addViewport(CameraObj->get_component<ssuge::CameraComponent>(ssuge::ComponentType::CAMERA)->get_camera());

    // also need to tell where we are
    /*ssuge::GameObject* CameraObj2 = GAME_OBJECT_MANAGER->create_game_object("temporary", "SecondCamera");
    CameraObj2->create_camera_component();
    CameraObj2->set_position_world(Ogre::Vector3(0, 15, -30));
    CameraObj2->set_orientation(180, 0, 1, 0);
    CameraObj2->look_at(0, 5, 0);
    CameraObj2->set_parent(OgreObj);
    
    Ogre::Light* light2 = mSceneManager->createLight("DirectLight");
    ssuge::GameObject* LightObject2 = GAME_OBJECT_MANAGER->create_game_object("temporary", "light2");  ///make a game obj with group temporary and name light
    LightObject2->create_light_componenet(LightType::DIRECTIONAL);
    LightObject2->set_parent(CameraObj2);
    LightObject2->get_component<ssuge::CameraComponent>(ssuge::ComponentType::LIGHT)->set_visible(false);
    */

    // setting up the plane
    /*Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "ground", Ogre::RGN_DEFAULT,
        plane,
        100, 100, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
      */  
    // make the ground an entity
    //ssuge::GameObject* groundObj = GAME_OBJECT_MANAGER->create_game_object("temporary", "ground");
    //groundObj->create_mesh_component("ground");
    //groundObj->get_component<ssuge::MeshComponent>(ssuge::ComponentType::MESH)->set_material(0, "Examples/Rockwall");
    //Ogre::Entity* groundEntity = mSceneManager->createEntity("ground");
    //mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    //groundEntity->setCastShadows(false);
    //groundEntity->setMaterialName("Examples/Rockwall");

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
