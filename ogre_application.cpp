#include "ogre_application.h"
#include "bin/path_config.h"
//#include "OgreText.h"

namespace ogre_application {

/* Some configuration constants */
/* They are written here as global variables, but ideally they should be loaded from a configuration file */

/* Initialization */

const Ogre::String config_filename_g = "";
const Ogre::String plugins_filename_g = "";
const Ogre::String log_filename_g = "Ogre.log";

/* Main window settings */
const Ogre::String window_title_g = "South (Beta 21, Dec.9)";
const Ogre::String custom_window_capacities_g = "";
const unsigned int window_width_g = 1440;
const unsigned int window_height_g = 900;
const bool window_full_screen_g = false;

/* Viewport and camera settings */
float viewport_width_g = 1.0f;
float viewport_height_g = 1.0f;
float viewport_left_g = (1.0f - viewport_width_g) * 0.5f;
float viewport_top_g = (1.0f - viewport_height_g) * 0.5f;
unsigned short viewport_z_order_g = 100;
const Ogre::ColourValue viewport_background_color_g(0.0, 0.0, 0.0);
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 2000.0;
Ogre::Vector3 camera_position_g(0.0, 0.1, -0.3);
Ogre::Vector3 camera_look_at_g(0.0, 0.0, 1.0);
Ogre::Vector3 camera_up_g(0.0, 1.0, 0.0);

/* Materials */
const Ogre::String material_directory_g = MATERIAL_DIRECTORY;
const Ogre::String material_directory_2 = MATERIAL_DIRECTORY2; //bird related
const Ogre::String material_directory_sound = MATERIAL_DIRECTORY_SOUND;
const Ogre::String material_directory_enemy = MATERIAL_DIRECTORY_ENEMY; //enemy related
const Ogre::String material_directory_wolffront = MATERIAL_DIRECTORY_WOLFFRONT;
const Ogre::String material_directory_wolfrear = MATERIAL_DIRECTORY_WOLFREAR;
const Ogre::String material_directory_wolfbody = MATERIAL_DIRECTORY_WOLFBODY;

OgreApplication::OgreApplication(void){
	
    /* Don't do work in the constructor, leave it for the Init() function */
}

void OgreApplication::Init(void){
	/* Set default values for the variables */
	std::cout << "  Assigning Variables..." << std::endl;
	animating_ = true;
	space_down_ = false;
	animation_state_ = NULL;
	input_manager_ = NULL;
	keyboard_ = NULL;
	mouse_ = NULL;
	gameState = 0;

	paddle_ = 0;
	run_ = -30;
	run2_ = -45;
	swing_ = 1;

	playerX = 0;
	playerY = 0;
	playerZ = 0;

	touched = 0;
	optionLayer = 0;
	optionSetting = "OFF";
	optionSetting2 = "OFF";
	optionSetting3 = "OFF";
	optionSetting4 = "ON";
	optionSetting5 = " 32";

	//combat related
	weaponChoice = 1;
	featherDistance = 0.01;
	featherShot = false;
	colorChange = false;
	whit = false;
	chit = false;
	fchit = false;
	fwhit = false;
	wdamage = 0;
	cdamage = 0;
	whpoint = 1000;
	chpoint = 1000;
	wdeath = false;
	cdeath = false;
	bounceTimes = 0;
	
	//particle related
	timer_ = 0.0;
	animating_particles_ = true;

	//ui related
	green = Ogre::ColourValue(0,255,0);
	orange = Ogre::ColourValue(255,140,0);
	red = Ogre::ColourValue(255,0,0);
	blue = Ogre::ColourValue(0,0,255);
	black = Ogre::ColourValue(0,0,0);
	white = Ogre::ColourValue(0,0,0);
	mapNumber = 1;
	menuNumber = 0;

	Ogre::Quaternion ttq(Ogre::Degree(-15), Ogre::Vector3::UNIT_Y);
	tq = ttq;

	//initial the game with either 1st person mode or 3rd person mode (for testing purpose)
	cam_mode = false;

	//developer defined variables
	level = 1;
	hunger2 = 280;
	hunger3 = 280;
	thirst2 = 280;
	thirst3 = 280;

	timer = new Ogre::Timer();
	timer->reset();

	/* Run all initialization steps */
	std::cout << "  Creating Essential Game Elements..." << std::endl;
    InitRootNode();
    InitPlugins();
	std::cout << "  Initializing Rendering System..." << std::endl;
    InitRenderSystem();
	std::cout << "  Rendering the Game Window..." << std::endl;
    InitWindow();
    InitViewport();
	std::cout << "  Initializing Control Modules..." << std::endl;
	InitEvents();
	InitOIS();
	std::cout << "  Initializing Sound System..." << std::endl;
	InitSound();
	//InitPlayer();
	std::cout << "  Loading Assets..." << std::endl;
	LoadMaterials();
	
}

void OgreApplication::InitSound(){
	soundMgr = SoundManager::createManager();
 
	std::cout << soundMgr->listAvailableDevices();
 
	soundMgr->init();
	soundMgr->setAudioPath( (char*) ".\\" );
 
    // Just for testing
	soundMgr->loadAudio( "South_Theme.wav", &audioId, true);
	soundMgr->loadAudio( "tweet.wav", &tweetId, true);
	soundMgr->loadAudio( "wind.wav", &windId, true);
	soundMgr->loadAudio( "ocean.wav", &oceanId, true);
	soundMgr->loadAudio( "raven.wav", &crowId, false);
	soundMgr->loadAudio( "arrow.wav", &arrowId, false);
	soundMgr->loadAudio( "burning.wav", &burningId, false);
	soundMgr->loadAudio( "stab.wav", &stabId, false);
	soundMgr->loadAudio( "teleport.wav", &teleportId, false);
	soundMgr->loadAudio( "clear.wav", &clearId, false);
	soundMgr->loadAudio( "warning.wav", &warningId, false);
	soundMgr->loadAudio( "prepare.wav", &tickId, true);
	soundMgr->loadAudio( "explode.wav", &explodeId, false);
}

void OgreApplication::InitRootNode(void){

    try {
		
		/* We need to have an Ogre root to be able to access all Ogre functions */
        ogre_root_ = std::auto_ptr<Ogre::Root>(new Ogre::Root(config_filename_g, plugins_filename_g, log_filename_g));
		//ogre_root_->showConfigDialog();

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
		throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitPlugins(void){

    try {

		/* Load plugin responsible for OpenGL render system */
        Strings plugin_names;
        plugin_names.push_back("RenderSystem_GL");
		
        Strings::iterator iter = plugin_names.begin();
        Strings::iterator iter_end = plugin_names.end();
        for (; iter != iter_end; iter++){
            Ogre::String& plugin_name = (*iter);
            if (OGRE_DEBUG_MODE){
                plugin_name.append("_d");
            }
            ogre_root_->loadPlugin(plugin_name);
        }

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitRenderSystem(void){

    try {

        /* Load render system */
        const Ogre::RenderSystemList& render_system_list = ogre_root_->getAvailableRenderers();
        if (render_system_list.size() == 0)
        {
			throw(OgreAppException(std::string("OgreApp::Exception: Sorry, no rendersystem was found.")));
        }

        Ogre::RenderSystem *render_system = render_system_list.at(0);
        ogre_root_->setRenderSystem(render_system);

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

        
void OgreApplication::InitWindow(void){

    try {

        /* Create main window for the application */
		bool create_window_automatically = false;
        ogre_root_->initialise(create_window_automatically, window_title_g, custom_window_capacities_g);

        Ogre::NameValuePairList params;
        params["FSAA"] = "0";

		//turn on vsync to make sure the game is render at your maxmium screen refresh rate to prevent undisplayed framerate from rendering
        params["vsync"] = "true";
        ogre_window_ = ogre_root_->createRenderWindow(window_title_g, window_width_g, window_height_g, window_full_screen_g, &params);

        ogre_window_->setActive(true);
        ogre_window_->setAutoUpdated(false);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitViewport(void){

    try {

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create camera object */
        Ogre::Camera* camera = scene_manager->createCamera("MyCamera");
        Ogre::SceneNode* camera_scene_node = root_scene_node->createChildSceneNode("MyCameraNode");
        camera_scene_node->attachObject(camera);

        camera->setNearClipDistance(camera_near_clip_distance_g);
        camera->setFarClipDistance(camera_far_clip_distance_g); 

		camera->setPosition(camera_position_g);
		camera->lookAt(camera_look_at_g);
		camera->setFixedYawAxis(true, camera_up_g);
		camera->roll(Degree(90));

        /* Create viewport */
        viewport = ogre_window_->addViewport(camera, viewport_z_order_g, viewport_left_g, viewport_top_g, viewport_width_g, viewport_height_g);

        viewport->setAutoUpdated(true);
        viewport->setBackgroundColour(viewport_background_color_g);

		/* Set aspect ratio */
		float ratio = float(viewport->getActualWidth()) / float(viewport->getActualHeight());
        camera->setAspectRatio(ratio);

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitEvents(void){

	try {

		/* Add this object as a FrameListener for render events (see frameRenderingQueued event) */
		ogre_root_->addFrameListener(this);

		/* Add this object as a WindowEventListener to handle the window resize event */
		Ogre::WindowEventUtilities::addWindowEventListener(ogre_window_, this);

	}
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
		throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitOIS(void){

	/* Initialize the Object Oriented Input System (OIS) */
	try {

		/* Initialize input manager */
		OIS::ParamList pl; // Parameter list passed to the input manager initialization
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
		ogre_window_->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		pl.insert(std::make_pair(std::string("w32_mouse"), 
		std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), 
		std::string("DISCL_NONEXCLUSIVE")));
		input_manager_ = OIS::InputManager::createInputSystem(pl);

		/*size_t hWnd = 0;
		ogre_window_->getCustomAttribute("WINDOW", &hWnd);
		input_manager_ = OIS::InputManager::createInputSystem(hWnd);*/

		/* Initialize keyboard and mouse */
		keyboard_ = static_cast<OIS::Keyboard*>(input_manager_->createInputObject(OIS::OISKeyboard, false));

		mouse_ = static_cast<OIS::Mouse*>(input_manager_->createInputObject(OIS::OISMouse, false));
		unsigned int width, height, depth;
		int top, left;
		ogre_window_->getMetrics(width, height, depth, left, top);
		const OIS::MouseState &ms = mouse_->getMouseState();
		ms.width = width;
		ms.height = height;

	}
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::LoadMaterials(void){

    try {
		
		/* Load materials that can then be assigned to objects in the scene */
		Ogre::String resource_group_name = "MyGame";
		Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
		resource_group_manager.createResourceGroup(resource_group_name);
		bool is_recursive = false;

		//resource sotrage paths that is need to be loaded
		resource_group_manager.addResourceLocation(material_directory_g, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.addResourceLocation(material_directory_2, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.addResourceLocation(material_directory_wolfbody, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.addResourceLocation(material_directory_wolffront, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.addResourceLocation(material_directory_wolfrear, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.addResourceLocation(material_directory_enemy, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.addResourceLocation(material_directory_sound, "FileSystem", resource_group_name, is_recursive);

		resource_group_manager.initialiseResourceGroup(resource_group_name);
		resource_group_manager.loadResourceGroup(resource_group_name);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the mesh "object_name" */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity to give it color, etc. */
		entity->setMaterialName(material_name);

		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position, orientation and scale */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

        return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

//create global ambient lighting
void OgreApplication::CreateLighting(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	scene_manager->setAmbientLight(Ogre::ColourValue(1.8, 1.8, 1.8));
	light = scene_manager->createLight("MainLight");
	light->setPosition(20, 80, 50);

}

//create 4 sky bounding domes
Ogre::SceneNode* OgreApplication::CreateSky(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* skyEntity = scene_manager->createEntity("sky.mesh");
	Ogre::SceneNode* skyNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	skyNode->attachObject(skyEntity);
	skyNode->scale(1000.0f, 1000.0f, 1000.0f);
	skyNode->setPosition(0,2925,0);

	skyMesh = static_cast<Ogre::Entity*>(skyNode->getAttachedObject(0));
	//playerMesh->setQueryFlags(PLAYER_QUERY_FLAG)
	collision_tools.register_entity(skyMesh, Collision::COLLISION_SPHERE);

	return skyNode;
}

Ogre::SceneNode* OgreApplication::CreateNight(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* nightEntity = scene_manager->createEntity("night.mesh");
	Ogre::SceneNode* nightNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	nightNode->attachObject(nightEntity);
	nightNode->scale(1000.0f, 1000.0f, 1000.0f);
	nightNode->setPosition(0,1400,0);

	

	return nightNode;
}

Ogre::SceneNode* OgreApplication::CreateSunset(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* sunsetEntity = scene_manager->createEntity("sunset.mesh");
	Ogre::SceneNode* sunsetNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	sunsetNode->attachObject(sunsetEntity);
	sunsetNode->scale(1000.0f, 1000.0f, 1000.0f);
	sunsetNode->setPosition(0,400,0);

	

	return sunsetNode;
}

Ogre::SceneNode* OgreApplication::CreateCloudy(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* cloudyEntity = scene_manager->createEntity("cloudy.mesh");
	Ogre::SceneNode* cloudyNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	cloudyNode->attachObject(cloudyEntity);
	cloudyNode->scale(1000.0f, 1000.0f, 1000.0f);
	cloudyNode->setPosition(0,-600,0);

	return cloudyNode;
}

//create water plane
Ogre::SceneNode* OgreApplication::CreateWater(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* waterEntity = scene_manager->createEntity("water.mesh");
	Ogre::SceneNode* waterNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	waterNode->attachObject(waterEntity);
	waterNode->scale(1000.0f, 1000.0f, 1000.0f);
	waterNode->setPosition(0,-590,0);

	return waterNode;

}

//hierarchically create the humming bird
Ogre::SceneNode* OgreApplication::CreateBody(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* bodyEntity = scene_manager->createEntity("body.mesh");
	body2 = scene_manager->getRootSceneNode()->createChildSceneNode();
	body2->attachObject(bodyEntity);
	body2->setPosition(0,0,0);

	playerMesh = static_cast<Ogre::Entity*>(body2->getAttachedObject(0));
	//playerMesh->setQueryFlags(PLAYER_QUERY_FLAG)
	collision_tools.register_entity(playerMesh, Collision::COLLISION_ACCURATE);

	return body2;
}

Ogre::SceneNode* OgreApplication::CreateLeftWing(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* leftEntity = scene_manager->createEntity("leftwing.mesh");
	Ogre::SceneNode* left = scene_manager->getRootSceneNode()->createChildSceneNode();
	left->attachObject(leftEntity);

	return left;

}

Ogre::SceneNode* OgreApplication::CreateRightWing(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* rightEntity = scene_manager->createEntity("rightwing.mesh");
	Ogre::SceneNode* right = scene_manager->getRootSceneNode()->createChildSceneNode();
	right->attachObject(rightEntity);

	return right;

}

//hierarchically create the wold
Ogre::SceneNode* OgreApplication::CreateWolfBody(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* wolfbodyEntity = scene_manager->createEntity("wolfBody.mesh");
	wolfbody2 = scene_manager->getRootSceneNode()->createChildSceneNode();
	wolfbody2->attachObject(wolfbodyEntity);

	return wolfbody2;
}

Ogre::SceneNode* OgreApplication::CreateWolfFront(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* wolffrontEntity = scene_manager->createEntity("wolfFront.mesh");
	Ogre::SceneNode* wolffront2 = scene_manager->getRootSceneNode()->createChildSceneNode();
	wolffront2->attachObject(wolffrontEntity);

	return wolffront2;
}

Ogre::SceneNode* OgreApplication::CreateWolfRear(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* wolfrearEntity = scene_manager->createEntity("wolfRear.mesh");
	Ogre::SceneNode* wolfrear2 = scene_manager->getRootSceneNode()->createChildSceneNode();
	wolfrear2->attachObject(wolfrearEntity);

	return wolfrear2;
}

//hierarchically create the raven
Ogre::SceneNode* OgreApplication::CreateCrowBody(){
	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* cbodyEntity = scene_manager->createEntity("ravenB.mesh");
	cbody2 = scene_manager->getRootSceneNode()->createChildSceneNode();
	cbody2->attachObject(cbodyEntity);

	//playerMesh = static_cast<Ogre::Entity*>(cbody2->getAttachedObject(0));
	//playerMesh->setQueryFlags(PLAYER_QUERY_FLAG)
	//collision_tools.register_entity(playerMesh, Collision::COLLISION_ACCURATE);

	return cbody2;
}

Ogre::SceneNode* OgreApplication::CreateCrowL(){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* cleftEntity = scene_manager->createEntity("ravenL.mesh");
	Ogre::SceneNode* cleft = scene_manager->getRootSceneNode()->createChildSceneNode();
	cleft->attachObject(cleftEntity);
	cleft->scale(5.0f,5.0f,5.0f);

	return cleft;

}

Ogre::SceneNode* OgreApplication::CreateCrowR(){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* crightEntity = scene_manager->createEntity("ravenR.mesh");
	Ogre::SceneNode* cright = scene_manager->getRootSceneNode()->createChildSceneNode();
	cright->attachObject(crightEntity);
	cright->scale(5.0f,5.0f,5.0f);

	return cright;

}

//create 4 types of terrains
Ogre::SceneNode* OgreApplication::CreateFirn(){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* firnEntity = scene_manager->createEntity("firn.mesh");
	Ogre::SceneNode* firnNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	firnNode->attachObject(firnEntity);
	firnNode->scale(500.0f, 500.0f, 500.0f);
	firnNode->setPosition(0,-200,0);

	firnMesh = static_cast<Ogre::Entity*>(firnNode->getAttachedObject(0));
	collision_tools.register_static_entity(firnMesh, firnNode->getPosition(), firnNode->getOrientation(), firnNode->getScale(), Collision::COLLISION_BOX);

	return firnNode;
}

Ogre::SceneNode* OgreApplication::CreateDesert(){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* desertEntity = scene_manager->createEntity("desert.mesh");
	Ogre::SceneNode* desertNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	desertNode->attachObject(desertEntity);
	desertNode->scale(500.0f, 500.0f, 500.0f);
	desertNode->setPosition(0,880,0);

	desertMesh = static_cast<Ogre::Entity*>(desertNode->getAttachedObject(0));
	collision_tools.register_static_entity(desertMesh, desertNode->getPosition(), desertNode->getOrientation(), desertNode->getScale(), Collision::COLLISION_BOX);

	return desertNode;
}

Ogre::SceneNode* OgreApplication::CreateValley(){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* valleyEntity = scene_manager->createEntity("valley.mesh");
	Ogre::SceneNode* valleyNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	valleyNode->attachObject(valleyEntity);
	valleyNode->scale(500.0f, 500.0f, 500.0f);
	valleyNode->setPosition(0,1880,0);

	valleyMesh = static_cast<Ogre::Entity*>(valleyNode->getAttachedObject(0));
	collision_tools.register_static_entity(valleyMesh, valleyNode->getPosition(), valleyNode->getOrientation(), valleyNode->getScale(), Collision::COLLISION_BOX);

	return valleyNode;
}

Ogre::SceneNode* OgreApplication::CreateIsland(){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* islandEntity = scene_manager->createEntity("island.mesh");
	Ogre::SceneNode* islandNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	islandNode->attachObject(islandEntity);
	islandNode->scale(500.0f, 500.0f, 500.0f);
	islandNode->setPosition(0,2900,0);

	return islandNode;
}

//distribute trees according to map information
Ogre::SceneNode** OgreApplication::CreatePines(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	Ogre::SceneNode* return_node[60];
	Ogre::SceneNode** return_pointer;

	for (int i=0; i< 25; i++){
		Ogre::Entity* pineEntity = scene_manager->createEntity("snowpine.mesh");
		random_x = -(rand() % 170 + 1);
		random_z = -(rand() % 40 + 58);
		random_y = -(rand() % 46 + 1);
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(pineEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}

	for (int i=25; i< 45; ++i){
		Ogre::Entity* pineEntity = scene_manager->createEntity("snowpine.mesh");
		random_x = rand() % 155 + 1;
		random_z = -(rand() % 85 + 45);
		random_y = rand() % 80 + 20;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(pineEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}

	for (int i=45; i< 60; ++i){
		Ogre::Entity* pineEntity = scene_manager->createEntity("snowpine.mesh");
		random_x = -(rand() % 145 + 50);
		random_z = -(rand() % 40 + 55);
		random_y = rand() % 70 + 35;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(pineEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}

	return_pointer = return_node;

	return return_pointer;
	
}

Ogre::SceneNode** OgreApplication::CreateOak(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	Ogre::SceneNode* return_node[60];
	Ogre::SceneNode** return_pointer;
	random_x = 200;
	random_x2 = 180;
	random_x3 = 90;
	random_x4 = 70;
	random_x5 = -79;
	random_x6 = -161;
	corrector = 0;

	for (int i=0; i< 15; i++){
		Ogre::Entity* oakEntity = scene_manager->createEntity("pine.mesh");
		random_z = rand() % 10 + 1907;
		random_y = -0.7 * random_x + 2.406;
		//std::cout << random_x << std::endl;
		//std::cout << random_z << std::endl;
		//std::cout << random_y << std::endl;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(oakEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x, random_z, random_y+corrector);
		random_x -= 30;
	}
	
	for (int i=15; i< 30; i++){
		Ogre::Entity* oakEntity = scene_manager->createEntity("pine.mesh");
		random_z = rand() % 10 + 1907;
		random_y = -0.997 * random_x2+5.406;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(oakEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x2, random_z, random_y+corrector);
		random_x2 -= 30;
	}
	for (int i=30; i< 40; i++){
		Ogre::Entity* oakEntity = scene_manager->createEntity("pine.mesh");
		random_z = rand() % 20 + 1900;
		random_y = 2.923 * random_x3 - 247.077;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(oakEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x3, random_z, random_y+corrector);
		random_x3 -= 6;
	}
	for (int i=40; i< 50; i++){
		Ogre::Entity* oakEntity = scene_manager->createEntity("pine.mesh");
		random_z = rand() % 20 + 1900;
		random_y = 0.191 * random_x4 + 61.621;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(oakEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x4, random_z, random_y+corrector);
		random_x4 -= 25;
	}
	for (int i=50; i< 55; i++){
		Ogre::Entity* oakEntity = scene_manager->createEntity("pine.mesh");
		random_z = rand() % 20 + 1900;
		random_y = -5 * random_x5 - 133;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(oakEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x5, random_z, random_y+corrector);
		random_x5 += 10;
	}
	for (int i=55; i< 60; i++){
		Ogre::Entity* oakEntity = scene_manager->createEntity("pine.mesh");
		random_z = rand() % 20 + 1900;
		random_y = -1.803 * random_x6 - 8.328;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(oakEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x6, random_z, random_y+corrector);
		random_x6 += 30;
	}

	return_pointer = return_node;

	return return_pointer;
	
}

Ogre::SceneNode** OgreApplication::CreateCatcus(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	Ogre::SceneNode* return_node[30];
	Ogre::SceneNode** return_pointer;

	for (int i=0; i< 10; i++){
		Ogre::Entity* pineEntity = scene_manager->createEntity("catcus.mesh");
		random_x = -(rand() % 210 + 0);
		random_z = rand() % 5 + 910;
		random_y = rand() % 150 + 1;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(pineEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(2.0f,2.0f,2.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}
	for (int i=10; i< 15; i++){
		Ogre::Entity* pineEntity = scene_manager->createEntity("catcu.mesh");
		random_x = rand() % 280 + 0;
		random_z = rand() % 15 + 917;
		random_y = rand() % 190 + 22;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(pineEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(25.0f,25.0f,25.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}
	for (int i=15; i< 25; i++){
		Ogre::Entity* pineEntity = scene_manager->createEntity("catcus.mesh");
		random_x = rand() % 280 + 0;
		random_z = rand() % 5 + 917;
		random_y = rand() % 190 + 22;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(pineEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(2.0f,2.0f,2.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}
	for (int i=25; i< 30; i++){
		Ogre::Entity* pineEntity = scene_manager->createEntity("catcu.mesh");
		random_x = -(rand() % 210 + 0);
		random_z = rand() % 5 + 917;
		random_y = rand() % 190 + 22;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(pineEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(25.0f,25.0f,25.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}

	return_pointer = return_node;

	return return_pointer;
	
}

Ogre::SceneNode** OgreApplication::CreateCoco(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	Ogre::SceneNode* return_node[20];
	Ogre::SceneNode** return_pointer;

	for (int i=0; i< 5; i++){
		Ogre::Entity* cocoEntity = scene_manager->createEntity("coco.mesh");
		random_x = rand() % 100 + 40;
		random_z = rand() % 27 + 2920;
		random_y = -(rand() % 100 + 40);
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(cocoEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}
	for (int i=5; i< 10; i++){
		Ogre::Entity* cocoEntity = scene_manager->createEntity("coco.mesh");
		random_x = -(rand() % 80 + 30);
		random_z = rand() % 27 + 2920;
		random_y = -(rand() % 80 + 30);
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(cocoEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_x, random_z, random_y);
	}
	for (int i=10; i< 15; i++){
		Ogre::Entity* cocoEntity = scene_manager->createEntity("coco.mesh");
		random_x = rand() % 100 + 40;
		random_z = rand() % 27 + 2920;
		random_y = -(rand() % 100 + 40);
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(cocoEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_y, random_z, random_x);
	}
	for (int i=15; i< 20; i++){
		Ogre::Entity* cocoEntity = scene_manager->createEntity("coco.mesh");
		random_x = rand() % 90 + 45;
		random_z = rand() % 27 + 2920;
		random_y = rand() % 90 + 45;
		return_node[i] = scene_manager->getRootSceneNode()->createChildSceneNode();
		return_node[i]->attachObject(cocoEntity);
		MenuTransformation(return_node[i]);
		return_node[i]->scale(5.0f,5.0f,5.0f);
		return_node[i]->setPosition(random_y, random_z, random_x);
	}

	return_pointer = return_node;

	return return_pointer;
	
}

//create 3d main menu
Ogre::SceneNode* OgreApplication::CreatePlay(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* playEntity = scene_manager->createEntity("play.mesh");
	Ogre::SceneNode* playNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	playNode->attachObject(playEntity);

	return playNode;
}

Ogre::SceneNode* OgreApplication::CreateOption(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* optionEntity = scene_manager->createEntity("option.mesh");
	Ogre::SceneNode* optionNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	optionNode->attachObject(optionEntity);
	Ogre::Quaternion menuRight(Ogre::Degree(30), Ogre::Vector3::UNIT_Z);
	optionNode->rotate(menuRight);
	return optionNode;
}

Ogre::SceneNode* OgreApplication::CreateCredits(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* creditsEntity = scene_manager->createEntity("credits.mesh");
	Ogre::SceneNode* creditsNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	creditsNode->attachObject(creditsEntity);

	return creditsNode;
}

Ogre::SceneNode* OgreApplication::CreateExit(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* exitEntity = scene_manager->createEntity("exit.mesh");
	Ogre::SceneNode* exitNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	exitNode->attachObject(exitEntity);

	return exitNode;
}

Ogre::SceneNode* OgreApplication::CreateTitle(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* titleEntity = scene_manager->createEntity("title.mesh");
	Ogre::SceneNode* titleNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	titleNode->attachObject(titleEntity);
	titleNode->scale(2.0f, 2.0f, 2.0f);

	return titleNode;
}

//create shelter building objects
Ogre::SceneNode* OgreApplication::CreateGate(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* gateEntity = scene_manager->createEntity("gate.mesh");
	Ogre::SceneNode* gateNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	gateNode->attachObject(gateEntity);
	MenuTransformation(gateNode);
	gateNode->setPosition(90,928,-170);

	return gateNode;
}

Ogre::SceneNode* OgreApplication::CreateGate2(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* gate2Entity = scene_manager->createEntity("gate2.mesh");
	Ogre::SceneNode* gate2Node = scene_manager->getRootSceneNode()->createChildSceneNode();
	gate2Node->attachObject(gate2Entity);
	MenuTransformation(gate2Node);
	gate2Node->scale(0.003f, 0.003f, 0.003f);
	gate2Node->setPosition(195,1988,183);

	return gate2Node;
}

Ogre::SceneNode* OgreApplication::CreateBeacon(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* beaconEntity = scene_manager->createEntity("beacon.mesh");
	Ogre::SceneNode* beaconNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	beaconNode->attachObject(beaconEntity);
	MenuTransformation(beaconNode);
	beaconNode->roll(Ogre::Degree(120));
	beaconNode->scale(13.5f,13.5f,13.5f);
	beaconNode->setPosition(3.7, 2937, -179.5);

	return beaconNode;
}

Ogre::SceneNode* OgreApplication::CreateHouse(){

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* houseEntity = scene_manager->createEntity("house.mesh");
	Ogre::SceneNode* houseNode = scene_manager->getRootSceneNode()->createChildSceneNode();
	houseNode->attachObject(houseEntity);
	MenuTransformation(houseNode);
	//houseNode->scale(0.3f, 0.5f, 0.35f);
	houseNode->setPosition(18,-94,-270);

	return houseNode;
}

//create reference object (for debugging purposes)
Ogre::SceneNode* OgreApplication::CreateCube(void){

    try {
        /* Create a cube */

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::SceneNode* returnNode;

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        Ogre::String object_name = "Cube";
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create triangle list for the object */
        Ogre::String material_name = "ShinyMaterial";
        object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

        /* Vertices of a cube */
        Ogre::Vector3 v0(-0.005, -0.005,  0.005);
		Ogre::Vector3 v1( 0.005, -0.005,  0.005);
		Ogre::Vector3 v2( 0.005,  0.005,  0.005);
		Ogre::Vector3 v3(-0.005,  0.005,  0.005);
		Ogre::Vector3 v4(-0.005, -0.005, -0.005);
		Ogre::Vector3 v5( 0.005, -0.005, -0.005);
		Ogre::Vector3 v6( 0.005,  0.005, -0.005);
		Ogre::Vector3 v7(-0.005,  0.005, -0.005);

        /* Normal of each face of the cube */
        Ogre::Vector3 n0( 0.0,  0.0,  1.0);
        Ogre::Vector3 n1( 1.0,  0.0,  0.0);
        Ogre::Vector3 n2( 0.0,  0.0, -1.0);
        Ogre::Vector3 n3(-1.0,  0.0,  0.0);
        Ogre::Vector3 n4( 0.0,  1.0,  0.0);
        Ogre::Vector3 n5( 0.0, -1.0,  0.0);

        /* Cube's vertex colors */
        Ogre::ColourValue clr0(0.0, 0.0, 1.0);
        Ogre::ColourValue clr1(1.0, 0.0, 1.0);
        Ogre::ColourValue clr2(1.0, 1.0, 1.0);
        Ogre::ColourValue clr3(0.0, 1.0, 0.0);
        Ogre::ColourValue clr4(0.0, 0.0, 1.0);
        Ogre::ColourValue clr5(1.0, 0.0, 0.0);
        Ogre::ColourValue clr6(1.0, 1.0, 0.0);
        Ogre::ColourValue clr7(0.0, 1.0, 0.0);
        
        /* This construction only partially uses shared vertices, so that we can assign appropriate vertex normals
           to each face */
        /* Each face of the cube is defined by four vertices (with the same normal) and two triangles */
        object->position(v0);
        object->normal(n0);
        object->textureCoord(0, 0);
        object->colour(clr0);

        object->position(v1);
        object->normal(n0);
        object->textureCoord(1, 1);
        object->colour(clr1);

        object->position(v2);
        object->normal(n0);
        object->textureCoord(1, 1);
        object->colour(clr2);

        object->position(v3);
        object->normal(n0);
        object->textureCoord(0, 1);
        object->colour(clr3);
        
        object->position(v1);
        object->normal(n1);
        object->textureCoord(0, 0);
        object->colour(clr1);

        object->position(v5);
        object->normal(n1);
        object->textureCoord(1, 0);
        object->colour(clr5);

        object->position(v6);
        object->normal(n1);
        object->textureCoord(1, 1);
        object->colour(clr6);

        object->position(v2);
        object->normal(n1);
        object->textureCoord(0, 1);
        object->colour(clr2);

        object->position(v5);
        object->normal(n2);
        object->textureCoord(0, 0);
        object->colour(clr5);

        object->position(v4);
        object->normal(n2);
        object->textureCoord(1, 0);
        object->colour(clr4);
        
        object->position(v7);
        object->normal(n2);
        object->textureCoord(1, 1);
        object->colour(clr7);

        object->position(v6);
        object->normal(n2);
        object->textureCoord(0, 1);
        object->colour(clr6);

        object->position(v4);
        object->normal(n3);
        object->textureCoord(0, 0);
        object->colour(clr4);

        object->position(v0);
        object->normal(n3);
        object->textureCoord(1, 0);
        object->colour(clr0);

        object->position(v3);
        object->normal(n3);
        object->textureCoord(1, 1);
        object->colour(clr3);

        object->position(v7);
        object->normal(n3);
        object->textureCoord(0, 1);
        object->colour(clr7);

        object->position(v3);
        object->normal(n4);
        object->textureCoord(0, 0);
        object->colour(clr3);

        object->position(v2);
        object->normal(n4);
        object->textureCoord(1, 0);
        object->colour(clr2);

        object->position(v6);
        object->normal(n4);
        object->textureCoord(1, 1);
        object->colour(clr6);

        object->position(v7);
        object->normal(n4);
        object->textureCoord(0, 1);
        object->colour(clr7);

        object->position(v1);
        object->normal(n5);
        object->textureCoord(0, 0);
        object->colour(clr1);

        object->position(v0);
        object->normal(n5);
        object->textureCoord(1, 0);
        object->colour(clr0);

        object->position(v4);
        object->normal(n5);
        object->textureCoord(1, 1);
        object->colour(clr4);

        object->position(v5);
        object->normal(n5);
        object->textureCoord(0, 1);
        object->colour(clr5);

        for (int i = 0; i < 6; i++){
            object->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
            object->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
        }
   
        /* We finished the object */
        object->end();
        
        /* Convert triangle list to a mesh */
        Ogre::String mesh_name = "Cube";
        object->convertToMesh(mesh_name);

		returnNode = scene_manager->getRootSceneNode();
		returnNode->attachObject(object);

		return returnNode;

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode** OgreApplication::CreateCubes(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	Ogre::SceneNode* return_node[1];
	Ogre::SceneNode** return_pointer;

	for (int i = 0; i < 1; i++){
		Ogre::String entity_name, prefix("Cube");
		entity_name = prefix + Ogre::StringConverter::toString(i);
		Ogre::Entity *entity = scene_manager->createEntity(entity_name, "Cube");

		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
		return_node[i] = root_scene_node->createChildSceneNode(entity_name);
		return_node[i]->attachObject(entity);
	}
	
	return_pointer = return_node;

	return return_pointer;
}

//create weapon objects
Ogre::SceneNode* OgreApplication::CreateFeather(){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	Ogre::Entity* featherEntity = scene_manager->createEntity("farrow.mesh");
	Ogre::SceneNode* feather = scene_manager->getRootSceneNode()->createChildSceneNode();
	feather->attachObject(featherEntity);
	feather->roll(Ogre::Degree(90));

	return feather;
}

//create particles
void OgreApplication::CreateRingGeometry(Ogre::String object_name, int num_particles, float loop_radius, float circle_radius){

	try {
		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create point list for the object */
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);

		/* Initialize random numbers */
		std::srand(std::time(0));

		/* Create a set of points which will be the particles */
		/* This is similar to drawing a torus: we will sample points on the surface of the torus */
		float maxspray = 0.5; // This is how much we allow the points to wander around
		float u, v, w, theta, phi, spray; // Work variables
		for (int i = 0; i < num_particles; i++){
			
			// Randomly select two numbers to define a point on the torus
			u = ((double) rand() / (RAND_MAX));
            v = ((double) rand() / (RAND_MAX));
            
			// Use u and v to define the point on the torus
            theta = u * Ogre::Math::TWO_PI;
			phi = v * Ogre::Math::TWO_PI;
			Ogre::Vector3 center = Ogre::Vector3(loop_radius*cos(theta), loop_radius*sin(theta), 0.0);
            Ogre::Vector3 normal = Ogre::Vector3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
			object->position(center + normal*circle_radius); // Position of the point
			object->colour(Ogre::ColourValue(((float) i)/((float) num_particles), 0.0, 1.0 - (((float) i)/((float) num_particles))));
			object->textureCoord(Ogre::Vector2(0.0, 0.0));

			// Now sample a point on a sphere to define a direction for points to wander around
			u = ((double) rand() / (RAND_MAX));
            v = ((double) rand() / (RAND_MAX));
			w = ((double) rand() / (RAND_MAX));
			
			theta = u * Ogre::Math::TWO_PI;
			phi = acos(2.0*v * -1.0);
			spray = maxspray*pow((float) w, (float) (1.0/3.0)); // Cubic root
			Ogre::Vector3 wander = Ogre::Vector3(spray*sin(theta)*sin(phi), spray*cos(theta)*sin(phi), cos(phi));

			object->normal(wander);
		}

		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateParticleGeometry(Ogre::String object_name, int num_particles){

	try {
		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create point list for the object */
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);

		/* Initialize random numbers */
		std::srand(std::time(0));

		/* Create a set of points which will be the particles */
		/* This is similar to drawing a sphere: we will sample points on a sphere, but will allow them to also
		   deviate a bit from the sphere along the normal (change of radius) */
		float trad = 0.4; // Defines the starting point of the particles
        float maxspray = 0.5; // This is how much we allow the points to deviate from the sphere
		float u, v, w, theta, phi, spray; // Work variables
		for (int i = 0; i < num_particles; i++){
			
			// Randomly select three numbers to define a point in spherical coordinates
			u = ((double) rand() / (RAND_MAX));
            v = ((double) rand() / (RAND_MAX));
            w = ((double) rand() / (RAND_MAX));

			// Use u to define the angle theta along one direction of a sphere
            theta = u * 2.0 * 3.1416;
			// Use v to define the angle phi along the other direction of the sphere
			phi = acos(2.0*v - 1.0);
			// Use we to define how much we can deviate from the surface of the sphere (change of radius)
            spray = maxspray*pow((float) w, (float) (1.0/3.0))*3200; // Cubic root of w

			// Define the normal and point based on theta, phi and the spray
            Ogre::Vector3 normal = Ogre::Vector3(spray*cos(theta)*sin(phi), spray*sin(theta)*sin(phi), spray*cos(phi));
			object->position(normal.x*trad, normal.y*trad, normal.z*trad);
			object->normal(normal);
			object->colour(Ogre::ColourValue(i/(float) num_particles, 0.0, 1.0 - (i/(float) num_particles))); // We can use the color for debug, if needed
		}
		
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateBloodGeometry(Ogre::String object_name, int num_particles){

	try {
		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create point list for the object */
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);

		/* Initialize random numbers */
		std::srand(std::time(0));

		/* Create a set of points which will be the particles */
		/* This is similar to drawing a sphere: we will sample points on a sphere, but will allow them to also
		   deviate a bit from the sphere along the normal (change of radius) */
		float trad = 0.4; // Defines the starting point of the particles
        float maxspray = 0.5; // This is how much we allow the points to deviate from the sphere
		float u, v, w, theta, phi, spray; // Work variables
		for (int i = 0; i < num_particles; i++){
			
			// Randomly select three numbers to define a point in spherical coordinates
			u = ((double) rand() / (RAND_MAX));
            v = ((double) rand() / (RAND_MAX));
            w = ((double) rand() / (RAND_MAX));

			// Use u to define the angle theta along one direction of a sphere
            theta = u * 2.0 * 3.1416;
			// Use v to define the angle phi along the other direction of the sphere
			phi = acos(2.0*v - 1.0);
			// Use we to define how much we can deviate from the surface of the sphere (change of radius)
            spray = maxspray*pow((float) w, (float) (1.0/3.0)); // Cubic root of w

			// Define the normal and point based on theta, phi and the spray
            Ogre::Vector3 normal = Ogre::Vector3(spray*cos(theta)*sin(phi), spray*sin(theta)*sin(phi), spray*cos(phi));
			object->position(normal.x*trad, normal.y*trad, normal.z*trad);
			object->normal(normal);
			object->colour(Ogre::ColourValue(i/(float) num_particles, 0.0, 1.0 - (i/(float) num_particles))); // We can use the color for debug, if needed
		}
		
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateTorus(Ogre::String object_name, float loop_radius, float circle_radius, int num_loop_samples, int num_circle_samples){

    try {
		/* Create a torus
		   The torus is built from a large loop with small circles around the loop */

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create triangle list for the object */
		object->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Add vertices to the object */
		float theta, phi; // Angles for circles
		Ogre::Vector3 loop_center;
		Ogre::Vector3 vertex_position;
		Ogre::Vector3 vertex_normal;
		Ogre::ColourValue vertex_color;

		for (int i = 0; i < num_loop_samples; i++){ // large loop
			
			theta = Ogre::Math::TWO_PI*i/num_loop_samples; // loop sample (angle theta)
			loop_center = Ogre::Vector3(loop_radius*cos(theta), loop_radius*sin(theta), 0); // centre of a small circle

			for (int j = 0; j < num_circle_samples; j++){ // small circle
				
				phi = Ogre::Math::TWO_PI*j/num_circle_samples; // circle sample (angle phi)
				
				/* Define position, normal and color of vertex */
				vertex_normal = Ogre::Vector3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
				vertex_position = loop_center + vertex_normal*circle_radius;
				/*Ogre::Vector3(loop_center.x + local_normal.x*circle_radius, 
				                loop_center.y + local_normal.y*circle_radius, 
				     			loop_center.z + local_normal.z*circle_radius);*/
				vertex_color = Ogre::ColourValue(1.0 - ((float) i / (float) num_loop_samples), 
				                                 (float) i / (float) num_loop_samples, 
				                                 (float) j / (float) num_circle_samples);
				/*vertex_color = Ogre::ColourValue(0.0, 0.0, 1.0);*/

				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				object->colour(vertex_color); 
			}
		}

		/* Add triangles to the object */
		for (int i = 0; i < num_loop_samples; i++){
			for (int j = 0; j < num_circle_samples; j++){
				// Two triangles per quad
				object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j, 
							     i*num_circle_samples + ((j + 1) % num_circle_samples),
								 i*num_circle_samples + j);	
				object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j,
					             ((i + 1) % num_loop_samples)*num_circle_samples + ((j + 1) % num_circle_samples),
								 i*num_circle_samples + ((j + 1) % num_circle_samples));
				/*object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j, 
					             i*num_circle_samples + j,
							     i*num_circle_samples + ((j + 1) % num_circle_samples));	
				object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j,
					             i*num_circle_samples + ((j + 1) % num_circle_samples),
					             ((i + 1) % num_loop_samples)*num_circle_samples + ((j + 1) % num_circle_samples));*/
			}
		}

		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_ = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity2(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_2 = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity3(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_3 = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity4(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_4 = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity5(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_5 = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity6(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_6 = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity7(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_7 = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity8(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		particle_material_name_8 = material_name;
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

//create spline systems
void OgreApplication::CreateControlPoints(Ogre::String control_points_name, int num_control_points, Ogre::String material_name){

	try {
		// Control points for the spline
		Ogre::Vector3 *control_point;

		/* Allocate memory for control points */
		control_point = new Ogre::Vector3[num_control_points];

		/* Create control points of a piecewise spline */
		/* We store the control points in groups of 4 */
		/* Each group represents the control points (p0, p1, p2, p3) of a cubic Bezier curve */
		/* To ensure C1 continuity, we constrain the first and second point of each curve according to the previous curve */
        
		// Initialize the first two control points to fixed values */
		control_point[0] = Ogre::Vector3(0.0, 0.0, 0.0);
		control_point[1] = Ogre::Vector3(0.0, 3.0, 0.0);

		// Create remaining points
		for (int i = 2; i < num_control_points; i++){
			// Check if we have the first or second point of a curve
			// Then we need to constrain the points
			if (i % 4 == 0){
				// Constrain the first point of the curve
				// p3 = q0, where the previous curve is (p0, p1, p2, p3) and the current curve is (q0, q1, q2, q3)
				// p3 is at position -1 from the current point q0
				control_point[i] = control_point[i - 1];
			} else if (i % 4 == 1){
				// Constrain the second point of the curve
				// q1 = 2*p3 ?p2
				// p3 is at position -1 and we add another -1 since we are at i%4 == 1 (not i%4 == 0)
				// p2 is at position -2 and we add another -1 since we are at i%4 == 1 (not i%4 == 0)
				control_point[i] = 2.0*control_point[i -2] - control_point[i - 3];
			} else {
				// Other points: we can freely assign random values to them
				// Get 3 random numbers
				float u, v, w;
				u = ((double) rand() / (RAND_MAX));
				v = ((double) rand() / (RAND_MAX));
				w = ((double) rand() / (RAND_MAX));
				// Define control points based on u, v, and w and scale by the control point index
				control_point[i] = Ogre::Vector3(u*3.0*(i/4 + 1), v*3.0*(i/4+1), w*2.5*(i/4+1));
				//control_point[i] = Ogre::Vector3(u*3.0*(i/4 + 1), v*3.0*(i/4+1), 0.0); // Easier to visualize with the control points on the screen
			}
		}

		/* Add control points to the material's shader */
		/* Translate the array of Ogre::Vector3 to an accepted format */
		float *shader_data;
		shader_data = new float[num_control_points*4];
		for (int i = 0; i < num_control_points; i++){
			shader_data[i*4] = control_point[i].x;
			shader_data[i*4 + 1] = control_point[i].y;
			shader_data[i*4 + 2] = control_point[i].z;
			shader_data[i*4 + 3] = 0.0;
		}

		/* Add array as a parameter to the shader of the specified material */
		Ogre::MaterialPtr matt = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(material_name));
		matt->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("control_point", shader_data, num_control_points, 4);

		/* Also create a mesh out of the control points, so that we can render them, if needed */
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(control_points_name);
        object->setDynamic(false);
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);
		for (int i = 0; i < num_control_points; i++){
			object->position(control_point[i]);
			// Color allows us to keep track of control point ordering
			object->colour(1.0 - ((float) i)/((float)num_control_points), 0.0, ((float) i)/((float)num_control_points));
		}
		object->end();
        object->convertToMesh(control_points_name);

		/* Free memory we used to store control points temporarily */
		delete [] control_point;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }	
}

void OgreApplication::CreateControlPointEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the control points */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity */
		entity->setMaterialName(material_name);
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		//scene_node->scale(0.5, 0.5, 0.5);
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

// Create a rotation matrix based on an angle and an axis
Ogre::Matrix4 RotationMatrix(Ogre::Vector3 axis, Ogre::Radian angle){

	Ogre::Matrix3 mat;
	mat = Ogre::Matrix3::IDENTITY;
	mat.FromAngleAxis(axis, angle);
	return Ogre::Matrix4(mat);
}

// Create a translation matrix based on a vector of translations (x, y, z)
Ogre::Matrix4 TranslationMatrix(Ogre::Vector3 trans){

	Ogre::Matrix4 mat;
	mat = Ogre::Matrix4::IDENTITY;
	mat.setTrans(trans);
	return mat;
}

// Create a scaling matrix based on a vector of scale factors (x, y, z)
Ogre::Matrix4 ScalingMatrix(Ogre::Vector3 scale){

	Ogre::Matrix4 mat;
	mat = Ogre::Matrix4::IDENTITY;
	mat.setScale(scale);
	return mat;
}

// Assign a transformation matrix to a scene node
void AssignTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf){
	
	/* In many graphic frameworks, we would simply multiply our geometry by the transformation matrix.
	   However, OGRE stores the transformations of a node in a more efficient manner.
	   So, we need to decompose the transformation first into three components and then assign them
	   to the scene node.*/
	Ogre::Vector3 trans, scale;
	Ogre::Quaternion quat;
	
	transf.decomposition(trans, scale, quat);
	node->setScale(scale);
	node->setOrientation(quat);
	node->setPosition(trans);
}

//matrix trasformation, all entities are hierarchically transformed based on their body
void OgreApplication::BodyTransformation(Ogre::SceneNode* node, float hover){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//initialize root
	root = Ogre::Matrix4::IDENTITY;

	//root = TranslationMatrix(Ogre::Vector3(hover, 0.0f, 0.0f)) * root;
	root = RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0),Ogre::Radian(Ogre::Math::PI/2.0)) * root;

	release = root;

	AssignTransf(node, release);

	LwingTransformation(lwing, 0);
	RwingTransformation(rwing, 0);
	//LwingTransformation(clwing, 0);
	//RwingTransformation(crwing, 0);

}

void OgreApplication::LwingTransformation(Ogre::SceneNode* node, float paddle){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//transformation step recorder
	Ogre::Matrix4 recorder;

	recorder = Ogre::Matrix4::IDENTITY;

	//
	recorder = RotationMatrix(Ogre::Vector3(0.0, 1.0, 0.0),Ogre::Degree(paddle)) * recorder;
	recorder = TranslationMatrix(Ogre::Vector3(-1.0f, 0.0f, 0.0f)) * recorder;

	release = root * recorder;

	AssignTransf(node, release);

}

void OgreApplication::RwingTransformation(Ogre::SceneNode* node, float paddle){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//transformation step recorder
	Ogre::Matrix4 recorder;

	recorder = Ogre::Matrix4::IDENTITY;
	
	//recorder = TranslationMatrix(Ogre::Vector3(-0.001f, 0.0f, 0.0f)) * recorder;
	recorder = RotationMatrix(Ogre::Vector3(0.0, 1.0, 0.0),Ogre::Degree(-paddle)) * recorder;
	recorder = TranslationMatrix(Ogre::Vector3(1.0f, 0.0f, 0.0f)) * recorder;

	release = root * recorder;

	AssignTransf(node, release);

}

void OgreApplication::CrowBodyTransformation(Ogre::SceneNode* node, float hover){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//initialize root
	croot = Ogre::Matrix4::IDENTITY;

	//root = TranslationMatrix(Ogre::Vector3(hover, 0.0f, 0.0f)) * root;
	croot = RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0),Ogre::Radian(Ogre::Math::PI/2.0)) * croot;

	release = croot;

	AssignTransf(node, release);

	CrowLTransformation(clwing, 0);
	CrowRTransformation(crwing, 0);

}

void OgreApplication::CrowLTransformation(Ogre::SceneNode* node, float run){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//transformation step recorder
	Ogre::Matrix4 recorder;

	recorder = Ogre::Matrix4::IDENTITY;

	recorder = RotationMatrix(Ogre::Vector3(0.0, 1.0, 0.0),Ogre::Degree(run)) * recorder;
	//recorder = TranslationMatrix(Ogre::Vector3(0.0f, 0.7f, -1.125f)) * recorder;

	release = croot * recorder;

	AssignTransf(node, release);

}

void OgreApplication::CrowRTransformation(Ogre::SceneNode* node, float run){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//transformation step recorder
	Ogre::Matrix4 recorder;

	recorder = Ogre::Matrix4::IDENTITY;

	recorder = RotationMatrix(Ogre::Vector3(0.0, 1.0, 0.0),Ogre::Degree(-run)) * recorder;
	//recorder = TranslationMatrix(Ogre::Vector3(0.015f, -0.3f, -1.2f)) * recorder;

	release = croot * recorder;

	AssignTransf(node, release);

}

void OgreApplication::WolfBodyTransformation(Ogre::SceneNode* node){
	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//initialize root
	wolfroot = Ogre::Matrix4::IDENTITY;
	
	wolfroot = RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0),Ogre::Radian(Ogre::Math::PI/2.0)) * wolfroot;
	
	//land the wolf on the snowfield, comment out for testing purpose
	//wolfroot = TranslationMatrix(Ogre::Vector3(0.0f, 0.0f, 110.0f)) * wolfroot;

	release = wolfroot;

	AssignTransf(node, release);

	WolfFrontTransformation(wolffront, 0);
	WolfRearTransformation(wolfrear, 0);
}

void OgreApplication::WolfFrontTransformation(Ogre::SceneNode* node, float run){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//transformation step recorder
	Ogre::Matrix4 recorder;

	recorder = Ogre::Matrix4::IDENTITY;

	recorder = RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0),Ogre::Degree(run)) * recorder;
	recorder = TranslationMatrix(Ogre::Vector3(0.0f, 0.7f, -1.125f)) * recorder;

	release = wolfroot * recorder;

	AssignTransf(node, release);

}

void OgreApplication::WolfRearTransformation(Ogre::SceneNode* node, float run){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//transformation step recorder
	Ogre::Matrix4 recorder;

	recorder = Ogre::Matrix4::IDENTITY;

	recorder = RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0),Ogre::Degree(run)) * recorder;
	recorder = TranslationMatrix(Ogre::Vector3(0.015f, -0.3f, -1.2f)) * recorder;

	release = wolfroot * recorder;

	AssignTransf(node, release);

}

void OgreApplication::MenuTransformation(Ogre::SceneNode* node){

	//translation that while be applied in the end
	Ogre::Matrix4 release;

	//initialize root
	menuroot = Ogre::Matrix4::IDENTITY;
	
	menuroot = RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0),Ogre::Radian(Ogre::Math::PI/2.0)) * menuroot;
	
	//land the wolf on the snowfield, comment out for testing purpose
	//wolfroot = TranslationMatrix(Ogre::Vector3(0.0f, 0.0f, 110.0f)) * wolfroot;

	release = menuroot;

	AssignTransf(node, release);

}

//main game loop
void OgreApplication::MainLoop(){

    try {

        /* Main loop to keep the application going */

        ogre_root_->clearEventTimes();

        while(!ogre_window_->isClosed()){

			//std::cout << "Body Position X " << body->getPosition().x << std::endl;
			//std::cout << "Body Position Y " << body->getPosition().y << std::endl;
			//std::cout << "Body Position Z " << body->getPosition().z << std::endl;
			
            ogre_window_->update(false);

            ogre_window_->swapBuffers();

            ogre_root_->renderOneFrame();

            Ogre::WindowEventUtilities::messagePump();
        }
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

//this function has some issues right now, it is used for debugging purposes
void OgreApplication::Replay(){
	wolfbody->setPosition(0,0,0);
	wolffront->setPosition(0,0,0);
	wolfrear->setPosition(0,0,0);
	cbody->setPosition(0,0,0);
	clwing->setPosition(0,0,0);
	crwing->setPosition(0,0,0);
	cbody->roll(Ogre::Degree(-90*bounceTimes+45));
	clwing->roll(Ogre::Degree(-90*bounceTimes+45));
	crwing->roll(Ogre::Degree(-90*bounceTimes+45));
	setUp();

	hunger = 280;
	thirst = 280;
	hunger2 = 280;
	thirst2 = 280;
	hunger3 = 280;
	thirst3 = 280;

	//combat related
	weaponChoice = 1;
	featherDistance = 0.01;
	featherShot = false;
	colorChange = false;
	whit = false;
	chit = false;
	fchit = false;
	fwhit = false;
	wdamage = 0;
	cdamage = 0;
	whpoint = 1000;
	chpoint = 1000;
	wdeath = false;
	cdeath = false;
}

//create user interface system
void OgreApplication::InitUI(){
	
	hunger = 280;
	thirst = 280;
	hunger_str = "Hunger";
	thirst_str = "Thirst";
	mini_posx = 0;
	mini_posy = 0;

	mSilverback = new Gorilla::Silverback();
	mSilverback->loadAtlas("dejavu");
	mSilverback->loadAtlas("firn_bg");
	mSilverback->loadAtlas("desert_bg");
	mSilverback->loadAtlas("valley_bg");
	mSilverback->loadAtlas("island_bg");
	mSilverback->loadAtlas("warrow");
	mSilverback->loadAtlas("wbeak");
	mSilverback->loadAtlas("wfeces");
	
	//1st overlay for general item
	mScreen = mSilverback->createScreen(viewport, "dejavu");

	mScreen->setOrientation(Ogre::OR_DEGREE_0);
	vpW = mScreen->getWidth(), vpH = mScreen->getHeight();

	mLayer = mScreen->createLayer(0);
	rect = mLayer->createRectangle(0,0, vpW, vpH);
	rect->background_gradient(Gorilla::Gradient_NorthSouth, Gorilla::rgb(255,255,255,0), Gorilla::rgb(255,255,255,0));
	
	rect2 = mLayer->createRectangle(0,vpH-150, 424,136);
	rect2->background_image("coco");

	hunger_bar = mLayer->createRectangle(100,vpH-118, hunger, 15);
	hunger_bar->background_colour(Ogre::ColourValue(0,0,0));

	thirst_bar = mLayer->createRectangle(100,vpH-75, thirst, 15);
	thirst_bar->background_colour(Ogre::ColourValue(0,0,0));

	//2nd overlay for map

	mScreen2 = mSilverback->createScreen(viewport, "firn_bg");
	mLayer2 = mScreen2->createLayer(0);
	firn_bg = mLayer2->createRectangle(15,15,513/2.5,513/2.5);
	firn_bg->background_image("firn_bg");

	mScreen3 = mSilverback->createScreen(viewport, "desert_bg");
	mLayer3 = mScreen3->createLayer(0);
	desert_bg = mLayer3->createRectangle(15,15,513/2.5,513/2.5);
	desert_bg->background_image("desert_bg");

	mScreen4 = mSilverback->createScreen(viewport, "valley_bg");
	mLayer4 = mScreen4->createLayer(0);
	valley_bg = mLayer4->createRectangle(15,15,513/2.5,513/2.5);
	valley_bg->background_image("valley_bg");

	mScreen5 = mSilverback->createScreen(viewport, "island_bg");
	mLayer5 = mScreen5->createLayer(0);
	island_bg = mLayer5->createRectangle(15,15,513/2.5,513/2.5);
	island_bg->background_image("island_bg");

	//6th overlay for the top updated item
	mScreen6 = mSilverback->createScreen(viewport, "dejavu");
	mLayer6 = mScreen6->createLayer(0);
	label = mLayer6->createRectangle(vpW-350,20, 220*1.5,67*1.5);
	label->background_image("label");

	//weapon related overlay
	mScreen7 = mSilverback->createScreen(viewport, "warrow");
	mLayer7 = mScreen7->createLayer(0);
	warrow = mLayer7->createRectangle(vpW-245,vpH-300,1024/4,1252/4);
	warrow->background_image("warrow");

	mScreen8 = mSilverback->createScreen(viewport, "wbeak");
	mLayer8 = mScreen8->createLayer(0);
	wbeak = mLayer8->createRectangle(vpW-245,vpH-300,1024/4,1252/4);
	wbeak->background_image("wbeak");

	mScreen9 = mSilverback->createScreen(viewport, "wfeces");
	mLayer9 = mScreen9->createLayer(0);
	wfeces = mLayer9->createRectangle(vpW-245,vpH-300,1024/4,1252/4);
	wfeces->background_image("wfeces");

}

//update user interface when the game is running
void OgreApplication::UpdateUIInfo(){

	hunger_strc = mLayer->createCaption(9, 103, vpH-117, hunger_str);
	hunger_strc->colour(Ogre::ColourValue(255,255,255));
    hunger_strc->width(50);
    hunger_strc->align(Gorilla::TextAlign_Left);

	thirst_strc = mLayer->createCaption(9, 103, vpH-74, thirst_str);
	thirst_strc->colour(Ogre::ColourValue(255,255,255));
    thirst_strc->width(50);
    thirst_strc->align(Gorilla::TextAlign_Left);

	//std::cout << "color change is: " << colorChange << std::endl;
	crosshair = mLayer->createCaption(24, vpW/2-24, vpH/2-24, "+");
	if (colorChange == false){
		crosshair->colour(green);
		winfo = mLayer6->createMarkupText(14,vpW/2-24, vpH/2, " ");
		cinfo = mLayer6->createMarkupText(14,vpW/2-24, vpH/2, " ");
		//cinfo = mLayer->createCaption(14, vpW/2-24, vpH/2, " ");
	}if (colorChange == true){
		if (whit == true || fwhit == true){
			crosshair->colour(red);
			winfo = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Wolf: " + std::to_string(whpoint) + " HP");
		}
		if (chit == true || fchit == true){
			crosshair->colour(red);
			cinfo = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Raven: " + std::to_string(chpoint) + " HP");
		}
	}

	if (mapNumber == 3){
		if (body->getPosition().y < 1950){
			crosshair->colour(red);
			warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%You are injured by nearby posionious plants, please retreat from this area!");
		}
		if (body->getPosition().y >= 1950){
			crosshair->colour(green);
			warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
		}
	}

	crosshair->width(50);
    crosshair->align(Gorilla::TextAlign_Left);

	mini_pos = mLayer6->createCaption(9, (15 + (513/2.5/2) + mini_posx - 9), (15 + (513/2.5/2) + mini_posy + 9), "*");
	mini_pos->colour(red);
	mini_pos->width(50);
    mini_pos->align(Gorilla::TextAlign_Left);

	//std::cout << "minix " << (vpW-(513/2.5)/2 + mini_posx) << std::endl;
	//std::cout << "miniy " << ((513/2.5)/2 + mini_posy) << std::endl;

	hunger = hunger3 - timer->getMilliseconds()/1000;
	thirst = thirst3 - timer->getMilliseconds()/2000;
	if (hunger <= 0){
		hunger = 0;
	}
	
	if(colorChange == false){
		
	}if (colorChange == true){
		hunger3 = hunger3-0.01;
		thirst3 = thirst3-0.005;
	}

	if (thirst == 0){
		//die
	}

	mLayer->destroyRectangle(hunger_bar);
	mLayer->destroyRectangle(thirst_bar);
	//std::cout << "hunger is: " << hunger << std::endl;
	hunger_bar = mLayer->createRectangle(100,vpH-118, hunger, 15);
	thirst_bar = mLayer->createRectangle(100,vpH-75, thirst, 15);
	hunger_bar->background_colour(Ogre::ColourValue(0,0,0));
	thirst_bar->background_colour(Ogre::ColourValue(0,0,0));

	if (mapNumber == 1){
		if (wdeath == true){
			cleared = mLayer->createCaption(14, 15, 672, "CLEAR");
			cleared->colour(green);
		}
		if (housec == true){
			touched = 1;
			soundMgr->playAudio(clearId, false);
		}
		if (housec == true && wdeath == true){
			cleared2 = mLayer->createCaption(14, 15, 716, "CLEAR");
			cleared2->colour(green);
		}
		if (touched == 1){
			cleared2 = mLayer->createCaption(14, 15, 694, "CLEAR");
			cleared2->colour(green);
			
		}
		if (wdeath == false){
			cleared = mLayer->createCaption(14, 15, 672, "DOING");
			cleared->colour(orange);
		}
		if (touched != 1){
			cleared2 = mLayer->createCaption(14, 15, 694, "DOING");
			cleared2->colour(orange);
		}
		if (housec == false || wdeath == false){
			cleared3 = mLayer->createCaption(14, 15, 716, "DOING");
			cleared3->colour(orange);
		}
	}
	if (mapNumber == 2){
		if (cdeath == true){
			cleared = mLayer->createCaption(14, 15, 672, "CLEAR");
			cleared->colour(green);
		}
		if (tentc == true){
			touched = 2;
			soundMgr->playAudio(clearId, false);
		}
		if (tentc == true && cdeath == true){
			cleared2 = mLayer->createCaption(14, 15, 716, "CLEAR");
			cleared2->colour(green);
		}
		if (touched == 2){
			cleared2 = mLayer->createCaption(14, 15, 694, "CLEAR");
			cleared2->colour(green);
		}
		if (cdeath == false){
			cleared = mLayer->createCaption(14, 15, 672, "DOING");
			cleared->colour(orange);
		}
		if (touched != 2){
			cleared2 = mLayer->createCaption(14, 15, 694, "DOING");
			cleared2->colour(orange);
		}
		if (tentc == false || cdeath == false){
			cleared3 = mLayer->createCaption(14, 15, 716, "DOING");
			cleared3->colour(orange);
		}
	}
	if (mapNumber == 3){
		if (body->getPosition().y > 1950){
			cleared = mLayer->createCaption(14, 15, 672, "CLEAR");
			cleared->colour(green);
			soundMgr->pauseAudio(warningId);
		}
		if (castlec == true){
			touched = 3;
			soundMgr->playAudio(clearId, false);
		}
		if (castlec == true){
			cleared2 = mLayer->createCaption(14, 15, 716, "CLEAR");
			cleared2->colour(green);
		}
		if (touched == 3){
			cleared2 = mLayer->createCaption(14, 15, 694, "CLEAR");
			cleared2->colour(green);
		}
		if (body->getPosition().y <= 1950){
			cleared = mLayer->createCaption(14, 15, 672, "DOING");
			cleared->colour(orange);
			soundMgr->playAudio(warningId, false);
		}
		if (touched != 3){
			cleared2 = mLayer->createCaption(14, 15, 694, "DOING");
			cleared2->colour(orange);
		}
		if (castlec == false || body->getPosition().y < 1950){
			cleared3 = mLayer->createCaption(14, 15, 716, "DOING");
			cleared3->colour(orange);
		}
	}
	if (mapNumber == 4){
		if (beaconc == true){
			touched = 4;
			soundMgr->playAudio(clearId, false);
		}
		if (touched == 4){
			cleared = mLayer->createCaption(14, 15, 672, "CLEAR");
			cleared->colour(green);
		}
		if (touched != 4){
			cleared = mLayer->createCaption(14, 15, 672, "DOING");
			cleared->colour(orange);
		}
	}
}

//update camera system when the game is running
void OgreApplication::CameraControl(){
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::Camera* camera = scene_manager->getCamera("MyCamera");
	if (weaponChoice != 3){
		camera->setAutoTracking (true, body);
	}

	playerO = body->getOrientation();

	//First Person Camera
	if (cam_mode == false){

		Ogre::Quaternion ccxq(Ogre::Degree(90), Ogre::Vector3::UNIT_X);
		camera->rotate(ccxq);
		Ogre::Quaternion ccyq(Ogre::Degree(180), Ogre::Vector3::UNIT_Y);
		camera->rotate(ccyq);
		Ogre::Quaternion cczq(Ogre::Degree(180), Ogre::Vector3::UNIT_Z);
		camera->rotate(cczq);

		if (weaponChoice != 3){
			camera->setOrientation(body->getOrientation());

			camera->setPosition(body->getPosition());
		}
		
		//camera->setPosition(0,0,-0.5);

		camera_reference->setVisible(false);
		body->setVisible(false);
		lwing->setVisible(false);
		rwing->setVisible(false);
	}

	//Third Person Camera
	if (cam_mode == true){

		//camera->setPosition(playerO * Ogre::Vector3(body->getPosition().x,body->getPosition().y,body->getPosition().z-0.5));
		//camera->setPosition(0,0,-0.5);

		//camera_reference->setOrientation(playerO);
		//camera_reference->setPosition(playerO * Ogre::Vector3(0,playerZ-0.2,-0.1));
		camera_reference->setPosition(body->getPosition().x,body->getPosition().y,body->getPosition().z);
		//camera->setOrientation(playerO);
		camera->setPosition(camera_reference->getPosition().x, camera_reference->getPosition().y+0.2, camera_reference->getPosition().z-0.5);
		
		camera_reference->setVisible(false);
		body->setVisible(true);
		lwing->setVisible(true);
		rwing->setVisible(true);
		
	}
	//camera->setOrientation(camera_reference->getOrientation());
	//camera->setPosition(camera_reference->getPosition());
	//std::cout<< "playerZ" << playerZ << std::endl;
	
}

//update menu items when the game is running
void OgreApplication::MenuControl(){

	Ogre::Quaternion menuShift(Ogre::Degree(swing_), Ogre::Vector3::UNIT_Z);
	if (swing_ <= -1){
		swing_ = -1;
		swingSwitch = true;
	}
	if (swing_ >= 1){
		swing_ = 1;
		swingSwitch = false;
	}

	if (swingSwitch == true){
		swing_ += 0.01;
	}
	if (swingSwitch == false){
		swing_ -= 0.01;
	}
	
	play->rotate(menuShift);
	option->rotate(menuShift);
	credits->rotate(menuShift);
	exit->rotate(menuShift);

	if (menuNumber == 0){
		title->setVisible(true);
		play->setVisible(false);
		option->setVisible(false);
		credits->setVisible(false);
		exit->setVisible(false);
			
	}

	if (menuNumber == 1){
		title->setVisible(false);
		play->setVisible(true);
		option->setVisible(false);
		credits->setVisible(false);
		exit->setVisible(false);

		if (keyboard_->isKeyDown(OIS::KC_RETURN)){
			gameState = 1;
			timer->reset();
			mLayer8->setVisible(true);
			soundMgr->pauseAudio(audioId);
			soundMgr->playAudio(tweetId, true);
			soundMgr->playAudio(windId, true);
			if (mapNumber == 4){
				soundMgr->playAudio(oceanId, true);
			}
		}
			
	}

	if (menuNumber == 2){
		title->setVisible(false);
		play->setVisible(false);
		option->setVisible(true);
		credits->setVisible(false);
		exit->setVisible(false);

		optionSettingFake = mLayer6->createCaption(24, vpW/1.5, 180, optionSetting);
		optionSetting2Fake = mLayer6->createCaption(24, vpW/1.5, 257, optionSetting2);
		optionSetting3Fake = mLayer6->createCaption(24, vpW/1.5, 332, optionSetting3);
		optionSetting4Fake = mLayer6->createCaption(24, vpW/1.5, 407, optionSetting4);
		optionSetting5Fake = mLayer6->createCaption(24, vpW/1.5, 482, optionSetting5);
		

		if (optionLayer == 0){
			optionAnchor = mLayer6->createCaption(24, vpW/3, 180, "*");
		}
		if (optionLayer == 1){
			optionAnchor = mLayer6->createCaption(24, vpW/3, 257, "*");
		}
		if (optionLayer == 2){
			optionAnchor = mLayer6->createCaption(24, vpW/3, 332, "*");
		}
		if (optionLayer == 3){
			optionAnchor = mLayer6->createCaption(24, vpW/3, 407, "*");
		}
		if (optionLayer == 4){
			optionAnchor = mLayer6->createCaption(24, vpW/3, 482, "*");
		}
		if (keyboard_->isKeyDown(OIS::KC_Q)){
			optionLayer = 0;
		}
		if (keyboard_->isKeyDown(OIS::KC_W)){
			optionLayer = 1;
		}
		if (keyboard_->isKeyDown(OIS::KC_E)){
			optionLayer = 2;
		}
		if (keyboard_->isKeyDown(OIS::KC_R)){
			optionLayer = 3;
		}
		if (keyboard_->isKeyDown(OIS::KC_T)){
			optionLayer = 4;
		}
		if (optionLayer == 0){
			if (keyboard_->isKeyDown(OIS::KC_A)){
				optionSetting = "OFF";
				optionSettingFake->colour(red);
			}
			if (keyboard_->isKeyDown(OIS::KC_S)){
				optionSetting = "ON";
				optionSettingFake->colour(green);
			}
		}
		if (optionLayer == 1){
			if (keyboard_->isKeyDown(OIS::KC_A)){
				optionSetting2 = "OFF";
				optionSetting2Fake->colour(red);
			}
			if (keyboard_->isKeyDown(OIS::KC_S)){
				optionSetting2 = "ON";
				optionSetting2Fake->colour(green);
			}
		}
		if (optionLayer == 2){
			if (keyboard_->isKeyDown(OIS::KC_A)){
				optionSetting3 = "OFF";
				optionSetting3Fake->colour(red);
			}
			if (keyboard_->isKeyDown(OIS::KC_S)){
				optionSetting3 = "ON";
				optionSetting3Fake->colour(green);
			}
		}
		if (optionLayer == 3){
			if (keyboard_->isKeyDown(OIS::KC_A)){
				optionSetting4 = "OFF";
				optionSetting4Fake->colour(red);
			}
			if (keyboard_->isKeyDown(OIS::KC_S)){
				optionSetting4 = "ON";
				optionSetting4Fake->colour(green);
			}
		}
		if (optionLayer == 4){
			if (keyboard_->isKeyDown(OIS::KC_A)){
				optionSetting5 = " 16";
				optionSetting5Fake->colour(green);
			}
			if (keyboard_->isKeyDown(OIS::KC_S)){
				optionSetting5 = " 32";
				optionSetting5Fake->colour(green);
			}
		}

		optionAnchor->colour(green);
		optionName = mLayer6->createMarkupText(24,vpW/2.8, 180, "%@24%Fullscreen\n\nAnti-Alising\n\nHide UI\n\nV-SYNC\n\nColor Depth");
	}
	if (menuNumber == 3){
		title->setVisible(false);
		play->setVisible(false);
		option->setVisible(false);
		credits->setVisible(true);
		exit->setVisible(false);
		creditsText = mLayer6->createMarkupText(24,vpW/3, 150, "%@24%South");
		creditsTextLabel = mLayer6->createMarkupText(14,vpW/3, 180, "%@14%by Jater Xu, Xinpeng Jiang\n\nDesigned\nby Jater Xu, Xinpeng Jiang\n\nGame Art\n\nMesh Modeling/Editing\nTerrain Creation\nBackground Music\nLevel Design\nUI Design\nSound Effects\nby Jater Xu");
		creditsTextLabel2 = mLayer6->createMarkupText(14,vpW/1.8, 180, "%@14%\n\nGame Program\n\nCollision Detection\nAnimation\nTree Distribution\nby Xinpeng Jiang\n\nCamera System\nCombat System\nVisual Effects\nby Jater Xu");
		creditsTextLabel3 = mLayer6->createMarkupText(14,vpW/1.3, 180, "%@14%\n\nPowered By\n\nOgre3D (3D Engine)\nVisual Studio 2012 (Coding)\nBlender (Modeling)\nWorld Machine 2 (Terrain Engine)\nLogic Pro X (Music)\nOpenAL (Sound)\nGorilla (User Interface)\nNew MOC (Terrain Collision)\n\nThanks for playing!");
	}
	if (menuNumber == 4){
		title->setVisible(false);
		play->setVisible(false);
		option->setVisible(false);
		credits->setVisible(false);
		exit->setVisible(true);

		if (keyboard_->isKeyDown(OIS::KC_RETURN)){
			std::exit(0);
		}

	}

	if (mapNumber == 1){
		firn->setVisible(true);
		desert->setVisible(false);
		valley->setVisible(false);
		island->setVisible(false);
	}
	if (mapNumber == 2){
		firn->setVisible(false);
		desert->setVisible(true);
		valley->setVisible(false);
		island->setVisible(false);
	}
	if (mapNumber == 3){
		firn->setVisible(false);
		desert->setVisible(false);
		valley->setVisible(true);
		island->setVisible(false);
	}
	if (mapNumber == 4){
		firn->setVisible(false);
		desert->setVisible(false);
		valley->setVisible(false);
		island->setVisible(true);
	}
}

//set up AI related items
void OgreApplication::setUp(){
	cxmovement = 0.1;
	cymovement = 0.1;
	wxmovement = 0.1;
	wymovement = 0.1;
	wfmovement = 2.9;
	wrmovement = -1.3;

	wolfbody->setScale(4.0f,4.0f,4.0f);
	wolffront->setScale(4.0f,4.0f,4.0f);
	wolfrear->setScale(4.0f,4.0f,4.0f);
	wolfbody->translate(-175,-133,-170);
	wolffront->translate(-175,-133,-170);
	wolfrear->translate(-175,-133,-170);
	wolfbody->roll(Ogre::Degree(-90));
	wolffront->roll(Ogre::Degree(-90));
	wolfrear->roll(Ogre::Degree(-90));
	wolfbody->translate(0,-3.7,0);
	wolffront->translate(wfmovement,0,-0.7);
	wolfrear->translate(wrmovement,0,0.2);

	bounceTimes = 0;
	cbody->translate(150,1000,0);
	clwing->translate(150,1000,0);
	crwing->translate(150,1000,0);
	cbody->roll(Ogre::Degree(-45));
	clwing->roll(Ogre::Degree(-45));
	crwing->roll(Ogre::Degree(-45));

	soundMgr->playAudio( audioId, true);
	farrow->yaw(Ogre::Degree(90));
	shockwave->scale(3.0f,3.0f,3.0f);
	shockwave->pitch(Ogre::Degree(90));

}

//function for controling enemies patrol path and handling death upon attack
void OgreApplication::EnemyMovement(){
	//crow movement
	if (cdeath == false){
		cbody->translate(cxmovement,0,cymovement);
		clwing->translate(cxmovement,0,cymovement);
		crwing->translate(cxmovement,0,cymovement);
		clwing->setOrientation(cbody->getOrientation());
		crwing->setOrientation(cbody->getOrientation());
		clwing->yaw(Ogre::Degree(-run2_));
		crwing->yaw(Ogre::Degree(run2_));
	}
	if (cbody->getPosition().x > 200){
		cxmovement = -cxmovement;
		cbody->roll(Ogre::Degree(90));
		bounceTimes += 1;
	}
	if (cbody->getPosition().x < -200){
		cxmovement = -cxmovement;
		cbody->roll(Ogre::Degree(90));
		bounceTimes += 1;
	}
	if (cbody->getPosition().z < -200){
		cymovement = -cymovement;
		cbody->roll(Ogre::Degree(90));
		bounceTimes += 1;
	}
	if (cbody->getPosition().z > 200){
		cymovement = -cymovement;
		cbody->roll(Ogre::Degree(90));
		bounceTimes += 1;
	}
	if (cdeath == true){
		cbody->translate(0,-0.1,0);
		clwing->translate(0,-0.1,0);
		crwing->translate(0,-0.1,0);
		soundMgr->pauseAudio(crowId);
		if (cbody->getPosition().z <= 600){
			cbody->setVisible(false);
			crwing->setVisible(false);
			clwing->setVisible(false);
		}
		
	}

	//wolf movement
	if (wdeath == false){
		wolfbody->translate(wxmovement,0,0);
		wolffront->translate(wxmovement,0,0);
		wolfrear->translate(wxmovement,0,0);
		wolffront->setOrientation(wolfbody->getOrientation());
		wolfrear->setOrientation(wolfbody->getOrientation());
		wolffront->pitch(Ogre::Degree(run_));
		wolfrear->pitch(Ogre::Degree(-run_));
	}
	if (wolfbody->getPosition().x > 110){
		wxmovement = -wxmovement;
		wolfbody->roll(Ogre::Degree(180));
		wolffront->translate(-1.7,0,0);
		wolfrear->translate(-1.5,0,0);
		wposr = wolfrear->getPosition();
		wposf = wolffront->getPosition();
		wolffront->setPosition(wposr);
		wolfrear->setPosition(wposf);
	}
	if (wolfbody->getPosition().x < -177){
		wxmovement = -wxmovement;
		wolfbody->roll(Ogre::Degree(180));
		wolffront->translate(1.7,0,0);
		wolfrear->translate(1.5,0,0);
		wposr = wolfrear->getPosition();
		wposf = wolffront->getPosition();
		wolffront->setPosition(wposr);
		wolfrear->setPosition(wposf);
	}
	if (wdeath == true){
		wolfbody->translate(0,-0.1,0);
		wolffront->translate(0,-0.1,0);
		wolfrear->translate(0,-0.1,0);
		wolfbody->yaw(Ogre::Degree(-1));
		wolffront->yaw(Ogre::Degree(-1));
		wolfrear->yaw(Ogre::Degree(-1));
		if (wolfbody->getPosition().z <= -220){
			wolfbody->setVisible(false);
			wolffront->setVisible(false);
			wolfrear->setVisible(false);
		}
	}
	
}

//collision detection between players and shelter building objects (cube AABB collision)
void OgreApplication::playerVsBuildingCollision(){
	Ogre::Vector3 playerpos = body->getPosition();
	tentbox=gate->_getWorldAABB();
	castlebox=gate2->_getWorldAABB();
	beaconbox=beacon->_getWorldAABB();
	housebox=house->_getWorldAABB();

	if(housebox.contains(playerpos)){housec=true;}
	else{housec=false;}
	if(tentbox.contains(playerpos)){tentc=true;}
	else{tentc=false;}
	if(castlebox.contains(playerpos)){castlec=true;}
	else{castlec=false;}
	if(beaconbox.contains(playerpos)){beaconc=true;}
	else{beaconc=false;}
	if (housec == true && wdeath == true){
		mapNumber = 2;
		body->setPosition(0,1000,0);
		lwing->setPosition(0,1000,0);
		rwing->setPosition(0,1000,0);
		light->setPosition(20, 1080, 50);
		firn->setVisible(false);
		desert->setVisible(true);
		valley->setVisible(false);
		island->setVisible(false);
		soundMgr->playAudio(teleportId, false);
		soundMgr->pauseAudio(oceanId);
	}
	if (tentc == true && cdeath == true){
		mapNumber = 3;
		body->setPosition(0,2000,0);
		lwing->setPosition(0,2000,0);
		rwing->setPosition(0,2000,0);
		light->setPosition(20, 2080, 50);
		firn->setVisible(false);
		desert->setVisible(false);
		valley->setVisible(true);
		island->setVisible(false);
		soundMgr->playAudio(teleportId, false);
		soundMgr->pauseAudio(oceanId);
		warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
	}
	if (castlec == true){
		mapNumber = 4;
		body->setPosition(0,3000,0);
		lwing->setPosition(0,3000,0);
		rwing->setPosition(0,3000,0);
		light->setPosition(20, 3080, 50);
		firn->setVisible(false);
		desert->setVisible(false);
		valley->setVisible(false);
		island->setVisible(true);
		soundMgr->playAudio(teleportId, false);
		warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
	}
	if (beaconc == true){
		mapNumber = 1;
		gameState = 0;
		menuNumber = 3;
		body->setPosition(0,0,0);
		lwing->setPosition(0,0,0);
		rwing->setPosition(0,0,0);
		light->setPosition(20, 80, 50);
		body->setVisible(false);
		desert->setVisible(false);
		valley->setVisible(true);
		island->setVisible(false);
		soundMgr->pauseAudio(oceanId);
		soundMgr->pauseAudio(tweetId);
		soundMgr->pauseAudio(windId);
		warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
	}
	if (castlec == true){
		mapNumber = 4;
		body->setPosition(0,3000,0);
		lwing->setPosition(0,3000,0);
		rwing->setPosition(0,3000,0);
		light->setPosition(20, 3080, 50);
		firn->setVisible(false);
		desert->setVisible(false);
		valley->setVisible(false);
		island->setVisible(true);
		soundMgr->playAudio(teleportId, false);
		warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
	}
	if (beaconc == true){
		mapNumber = 1;
		gameState = 0;
		menuNumber = 3;
		body->setPosition(0,0,0);
		lwing->setPosition(0,0,0);
		rwing->setPosition(0,0,0);
		light->setPosition(20, 80, 50);
		body->setVisible(false);
		lwing->setVisible(false);
		rwing->setVisible(false);
		firn->setVisible(true);
		desert->setVisible(false);
		valley->setVisible(false);
		island->setVisible(false);
		soundMgr->playAudio(audioId, true);
		soundMgr->pauseAudio(oceanId);
	}
}

//collision detection between players and enemies (sphere collision)
void OgreApplication::playerVsEnemyCollision(){
	Ogre::Vector3 playerpos = body->getPosition();
	Ogre::Vector3 enemypos = wolfbody->getPosition();
	Ogre::Vector3 cenemypos = cbody->getPosition();
	Ogre::Vector3 fplayerpos = farrow->getPosition();
	Ogre::Vector3 camera_reference_pos = camera_reference->getPosition();

	float distance = Ogre::Math::Sqrt((playerpos.x - enemypos.x) * (playerpos.x - enemypos.x) +
							   (playerpos.y - enemypos.y) * (playerpos.y - enemypos.y) +
							   (playerpos.z - enemypos.z) * (playerpos.z - enemypos.z));

	float cdistance = Ogre::Math::Sqrt((playerpos.x - cenemypos.x) * (playerpos.x - cenemypos.x) +
							   (playerpos.y - cenemypos.y) * (playerpos.y - cenemypos.y) +
							   (playerpos.z - cenemypos.z) * (playerpos.z - cenemypos.z));

	float fcdistance = Ogre::Math::Sqrt((fplayerpos.x - cenemypos.x) * (fplayerpos.x - cenemypos.x) +
							   (fplayerpos.y - cenemypos.y) * (fplayerpos.y - cenemypos.y) +
							   (fplayerpos.z - cenemypos.z) * (fplayerpos.z - cenemypos.z));

	float fwdistance = Ogre::Math::Sqrt((fplayerpos.x - enemypos.x) * (fplayerpos.x - enemypos.x) +
							   (fplayerpos.y - enemypos.y) * (fplayerpos.y - enemypos.y) +
							   (fplayerpos.z - enemypos.z) * (fplayerpos.z - enemypos.z));

	float wddistance = Ogre::Math::Sqrt((camera_reference_pos.x - enemypos.x) * (camera_reference_pos.x - enemypos.x) +
							   (camera_reference_pos.y - enemypos.y) * (camera_reference_pos.y - enemypos.y) +
							   (camera_reference_pos.z - enemypos.z) * (camera_reference_pos.z - enemypos.z));

	float cddistance = Ogre::Math::Sqrt((camera_reference_pos.x - cenemypos.x) * (camera_reference_pos.x - cenemypos.x) +
							   (camera_reference_pos.y - cenemypos.y) * (camera_reference_pos.y - cenemypos.y) +
							   (camera_reference_pos.z - cenemypos.z) * (camera_reference_pos.z - cenemypos.z));

	//std::cout << "distance: " << distance << std::endl;

	if (cddistance < 20){
		cdhit = true;
	}
	if (wddistance < 20){
		wdhit = true;
	}

	if (distance < 20){
		colorChange = true;
		whit = true;
		
	}
	if (cdistance < 20){
		colorChange = true;
		chit = true;
		
	}
	if (fcdistance < 20){
		colorChange = true;
		fchit = true;
		if (weaponChoice == 2){
			shockwave->setPosition(cbody->getPosition());
			shockwave->setVisible(true);
			shockwave->scale(2.0f,2.0f,2.0f);
			soundMgr->playAudio(crowId, true);
			if (featherShot == true){
				soundMgr->playAudio(burningId, false);
				chpoint -= 2;
				if (chpoint <= 0){
					warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Stage Clear!\nNow you can progress to the next stage by flying into shelter buildings.");
					cdeath = true;
				}
			}
		}
		
	}
	if (fwdistance < 20){
		colorChange = true;
		fwhit = true;
		if (weaponChoice == 2){
			shockwave->setPosition(wolfbody->getPosition());
			shockwave->setVisible(true);
			shockwave->scale(2.0f,2.0f,2.0f);
			if (featherShot == true){
				soundMgr->playAudio(burningId, false);
				whpoint -= 2;
				if (whpoint <= 0){
					warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Stage Clear!\nNow you can progress to the next stage by flying into shelter buildings.");
					wdeath = true;
				}
			}
		}
	}
	if (distance>=20 && cdistance>=20 && fcdistance>=20 && fwdistance>=20){
		colorChange = false;
		whit = false;
		chit = false;
		fchit = false;
		fwhit = false;
	}

}

//control weapon and combat system when the game is running
void OgreApplication::CombatReady(){
	farrow->setScale(0.02f,0.02f,0.02f);
	if (weaponChoice == 0){
		farrow->setVisible(false);
		tail->setVisible(false);
		shockwave->setVisible(false);
		hysteriaFire->setVisible(false);
		hysteriaGreen->setVisible(false);
		hysteriaBlue->setVisible(false);
	}
	if (weaponChoice == 1){
		farrow->setVisible(false);
		tail->setVisible(false);
		shockwave->setVisible(false);
		hysteriaFire->setVisible(false);
		hysteriaGreen->setVisible(false);
		hysteriaBlue->setVisible(false);
		farrow->setPosition(body->getPosition());
		
		//std::cout << "whit is: " << whit << std::endl;
		if (whit == true){
			if (keyboard_->isKeyDown(OIS::KC_SPACE)){
				whpoint = whpoint - 1;
				blood->setVisible(true);
				blood->setPosition(wolfrear->getPosition());
			}
			if (!keyboard_->isKeyDown(OIS::KC_SPACE)){

			}
			if (whpoint <= 0){
				whpoint = 0;
				wdeath = true;
			}
		}
		if (chit == true){
			if (keyboard_->isKeyDown(OIS::KC_SPACE)){
				soundMgr->playAudio(crowId, true);
				soundMgr->playAudio(stabId, true);
				chpoint = chpoint - 1;
				blood->setVisible(true);
				blood->setPosition(cbody->getPosition());
			}
			if (!keyboard_->isKeyDown(OIS::KC_SPACE)){
				if (chit == false){
					soundMgr->pauseAudio(crowId);
					soundMgr->pauseAudio(stabId);
				}
			}
			if (chpoint <= 0){
				chpoint = 0;
				cdeath = true;
			}
		}
		
	}
	if (weaponChoice == 2){
		farrow->setVisible(true);
		tail->setPosition(farrow->getPosition());
		hysteriaFire->setVisible(false);
		hysteriaGreen->setVisible(false);
		hysteriaBlue->setVisible(false);

		if (cam_mode == false && featherShot == false){
			tail->setVisible(false);
			shockwave->setVisible(false);
			soundMgr->pauseAudio(crowId);
			shockwave->setScale(1.0f,1.0f,1.0f);
			farrow->setOrientation(body->getOrientation());
			tail->setOrientation(camera->getOrientation());
			farrow->setPosition(body->getPosition().x, body->getPosition().y, body->getPosition().z);
		}
		if (cam_mode == true && featherShot == false){
			farrow->setVisible(false);
			tail->setVisible(false);
			shockwave->setVisible(false);
			soundMgr->pauseAudio(crowId);
			shockwave->setScale(1.0f,1.0f,1.0f);
			farrow->setOrientation(body->getOrientation());
			tail->setOrientation(farrow->getOrientation());
			farrow->setPosition(body->getPosition().x, body->getPosition().y, body->getPosition().z);
		}
		if (keyboard_->isKeyDown(OIS::KC_SPACE)){
			soundMgr->playAudio(arrowId, false);
			featherShot = true;
		}
		if (cam_mode == false && featherShot == true){
			tail->setVisible(true);
			featherDistance += 0.1;
			farrow->translate(body->getOrientation() * Ogre::Vector3(0,0,-featherDistance));
			tail->setOrientation(camera->getOrientation());

			if (featherDistance >= 10){
				featherDistance = 0;
				featherShot = false;
			}
		}

		if (cam_mode == true && featherShot == true){
			farrow->setVisible(true);
			tail->setVisible(true);
			featherDistance += 0.1;
			farrow->translate(body->getOrientation() * Ogre::Vector3(0,featherDistance,0));
			tail->setOrientation(farrow->getOrientation());
			if (featherDistance >= 10){
				featherDistance = 0;
				featherShot = false;
			}
		}

		//std::cout << "Feahter Position is: " << farrow->getPosition() << std::endl;
		//std::cout << "Body Position is: " << body->getPosition() << std::endl;
	}
	if (weaponChoice == 3){
		farrow->setVisible(false);
		tail->setVisible(false);
		shockwave->setVisible(false);
		farrow->setPosition(body->getPosition());

		if (whpoint <= 0){
			wdeath = true;
		}
		if (chpoint <= 0){
			cdeath = true;
		}

		if (cam_mode == true){
			detonation = mLayer6->createMarkupText(24,250,15, "%@24%Hysteria Drone only available under 1st Person Camera\nPress C or V to switch Camera Modes");
		}
		
		if (cam_mode == false){
			if (!keyboard_->isKeyDown(OIS::KC_SPACE)){
				detonation = mLayer6->createMarkupText(24,250,15, "%@24%Hold SPACE to operate Hysteria Drone");
				hysteriaFire->setVisible(false);
				hysteriaGreen->setVisible(false);
				hysteriaBlue->setVisible(false);
				hysteriaFire->setOrientation(body->getOrientation());
				hysteriaGreen->setOrientation(body->getOrientation());
				hysteriaBlue->setOrientation(body->getOrientation());
				hysteriaBlue->setPosition(body->getPosition().x-0.01,body->getPosition().y,body->getPosition().z);
				hysteriaGreen->setPosition(body->getPosition().x+0.01,body->getPosition().y,body->getPosition().z);
				hysteriaFire->setPosition(body->getPosition());
				camera->setOrientation(body->getOrientation());
				camera->setPosition(body->getPosition());
				camera_reference->setPosition(body->getPosition());
				camera_reference->setOrientation(camera->getOrientation());
				camera_reference->setVisible(false);
				camera->setAutoTracking (true, body);
				wdhit = false;
				cdhit = false;
			}
			if (keyboard_->isKeyDown(OIS::KC_SPACE)){
				detonation = mLayer6->createMarkupText(24,250, 15, "%@24%Press I,J,K,L to adjust drone's direction\nRelease SPACE to quit\nApproach enemies to detonate");
				hysteriaFire->setVisible(true);
				hysteriaGreen->setVisible(true);
				hysteriaBlue->setVisible(true);
				Ogre::Radian rot_factor(Ogre::Degree(0.5)); // Camera rotation with directional thrusters
				Ogre::Quaternion xq(Ogre::Degree(rot_factor), Ogre::Vector3::UNIT_X);//(playerX, 0, 0)); = (playerO * Ogre::Vector3::UNIT_X).getRotationTo();
				Ogre::Quaternion nxq(Ogre::Degree(-rot_factor), Ogre::Vector3::UNIT_X);//(playerX, 0, 0));
				Ogre::Quaternion yq(Ogre::Degree(rot_factor), Ogre::Vector3::UNIT_Z);//(0, playerZ, 0));
				Ogre::Quaternion nyq(Ogre::Degree(-rot_factor), Ogre::Vector3::UNIT_Z);//(0, playerZ, 0));
				Ogre::Quaternion zq(Ogre::Degree(rot_factor), Ogre::Vector3::UNIT_Y);//(0, playerZ, 0));
				Ogre::Quaternion nzq(Ogre::Degree(-rot_factor), Ogre::Vector3::UNIT_Y);//(0, playerZ, 0));
				velocity2 = 0.5;
				camera->setOrientation(camera_reference->getOrientation());
				camera->setPosition(camera_reference->getPosition());
				camera_reference->setVisible(false);
				camera->setAutoTracking (true, camera_reference);
				hysteriaFire->setOrientation(camera_reference->getOrientation());
				hysteriaGreen->setOrientation(camera_reference->getOrientation());
				hysteriaBlue->setOrientation(camera_reference->getOrientation());
				hysteriaBlue->setPosition(camera_reference->getPosition().x-0.01,camera_reference->getPosition().y,camera_reference->getPosition().z);
				hysteriaGreen->setPosition(camera_reference->getPosition().x+0.01,camera_reference->getPosition().y,camera_reference->getPosition().z);
				hysteriaFire->setPosition(camera_reference->getPosition());
				body->setVisible(true);
				lwing->setVisible(true);
				rwing->setVisible(true);
				
				if (keyboard_->isKeyDown(OIS::KC_I)){
					camera_reference->rotate(xq);
				}
				if (keyboard_->isKeyDown(OIS::KC_K)){
					camera_reference->rotate(nxq);
				}
				if (keyboard_->isKeyDown(OIS::KC_J)){
					camera_reference->rotate(zq);
				}
				if (keyboard_->isKeyDown(OIS::KC_L)){
					camera_reference->rotate(nzq);
				}
				camera_reference->translate(camera_reference->getOrientation() * Ogre::Vector3(0, 0,-velocity2));

				if (wdhit == true && whit == false){
					whpoint -= 400;
					camera_reference->setPosition(99999,99999,99999);
					soundMgr->playAudio(explodeId,false);
					wdhit = false;
				}
				if (cdhit == true  && chit == false){
					chpoint -= 400;
					camera_reference->setPosition(99999,99999,99999);
					soundMgr->playAudio(explodeId,false);
					cdhit = false;
				}

			}
		}
	}

}

bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& fe){
  
	/* This event is called after a frame is queued for  rendering */
	/* Do stuff in this event since the GPU is rendering and the CPU is idle */
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    camera = scene_manager->getCamera("MyCamera");

	//animating particles when the game is running
	if (animating_particles_){
			timer_ += fe.timeSinceLastFrame;
			Ogre::MaterialPtr mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_));
			mat->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
			Ogre::MaterialPtr mat2 = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_2));
			mat2->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
			Ogre::MaterialPtr mat3 = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_3));
			mat3->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
			Ogre::MaterialPtr mat4 = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_4));
			mat4->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
			Ogre::MaterialPtr mat5 = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_5));
			mat5->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
			Ogre::MaterialPtr mat6 = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_6));
			mat6->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
			Ogre::MaterialPtr mat7 = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_7));
			mat7->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
			Ogre::MaterialPtr mat8 = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(particle_material_name_8));
			mat8->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);
	}

	//change the overall moving speed
	velocity = 1.5;

	//special cases of collision detection for island map
	if (mapNumber == 4){
		//use raycast (ray query) to check if the player's enetity is collided with the sky bounding dome and water plane
		rets = collision_tools.check_ray_collision(body->getPosition(), sky->getPosition(), 1.0f, 1.0f, 
			   (0xFFFFFFFF | 0xFFFFFFFF), playerMesh, true);
		//block player upon collision detected
		if (!rets.collided || (body->getPosition().y <= 2936)){
			if (body->getPosition().x < 0){
				body->setPosition(body->getPosition().x+1, body->getPosition().y, body->getPosition().z);
				lwing->setPosition(lwing->getPosition().x+1, lwing->getPosition().y, lwing->getPosition().z);
				rwing->setPosition(rwing->getPosition().x+1, rwing->getPosition().y, rwing->getPosition().z);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			if (body->getPosition().x > 0){
				body->setPosition(body->getPosition().x-1, body->getPosition().y, body->getPosition().z);
				lwing->setPosition(lwing->getPosition().x-1, lwing->getPosition().y, lwing->getPosition().z);
				rwing->setPosition(rwing->getPosition().x-1, rwing->getPosition().y, rwing->getPosition().z);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			if (body->getPosition().y > 2936){
				body->setPosition(body->getPosition().x, body->getPosition().y-1, body->getPosition().z);
				lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y-1, lwing->getPosition().z);
				rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y-1, rwing->getPosition().z);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			if (body->getPosition().y <= 2936){
				body->setPosition(body->getPosition().x, body->getPosition().y+1, body->getPosition().z);
				lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y+1, lwing->getPosition().z);
				rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y+1, rwing->getPosition().z);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			if (body->getPosition().z < 0){
				body->setPosition(body->getPosition().x, body->getPosition().y, body->getPosition().z+1);
				lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y, lwing->getPosition().z+1);
				rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y, rwing->getPosition().z+1);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			if (body->getPosition().z > 0){
				body->setPosition(body->getPosition().x, body->getPosition().y, body->getPosition().z-1);
				lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y, lwing->getPosition().z-1);
				rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y, rwing->getPosition().z-1);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			
		}
	}
	//collision detection for firn map
	if (mapNumber == 1){
		ret = collision_tools.check_ray_collision(body->getPosition(), firn->getPosition(), 1.0f, 1.0f, 
			   (0xFFFFFFFF | 0xFFFFFFFF), skyMesh, true);
	}
	//collision detection for desert map
	if (mapNumber == 2){
		retd = collision_tools.check_ray_collision(body->getPosition(), desert->getPosition(), 1.0f, 1.0f, 
			   (0xFFFFFFFF | 0xFFFFFFFF), skyMesh, true);
	}
	//collision detection for valley map
	if (mapNumber == 3){
		retv = collision_tools.check_ray_collision(body->getPosition(), valley->getPosition(), 1.0f, 1.0f, 
			   (0xFFFFFFFF | 0xFFFFFFFF), skyMesh, true);
	}
	//std::cout << "REC is " << retd.collided << std::endl;
	EnemyMovement();

	/* Keep animating if flag is on */
	if (animating_){
		//CrowLTransformation(clwing, run2_);
		//CrowRTransformation(crwing, run2_);
		//WolfFrontTransformation(wolffront, run_);
		//WolfRearTransformation(wolfrear, -run_);
		
		//handle the animation of the player's controled humming bird
		paddle_+=3;

		if (paddle_ == 0){
			paddle_ = -paddle_;
		}
		if (paddle_ == 30){
			paddle_ = -paddle_;
		}
		
		//handle the animation of wolves
		if (run_ == -30){
			runSwitch = true;
		}
		if (run_ == 30){
			runSwitch = false;
		}

		if (runSwitch == true){
			run_ += 1;
		}
		if (runSwitch == false){
			run_ -= 1;
		}

		//for crows
		if (run2_ == -45){
			run2Switch = true;
		}
		if (run2_ == 45){
			run2Switch = false;
		}

		if (run2Switch == true){
			run2_ += 1;
		}
		if (run2Switch == false){
			run2_ -= 1;
		}
	}
	
	mini_posx = body->getPosition().x/2.5;
	mini_posy = body->getPosition().z/2.5;

	/* Capture input */
	keyboard_->capture();
	mouse_->capture();

	longt = body->getPosition().x/1000 + defaultLongt;
	lat = body->getPosition().z/1000 + defaultLat;

	if (keyboard_->isKeyDown(OIS::KC_ESCAPE)){
		ogre_root_->shutdown();
		ogre_window_->destroy();
		return false;
	}

	if (mapNumber == 1){
		water->setPosition(0,-590,0);
		snow->setVisible(true);
		
	}
	if (mapNumber == 2){
		water->setPosition(0,410,0);
		snow->setVisible(false);
	}
	if (mapNumber == 3){
		water->setPosition(0,1410,0);
		snow->setVisible(false);
	}
	if (mapNumber == 4){
		water->setPosition(0,2935,0);
		snow->setVisible(false);
	}

	mLayer->destroyAllCaptions();
	mLayer2->destroyAllCaptions();
	mLayer6->destroyAllCaptions();
	mLayer6->destroyAllMarkupTexts();

	if (mapNumber == 1){
		markup = mLayer6->createMarkupText(9,vpW-270,55, "%@9%Hudson Bay Mountain, Canada");

		caption = mLayer6->createCaption(9, vpW-255, 70, std::to_string(longt));
		caption->colour(Ogre::ColourValue(0,0,0));
		caption->width(50);
		caption->align(Gorilla::TextAlign_Left);

		caption2 = mLayer6->createCaption(9, vpW-180, 70, std::to_string(lat));
		caption2->colour(Ogre::ColourValue(0,0,0));
		caption2->width(50);
		caption2->align(Gorilla::TextAlign_Left);
		defaultLongt = 54.81;
		defaultLat = -127.34;
	}
	if (mapNumber == 2){

		markup = mLayer6->createMarkupText(9,vpW-270,55, "%@9%     Mojave Desert, America");

		caption = mLayer6->createCaption(9, vpW-255, 70, std::to_string(longt));
		caption->colour(Ogre::ColourValue(0,0,0));
		caption->width(50);
		caption->align(Gorilla::TextAlign_Left);

		caption2 = mLayer6->createCaption(9, vpW-180, 70, std::to_string(lat));
		caption2->colour(Ogre::ColourValue(0,0,0));
		caption2->width(50);
		caption2->align(Gorilla::TextAlign_Left);
		defaultLongt = 35.01;
		defaultLat = -115.47;
	}
	if (mapNumber == 3){
		markup = mLayer6->createMarkupText(9,vpW-270,55, "%@9%       Mount Tialoc, Mexico");

		caption = mLayer6->createCaption(9, vpW-255, 70, std::to_string(longt));
		caption->colour(Ogre::ColourValue(0,0,0));
		caption->width(50);
		caption->align(Gorilla::TextAlign_Left);

		caption2 = mLayer6->createCaption(9, vpW-180, 70, std::to_string(lat));
		caption2->colour(Ogre::ColourValue(0,0,0));
		caption2->width(50);
		caption2->align(Gorilla::TextAlign_Left);
		defaultLongt = 19.40;
		defaultLat = -98.71;
	}
	if (mapNumber == 4){
		markup = mLayer6->createMarkupText(9,vpW-270,55, "%@9%       Colon Island, Panama");

		caption = mLayer6->createCaption(9, vpW-255, 70, std::to_string(longt));
		caption->colour(Ogre::ColourValue(0,0,0));
		caption->width(50);
		caption->align(Gorilla::TextAlign_Left);

		caption2 = mLayer6->createCaption(9, vpW-180, 70, std::to_string(lat));
		caption2->colour(Ogre::ColourValue(0,0,0));
		caption2->width(50);
		caption2->align(Gorilla::TextAlign_Left);
		defaultLongt = 9.39;
		defaultLat = -82.26;
	}

	lw = mLayer6->createCaption(9, vpW-125, 70, "W");
	lw->colour(Ogre::ColourValue(0,0,0));
    lw->width(50);
    lw->align(Gorilla::TextAlign_Left);

	ln = mLayer6->createCaption(9, vpW-205, 70, "N  ,");
	ln->colour(Ogre::ColourValue(0,0,0));
	ln->width(50);
    ln->align(Gorilla::TextAlign_Left);

	//in main menu
	if (gameState == 0){

		farrow->setVisible(false);
		mLayer->setVisible(false);
		mLayer2->setVisible(false);
		mLayer3->setVisible(false);
		mLayer4->setVisible(false);
		mLayer5->setVisible(false);
		mLayer6->setVisible(true);
		mLayer7->setVisible(false);
		mLayer8->setVisible(false);
		mLayer9->setVisible(false);

		mini_pos = mLayer6->createCaption(9, (15 + (513/2.5/2) + mini_posx - 9), (15 + (513/2.5/2) + mini_posy + 9), " ");
		mini_pos->colour(red);
		mini_pos->width(50);
		mini_pos->align(Gorilla::TextAlign_Left);

		if (mapNumber == 1){
			camera->setAutoTracking (false, body);
			camera->setPosition(1,2.7,8);
			camera->lookAt(6,0,-10);
			option->setPosition(0,0,0);
			title->setPosition(0,0,0);
			play->setPosition(0,0,0);
			credits->setPosition(0,0,0);
			exit->setPosition(0,0,0);
			light->setPosition(20, 80, 50);
			body->setPosition(0,0,0);
			lwing->setPosition(0,0,0);
			rwing->setPosition(0,0,0);
		}
		if (mapNumber == 2){
			camera->setAutoTracking (false, body); 
			camera->setPosition(1,1002.7,8);
			camera->lookAt(6,1000,-10);
			option->setPosition(0,1000,0);
			title->setPosition(0,1000,0);
			play->setPosition(0,1000,0);
			credits->setPosition(0,1000,0);
			exit->setPosition(0,1000,0);
			light->setPosition(20, 1080, 50);
			body->setPosition(0,1000,0);
			lwing->setPosition(0,1000,0);
			rwing->setPosition(0,1000,0);
		}
		if (mapNumber == 3){
			camera->setAutoTracking (false, body); 
			camera->setPosition(1,2002.7,8);
			camera->lookAt(6,2000,-10);
			option->setPosition(0,2000,0);
			title->setPosition(0,2000,0);
			play->setPosition(0,2000,0);
			credits->setPosition(0,2000,0);
			exit->setPosition(0,2000,0);
			light->setPosition(20, 2080, 50);
			body->setPosition(0,2000,0);
			lwing->setPosition(0,2000,0);
			rwing->setPosition(0,2000,0);
		}
		if (mapNumber == 4){
			camera->setAutoTracking (false, body); 
			camera->setPosition(1,3002.7,8);
			camera->lookAt(6,3000,-10);
			option->setPosition(0,3000,0);
			title->setPosition(0,3000,0);
			play->setPosition(0,3000,0);
			credits->setPosition(0,3000,0);
			exit->setPosition(0,3000,0);
			light->setPosition(20, 3080, 50);
			body->setPosition(0,3000,0);
			lwing->setPosition(0,3000,0);
			rwing->setPosition(0,3000,0);
		}

		//set character visibility
		camera_reference->setVisible(false);
		body->setVisible(false);
		lwing->setVisible(false);
		rwing->setVisible(false);
		wolfbody->setVisible(false);
		wolffront->setVisible(false);
		wolfrear->setVisible(false);
		cbody->setVisible(false);
		clwing->setVisible(false);
		crwing->setVisible(false);

		if (keyboard_->isKeyDown(OIS::KC_1)){
			menuNumber = 1;
		}
		if (keyboard_->isKeyDown(OIS::KC_2)){
			menuNumber = 2;
		}
		if (keyboard_->isKeyDown(OIS::KC_3)){
			menuNumber = 3;
		}
		if (keyboard_->isKeyDown(OIS::KC_4)){
			menuNumber = 4;
		}
		if (keyboard_->isKeyDown(OIS::KC_BACK)){
			menuNumber = 0;
		}
		if (keyboard_->isKeyDown(OIS::KC_UP)){
			mapNumber = 1;
		}
		if (keyboard_->isKeyDown(OIS::KC_LEFT)){
			mapNumber = 2;
		}
		if (keyboard_->isKeyDown(OIS::KC_DOWN)){
			mapNumber = 3;
		}
		if (keyboard_->isKeyDown(OIS::KC_RIGHT)){
			mapNumber = 4;
		}
		MenuControl();

	}
	Ogre::Radian rot_factor(Ogre::Degree(1.0));
	Ogre::Quaternion xq(Ogre::Degree(rot_factor), Ogre::Vector3::UNIT_X);//(playerX, 0, 0)); = (playerO * Ogre::Vector3::UNIT_X).getRotationTo();
	Ogre::Quaternion nxq(Ogre::Degree(-rot_factor), Ogre::Vector3::UNIT_X);//(playerX, 0, 0));
	Ogre::Quaternion yq(Ogre::Degree(rot_factor), Ogre::Vector3::UNIT_Z);//(0, playerZ, 0));
	Ogre::Quaternion nyq(Ogre::Degree(-rot_factor), Ogre::Vector3::UNIT_Z);//(0, playerZ, 0));
	Ogre::Quaternion zq(Ogre::Degree(rot_factor), Ogre::Vector3::UNIT_Y);//(0, playerZ, 0));
	Ogre::Quaternion nzq(Ogre::Degree(-rot_factor), Ogre::Vector3::UNIT_Y);//(0, playerZ, 0));

	//in game
	//gamestate 1 is playing state, 0 is menu state
	if (gameState == 1){
		playerVsEnemyCollision();
		playerVsBuildingCollision();
		lwing->setOrientation(body->getOrientation());
		rwing->setOrientation(body->getOrientation());
		lwing->yaw(Ogre::Degree(-run_-10));
		rwing->yaw(Ogre::Degree(run_+10));

		mLayer->setVisible(true);
		mLayer6->setVisible(true);

		body->setVisible(true);
		lwing->setVisible(true);
		rwing->setVisible(true);
		
		title->setVisible(false);
		play->setVisible(false);
		option->setVisible(false);
		credits->setVisible(false);
		exit->setVisible(false);

		if (mapNumber == 1){
			mLayer2->setVisible(true);
			mLayer3->setVisible(false);
			mLayer4->setVisible(false);
			mLayer5->setVisible(false);
			wolfbody->setVisible(true);
			wolffront->setVisible(true);
			wolfrear->setVisible(true);
			cbody->setVisible(true);
			clwing->setVisible(true);
			crwing->setVisible(true);
			mission = mLayer6->createMarkupText(14,15, 650, "%@14%Objective:\n            1.Eliminate the wolf\n            2.Find the shelter\n            3.Teleport to next area using the shelter");
		}
		if (mapNumber == 2){
			mLayer2->setVisible(false);
			mLayer3->setVisible(true);
			mLayer4->setVisible(false);
			mLayer5->setVisible(false);
			wolfbody->setVisible(false);
			wolffront->setVisible(false);
			wolfrear->setVisible(false);
			cbody->setVisible(true);
			clwing->setVisible(true);
			crwing->setVisible(true);
			cbody->setScale(10.0f,10.0f,10.0f);
			clwing->setScale(10.0f,10.0f,10.0f);
			crwing->setScale(10.0f,10.0f,10.0f);
			mission = mLayer6->createMarkupText(14,15, 650, "%@14%Objective:\n            1.Eliminate the raven\n            2.Find the shelter\n            3.Teleport to next area using the shelter");
		}
		if (mapNumber == 3){
			mLayer2->setVisible(false);
			mLayer3->setVisible(false);
			mLayer4->setVisible(true);
			mLayer5->setVisible(false);
			wolfbody->setVisible(false);
			wolffront->setVisible(false);
			wolfrear->setVisible(false);
			cbody->setVisible(false);
			clwing->setVisible(false);
			crwing->setVisible(false);
			mission = mLayer6->createMarkupText(14,15, 650, "%@14%Objective:\n            1.Dodge the poisonous trees\n            2.Find the shelter\n            3.Teleport to next area using the shelter");
		}
		if (mapNumber == 4){
			mLayer2->setVisible(false);
			mLayer3->setVisible(false);
			mLayer4->setVisible(false);
			mLayer5->setVisible(true);
			wolfbody->setVisible(false);
			wolffront->setVisible(false);
			wolfrear->setVisible(false);
			cbody->setVisible(false);
			clwing->setVisible(false);
			crwing->setVisible(false);
			mission = mLayer6->createMarkupText(14,15, 650, "%@14%Objective:\n            1.Fly to the lighthouse");
		}

		if (keyboard_->isKeyDown(OIS::KC_1)){
			mLayer7->setVisible(false);
			mLayer8->setVisible(true);
			mLayer9->setVisible(false);
			weaponChoice = 1;
			soundMgr->pauseAudio(tickId);

		}
		if (keyboard_->isKeyDown(OIS::KC_2)){
			mLayer7->setVisible(true);
			mLayer8->setVisible(false);
			mLayer9->setVisible(false);
			weaponChoice = 2;
			soundMgr->pauseAudio(tickId);
		}
		if (keyboard_->isKeyDown(OIS::KC_3)){
			mLayer7->setVisible(false);
			mLayer8->setVisible(false);
			mLayer9->setVisible(true);
			weaponChoice = 3;
			soundMgr->playAudio(tickId,true);
		}


		if (keyboard_->isKeyDown(OIS::KC_BACK)){
			gameState = 0;
			menuNumber = 0;
			Replay();
			soundMgr->playAudio( audioId, true);
			soundMgr->pauseAudio(tweetId);
			soundMgr->pauseAudio(windId);
			soundMgr->pauseAudio(oceanId);

		}

		if (keyboard_->isKeyDown(OIS::KC_C)){

				cam_mode = true;
		
		}

		if (keyboard_->isKeyDown(OIS::KC_V)){

				cam_mode = false;
		
		}

		if (mapNumber != 4){
			if (!ret.collided || !retv.collided || !retd.collided){
				if (body->getPosition().x < 0){
					body->setPosition(body->getPosition().x+2, body->getPosition().y, body->getPosition().z);
					lwing->setPosition(lwing->getPosition().x+2, lwing->getPosition().y, lwing->getPosition().z);
					rwing->setPosition(rwing->getPosition().x+2, rwing->getPosition().y, rwing->getPosition().z);
				}
				if (body->getPosition().x > 0){
					body->setPosition(body->getPosition().x-2, body->getPosition().y, body->getPosition().z);
					lwing->setPosition(lwing->getPosition().x-2, lwing->getPosition().y, lwing->getPosition().z);
					rwing->setPosition(rwing->getPosition().x-2, rwing->getPosition().y, rwing->getPosition().z);
				}
				if (body->getPosition().y < 0){
					body->setPosition(body->getPosition().x, body->getPosition().y+2, body->getPosition().z);
					lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y+2, lwing->getPosition().z);
					rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y+2, rwing->getPosition().z);
				}
				if (body->getPosition().y >= 0){
					body->setPosition(body->getPosition().x, body->getPosition().y-2, body->getPosition().z);
					lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y-2, lwing->getPosition().z);
					rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y-2, rwing->getPosition().z);
				}
				if (body->getPosition().z < 0){
					body->setPosition(body->getPosition().x, body->getPosition().y, body->getPosition().z+2);
					lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y, lwing->getPosition().z+2);
					rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y, rwing->getPosition().z+2);
				}
				if (body->getPosition().z > 0){
					body->setPosition(body->getPosition().x, body->getPosition().y, body->getPosition().z-2);
					lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y, lwing->getPosition().z-2);
					rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y, rwing->getPosition().z-2);
				}
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
		}

		

		if (mapNumber == 1){
			if (body->getPosition().y < -134){
				body->setPosition(body->getPosition().x, body->getPosition().y+1, body->getPosition().z);
				lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y+1, lwing->getPosition().z);
				rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y+1, rwing->getPosition().z);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			
			
		}
		if (mapNumber == 2){
			if (body->getPosition().y < 923){
				body->setPosition(body->getPosition().x, body->getPosition().y+1, body->getPosition().z);
				lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y+1, lwing->getPosition().z);
				rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y+1, rwing->getPosition().z);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
		}
		if (mapNumber == 3){
			if (body->getPosition().y < 1922){
				body->setPosition(body->getPosition().x, body->getPosition().y+1, body->getPosition().z);\
				lwing->setPosition(lwing->getPosition().x, lwing->getPosition().y+1, lwing->getPosition().z);
				rwing->setPosition(rwing->getPosition().x, rwing->getPosition().y+1, rwing->getPosition().z);
				warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14%Your position is outside the flying area, Retreat!");
			}
			//std::cout << "retv is: " << retv.collided << std::endl;
		}

		//camera control and player movement for both first person camera and third person camera
		if (cam_mode == false){

			/*control the player along with the camera */
			if (keyboard_->isKeyDown(OIS::KC_UP)){
				body->rotate(xq);
				lwing->rotate(xq);
				rwing->rotate(xq);
				if (featherShot == false){
					farrow->rotate(nzq);
				}
				//farrow->pitch(rot_factor);
				//camera_reference->rotate(cxq);
				//camera->rotate(xq);

			}
    
			if (keyboard_->isKeyDown(OIS::KC_DOWN)){
				body->rotate(nxq);
				lwing->rotate(nxq);
				rwing->rotate(nxq);
				if (featherShot == false){
					farrow->rotate(zq);
				}
				//farrow->pitch(-rot_factor);
				//camera->rotate(nxq);
				//camera_reference->rotate(cnxq);
			}

			if (keyboard_->isKeyDown(OIS::KC_LEFT)){
				body->rotate(zq);
				lwing->rotate(zq);
				rwing->rotate(zq);
				if (featherShot == false){
					farrow->rotate(nxq);
				}
				//farrow->yaw(rot_factor);
				//camera_reference->rotate(cnyq);
				//camera->yaw(rot_factor);
				//camera->rotate(yq);
			}

			if (keyboard_->isKeyDown(OIS::KC_RIGHT)){
				body->rotate(nzq);
				lwing->rotate(nzq);
				rwing->rotate(nzq);
				if (featherShot == false){
					farrow->rotate(xq);
				}
				//farrow->yaw(-rot_factor);
				//camera_reference->rotate(cyq);
				//camera->rotate(nyq);
			}
			if (mapNumber == 1){
				if (ret.collided && body->getPosition().y>=-134){
					if (keyboard_->isKeyDown(OIS::KC_S)){
						body->translate(playerO * Ogre::Vector3(0, 0,velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						//camera_reference->translate(playerO * Ogre::Vector3(0, 0.05, 0));
						farrow->translate(playerO * Ogre::Vector3(0, velocity,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_W)){
						body->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						farrow->translate(playerO * Ogre::Vector3(0, -velocity,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0, -0.05, 0));
					}
					if (keyboard_->isKeyDown(OIS::KC_D)){
						body->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						//camera_reference->translate(playerO * Ogre::Vector3(-0.05, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(velocity, 0,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_A)){
						body->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(-velocity, 0,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0.05,0,0));
					}
					warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
				}
			}
			if (mapNumber == 2){
				if (retd.collided && body->getPosition().y>=923){
					if (keyboard_->isKeyDown(OIS::KC_S)){
						body->translate(playerO * Ogre::Vector3(0, 0,velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						//camera_reference->translate(playerO * Ogre::Vector3(0, 0.05, 0));
						farrow->translate(playerO * Ogre::Vector3(0, velocity,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_W)){
						body->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						farrow->translate(playerO * Ogre::Vector3(0, -velocity,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0, -0.05, 0));
					}
					if (keyboard_->isKeyDown(OIS::KC_D)){
						body->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						//camera_reference->translate(playerO * Ogre::Vector3(-0.05, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(velocity, 0,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_A)){
						body->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(-velocity, 0,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0.05,0,0));
					}
					warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
				}
			}
			if (mapNumber == 3){
				if (retv.collided && body->getPosition().y>=1922){
					if (keyboard_->isKeyDown(OIS::KC_S)){
						body->translate(playerO * Ogre::Vector3(0, 0,velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						//camera_reference->translate(playerO * Ogre::Vector3(0, 0.05, 0));
						farrow->translate(playerO * Ogre::Vector3(0, velocity,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_W)){
						body->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						farrow->translate(playerO * Ogre::Vector3(0, -velocity,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0, -0.05, 0));
					}
					if (keyboard_->isKeyDown(OIS::KC_D)){
						body->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						//camera_reference->translate(playerO * Ogre::Vector3(-0.05, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(velocity, 0,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_A)){
						body->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(-velocity, 0,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0.05,0,0));
					}
					warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
				}
			}
			if (mapNumber == 4){
				if (rets.collided && body->getPosition().y>2936){
					if (keyboard_->isKeyDown(OIS::KC_S)){
						body->translate(playerO * Ogre::Vector3(0, 0,velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0,velocity));
						//camera_reference->translate(playerO * Ogre::Vector3(0, 0.05, 0));
						farrow->translate(playerO * Ogre::Vector3(0, velocity,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_W)){
						body->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						lwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						rwing->translate(playerO * Ogre::Vector3(0, 0, -velocity));
						farrow->translate(playerO * Ogre::Vector3(0, -velocity,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0, -0.05, 0));
					}
					if (keyboard_->isKeyDown(OIS::KC_D)){
						body->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(velocity, 0, 0));
						//camera_reference->translate(playerO * Ogre::Vector3(-0.05, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(velocity, 0,0));
					}

					if (keyboard_->isKeyDown(OIS::KC_A)){
						body->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						lwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						rwing->translate(playerO * Ogre::Vector3(-velocity, 0, 0));
						farrow->translate(playerO * Ogre::Vector3(-velocity, 0,0));
						//camera_reference->translate(playerO * Ogre::Vector3(0.05,0,0));
					}
					warningMsg = mLayer6->createMarkupText(14,vpW/2-24, vpH/2+10, "%@14% ");
				}
			}
			
			
		}
		if (cam_mode == true){
			/*control the player along with the camera */
			if (keyboard_->isKeyDown(OIS::KC_DOWN)){
				body->rotate(xq);
				lwing->rotate(xq);
				rwing->rotate(xq);
				if (featherShot == false){
					farrow->rotate(nzq);
				}
				//camera_reference->rotate(xq);//, Ogre::Node::TransformSpace::TS_WORLD);
				//camera->rotate(xq);

			}
    
			if (keyboard_->isKeyDown(OIS::KC_UP)){
				body->rotate(nxq);
				lwing->rotate(nxq);
				rwing->rotate(nxq);
				if (featherShot == false){
					farrow->rotate(zq);
				}
				//camera->rotate(nxq);
				//camera_reference->rotate(nxq);//, Ogre::Node::TransformSpace::TS_WORLD);
			}

			if (keyboard_->isKeyDown(OIS::KC_RIGHT)){
				body->rotate(yq);
				lwing->rotate(yq);
				rwing->rotate(yq);
				if (featherShot == false){
					farrow->rotate(nyq);
				}
				//camera_reference->rotate(nyq);//, Ogre::Node::TransformSpace::TS_WORLD);
				//camera->yaw(rot_factor);
				//camera->rotate(yq);
			}

			if (keyboard_->isKeyDown(OIS::KC_LEFT)){
				body->rotate(nyq);
				lwing->rotate(nyq);
				rwing->rotate(nyq);
				if (featherShot == false){
					farrow->rotate(yq);
				}
				//camera_reference->rotate(yq);//, Ogre::Node::TransformSpace::TS_WORLD);
				//camera->rotate(nyq);
			}

			if (keyboard_->isKeyDown(OIS::KC_W)){
				body->translate(playerO * Ogre::Vector3(0, velocity,0));
				lwing->translate(playerO * Ogre::Vector3(0, velocity,0));
				rwing->translate(playerO * Ogre::Vector3(0, velocity,0));
				farrow->translate(playerO * Ogre::Vector3(0, -velocity,0));
				//camera_reference->translate(playerO * Ogre::Vector3(0, 0.005, 0));
				//playerZ = playerZ + 0.005;
			}

			if (keyboard_->isKeyDown(OIS::KC_S)){
				body->translate(playerO * Ogre::Vector3(0, -velocity, 0));
				lwing->translate(playerO * Ogre::Vector3(0, -velocity, 0));
				rwing->translate(playerO * Ogre::Vector3(0, -velocity, 0));
				farrow->translate(playerO * Ogre::Vector3(0, velocity,0));
				//camera_reference->translate(playerO * Ogre::Vector3(0, -0.005, 0));
				//playerZ = playerZ - 0.005;
			}
		
			if (keyboard_->isKeyDown(OIS::KC_D)){
				camera->yaw(rot_factor);
			}

			if (keyboard_->isKeyDown(OIS::KC_A)){
				camera->yaw(-rot_factor);
			}

		}

		//update camera system, update UI and update weapon and combat system
		CameraControl();
		UpdateUIInfo();
		CombatReady();
	}

	worldRotation();

    return true;
}

void OgreApplication::worldRotation(){
	sky->yaw(Ogre::Radian(0.0002));
	sunset->yaw(Ogre::Radian(0.0002));
	cloudy->yaw(Ogre::Radian(0.0002));
	night->yaw(Ogre::Radian(-0.0002));
	water->yaw(Ogre::Radian(-0.0002));
}

void OgreApplication::windowResized(Ogre::RenderWindow* rw){

	/* Update the window and aspect ratio when the window is resized */
	int width = rw->getWidth(); 
    int height = rw->getHeight();
      
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::Camera* camera = scene_manager->getCamera("MyCamera");

	if (camera != NULL){
		//std::cout << "1 " << (double)width/height << std::endl;
		camera->setAspectRatio((double)width/height);
    }

	const OIS::MouseState &ms = mouse_->getMouseState();
    ms.width = width;
    ms.height = height;

	ogre_window_->resize(width, height);
	ogre_window_->windowMovedOrResized();
	ogre_window_->update();
}


} // namespace ogre_application;
