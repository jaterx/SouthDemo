#pragma once

#include <exception>
#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/Overlay/OgreOverlayManager.h"
#include "OGRE/Overlay/OgreOverlayContainer.h"
#include "OGRE/Overlay/OgreTextAreaOverlayElement.h"
#include "OGRE/Overlay/OgreOverlaySystem.h"
#include "OgreStringConverter.h"
#include "OIS/OIS.h"
#include "Gorilla.h"
#include "CollisionTools.h"
//#include "audio_manager.h"
#include "SoundManager.h"

namespace ogre_application {


	/* A useful type to define */
	typedef std::vector<Ogre::String> Strings;

	/* Our exception type */
	class OgreAppException: public std::exception
    {
		private:
		    std::string message_;
	    public:
			OgreAppException(std::string message) : message_(message) {};
			virtual const char* what() const throw() { return message_.c_str(); };
	};
	

	/* Our Ogre application */
	class OgreApplication :
	    public Ogre::FrameListener, // Derive from FrameListener to be able to have render event callbacks
        public Ogre::WindowEventListener // Derive from FrameListener to be able to have window event callbacks
		{

        public:
            OgreApplication(void);
			// Call Init() before running the main loop
            void Init(void); 

			//game states related
			int gameState;
			//0: init, 1: mainmenu 2:map

            Ogre::SceneNode* OgreApplication::CreateEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);

			//world creation
			void OgreApplication::CreateLighting();
			Ogre::SceneNode* OgreApplication::CreateSky();
			Ogre::SceneNode* OgreApplication::CreateSunset();
			Ogre::SceneNode* OgreApplication::CreateCloudy();
			Ogre::SceneNode* OgreApplication::CreateNight();

			//character creation
			Ogre::SceneNode* OgreApplication::CreateBody();
			Ogre::SceneNode* OgreApplication::CreateLeftWing();
			Ogre::SceneNode* OgreApplication::CreateRightWing();
			Ogre::SceneNode* OgreApplication::CreateWolfBody();
			Ogre::SceneNode* OgreApplication::CreateWolfFront();
			Ogre::SceneNode* OgreApplication::CreateWolfRear();
			Ogre::SceneNode* OgreApplication::CreateCrowBody();
			Ogre::SceneNode* OgreApplication::CreateCrowL();
			Ogre::SceneNode* OgreApplication::CreateCrowR();

			//world creation
			Ogre::Light* light;
			Ogre::SceneNode* OgreApplication::CreateFirn();
			Ogre::SceneNode* OgreApplication::CreateDesert();
			Ogre::SceneNode* OgreApplication::CreateValley();
			Ogre::SceneNode* OgreApplication::CreateIsland();
			Ogre::SceneNode* OgreApplication::CreateWater();
			Ogre::SceneNode* OgreApplication::CreateGate();
			Ogre::SceneNode* OgreApplication::CreateGate2();
			Ogre::SceneNode* OgreApplication::CreateBeacon();
			Ogre::SceneNode* OgreApplication::CreateHouse();

			//placeholder shapes creation (this part will not be displayed for the final demo)
			//tree generating
			Ogre::SceneNode* OgreApplication::CreateCube(); // Create the geometry for a cube
			Ogre::SceneNode** OgreApplication::CreateCubes();
			Ogre::SceneNode** OgreApplication::CreatePines();
			Ogre::SceneNode** OgreApplication::CreateCoco();
			Ogre::SceneNode** OgreApplication::CreateCatcus();
			Ogre::SceneNode** OgreApplication::CreateOak();
			Ogre::SceneNode** piness;
			Ogre::SceneNode** catcuss;
			Ogre::SceneNode** cocoss;
			Ogre::SceneNode** oaks;
			int random_x;
			int random_x2;
			int random_x3;
			int random_x4;
			int random_x5;
			int random_x6;
			int random_y;
			int random_z;
			int corrector;

			//world animation
			void waterMovement();
			void worldRotation();

			Ogre::SceneNode* cube_reference;
			Ogre::SceneNode** cube_array;
			Ogre::SceneNode** pine_array;
			Ogre::SceneNode** catcus_array;
			Ogre::SceneNode** coco_array;
			Ogre::SceneNode** oak_array;

			//initial trasformation related
			void OgreApplication::BodyTransformation(Ogre::SceneNode* node, float hover);
			void OgreApplication::LwingTransformation(Ogre::SceneNode* node, float paddle);
			void OgreApplication::RwingTransformation(Ogre::SceneNode* node, float paddle);
			void OgreApplication::WolfBodyTransformation(Ogre::SceneNode* node);
			void OgreApplication::WolfFrontTransformation(Ogre::SceneNode* node, float run);
			void OgreApplication::WolfRearTransformation(Ogre::SceneNode* node, float run);
			void OgreApplication::CrowBodyTransformation(Ogre::SceneNode* node, float hover);
			void OgreApplication::CrowLTransformation(Ogre::SceneNode* node, float run);
			void OgreApplication::CrowRTransformation(Ogre::SceneNode* node, float run);

			//character object
			Ogre::SceneNode* lwing;
			Ogre::SceneNode* rwing;
			Ogre::SceneNode* body;
			Ogre::SceneNode* wolfbody;
			Ogre::SceneNode* wolffront;
			Ogre::SceneNode* wolfrear;
			Ogre::SceneNode* clwing;
			Ogre::SceneNode* crwing;
			Ogre::SceneNode* cbody;
			Ogre::SceneNode* cleft;
			Ogre::SceneNode* cright;
			Ogre::Matrix4 root;
			Ogre::Matrix4 wolfroot;
			Ogre::Matrix4 menuroot;
			Ogre::Matrix4 croot;

			//backup character object
			Ogre::SceneNode* body2;
			Ogre::SceneNode* cbody2;
			Ogre::SceneNode* wolfbody2;
			Ogre::SceneNode* wolffront2;
			Ogre::SceneNode* wolfrear2;

			//environment object
			Ogre::SceneNode* sky;
			Ogre::SceneNode* cloudy;
			Ogre::SceneNode* sunset;
			Ogre::SceneNode* night;
			Ogre::SceneNode* water;
			Ogre::SceneNode* firn;
			Ogre::SceneNode* island;
			Ogre::SceneNode* valley;
			Ogre::SceneNode* desert;
			Ogre::SceneNode* gate;
			Ogre::SceneNode* gate2;
			Ogre::SceneNode* beacon;
			Ogre::SceneNode* house;

			//collision detection
			Collision::SCheckCollisionAnswer ret; //detect collision between the player and firn mesh bounding box
			Collision::SCheckCollisionAnswer retd;//detect collision between the player and desert mesh bounding box
			Collision::SCheckCollisionAnswer retv;//detect collision between the player and valley mesh bounding box
			Collision::SCheckCollisionAnswer rets;//detect collision between the player and sky mesh sphere bounding box
			Collision::SCheckCollisionAnswer retw;//detect collision between the player and water mesh bounding box
			Collision::SCheckCollisionAnswer reth;//detect collision between the player and house mesh bounding box
			Collision::SCheckCollisionAnswer rett;//detect collision between the player and tent mesh bounding box
			Collision::SCheckCollisionAnswer retc;//detect collision between the player and castle mesh bounding box
			Collision::SCheckCollisionAnswer retb;//detect collision between the player and beacon mesh bounding box
			Collision::CollisionTools collision_tools;
			Ogre::Entity* playerMesh;
			Ogre::Entity* firnMesh;
			Ogre::Entity* desertMesh;
			Ogre::Entity* islandMesh;
			Ogre::Entity* valleyMesh;
			Ogre::Entity* skyMesh;
			Ogre::Entity* waterMesh;
			Ogre::Entity* houseMesh;
			Ogre::Entity* tentMesh;
			Ogre::Entity* castleMesh;
			Ogre::Entity* beaconMesh;
			void OgreApplication::playerVsBuildingCollision();
			Ogre::AxisAlignedBox housebox;
			Ogre::AxisAlignedBox tentbox;
			Ogre::AxisAlignedBox castlebox;
			Ogre::AxisAlignedBox beaconbox;
			bool housec;
			bool tentc;
			bool castlec;
			bool beaconc;

			//combat related
			int weaponChoice;
			Ogre::SceneNode* farrow;
			Ogre::SceneNode* hysteriaFire;
			Ogre::SceneNode* hysteriaBlue;
			Ogre::SceneNode* hysteriaGreen;
			float featherDistance;
			void OgreApplication::CombatReady();
			Ogre::SceneNode* OgreApplication::CreateFeather();
			bool featherShot;
			void OgreApplication::setUp(); //init enemey initial position
			void OgreApplication::EnemyMovement(); // control enemey movement

			//offset according to enemies' original positions
			float cxmovement;
			float cymovement;
			float wxmovement;
			float wymovement;
			float wfmovement;
			float wrmovement;
			Ogre::Vector3 wposf;
			Ogre::Vector3 wposr;

			//crosshair UI feedback
			bool colorChange;
			bool whit;
			bool fchit;
			bool chit;
			bool fwhit;
			bool wdhit;
			bool cdhit;
			int wdamage;
			int cdamage;
			void OgreApplication::playerVsEnemyCollision();
			bool wdeath;
			bool cdeath;
			void OgreApplication::Replay();
			int bounceTimes;
			float recordX;
			float recordY;
			float recordZ;

			//sound related
			void OgreApplication::InitSound();
			//audio_manager::AudioManager am;
			SoundManager* soundMgr;

			//sound effect ID
			unsigned int audioId;
			unsigned int windId;
			unsigned int tweetId;
			unsigned int oceanId;
			unsigned int crowId;
			unsigned int arrowId;
			unsigned int burningId;
			unsigned int stabId;
			unsigned int teleportId;
			unsigned int clearId;
			unsigned int warningId;
			unsigned int explodeId;
			unsigned int tickId;

			//ui objects
			Ogre::SceneNode* play;
			Ogre::SceneNode* option;
			Ogre::SceneNode* exit;
			Ogre::SceneNode* credits;
			Ogre::SceneNode* title;
			Ogre::SceneNode* OgreApplication::CreatePlay();
			Ogre::SceneNode* OgreApplication::CreateOption();
			Ogre::SceneNode* OgreApplication::CreateExit();
			Ogre::SceneNode* OgreApplication::CreateCredits();
			Ogre::SceneNode* OgreApplication::CreateTitle();
			void OgreApplication::MenuTransformation(Ogre::SceneNode* node);
			int menuNumber;
			void OgreApplication::MenuControl();
			float swing_;
			bool swingSwitch;
			Ogre::Quaternion tq;
			void OgreApplication::UpdateUIInfo();
			void OgreApplication::InitUI();
			Ogre::Viewport * viewport;
			int mapNumber; //1:firn, 2:desert, 3:valley, 4:island
			float defaultLongt, defaultLat;
			Ogre::SceneNode* tempSwaper;
			Ogre::SceneNode* pineTransmitter;

			//particle system related
			void CreateParticleGeometry(Ogre::String object_name, int num_particles);
			void CreateBloodGeometry(Ogre::String object_name, int num_particles);
			void CreateRingGeometry(Ogre::String object_name, int num_particles, float loop_radius = 0.1, float circle_radius = 0.3); // Create ring
			void CreateTorus(Ogre::String object_name, float loop_radius = 0.6, float circle_radius = 0.2, int num_loop_samples = 90, int num_circle_samples = 30);
			Ogre::SceneNode* CreateBloodEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity2(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity3(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity4(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity5(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity6(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity7(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* CreateParticleEntity8(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			void CreateControlPoints(Ogre::String control_points_name, int num_control_points, Ogre::String material_name);
			void CreateControlPointEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
			Ogre::SceneNode* snow;
			Ogre::SceneNode* blood;
			Ogre::SceneNode* tail;
			Ogre::SceneNode* shockwave;
			Ogre::SceneNode* splines;
			Ogre::SceneNode** particleArray;
			bool animating_particles_; // Whether the animation of particles is on or off
			Ogre::String particle_material_name_;
			Ogre::String particle_material_name_2;
			Ogre::String particle_material_name_3;
			Ogre::String particle_material_name_4;
			Ogre::String particle_material_name_5;
			Ogre::String particle_material_name_6;
			Ogre::String particle_material_name_7;
			Ogre::String particle_material_name_8;

			//ui objects from outside sources
			//loading screen
			Gorilla::Silverback*    mLoading;
			Gorilla::Screen*        mLoadingScreen;
			Gorilla::Layer*         mLoadingLayer;
			Gorilla::Caption*       loading_strc;

			//in game
			Ogre::Real vpH, vpW;
			Ogre::Real vpH2, vpW2;

			//create 10 overlay layers for user interface
			Gorilla::Silverback*    mSilverback;
			Gorilla::Silverback*    mSilverback2;
			Gorilla::Screen*        mScreen; //coor, status bar, crosshair
			Gorilla::Layer*         mLayer;
			Gorilla::Screen*        mScreen2; //firn mini map
			Gorilla::Layer*         mLayer2;
			Gorilla::Screen*        mScreen3; //desert mini map
			Gorilla::Layer*         mLayer3;
			Gorilla::Screen*        mScreen4; //valley mini map
			Gorilla::Layer*         mLayer4;
			Gorilla::Screen*        mScreen5; //island mini map
			Gorilla::Layer*         mLayer5;
			Gorilla::Screen*        mScreen6; //mini map indicator
			Gorilla::Layer*         mLayer6;
			Gorilla::Screen*        mScreen7; //arrow
			Gorilla::Layer*         mLayer7;
			Gorilla::Screen*        mScreen8; //feather
			Gorilla::Layer*         mLayer8;
			Gorilla::Screen*        mScreen9; //feces
			Gorilla::Layer*         mLayer9;
			Gorilla::Screen*        mScreen10; //frame
			Gorilla::Layer*         mLayer10;

			//texts for credits menus
			Gorilla::MarkupText*     creditsText;
			Gorilla::MarkupText*     creditsTextLabel;
			Gorilla::MarkupText*     creditsTextLabel2;
			Gorilla::MarkupText*     creditsTextLabel3;

			Gorilla::MarkupText*     markup;
			Gorilla::Rectangle*      rect2;
			Gorilla::Rectangle*      thirst_bar;
			Gorilla::Rectangle*      hunger_bar;
			Gorilla::Rectangle*      rect;
			Gorilla::Caption*        caption;
			Gorilla::Caption*        caption2;
			Gorilla::Caption*        lw;
			Gorilla::Caption*        ln;
			Gorilla::Caption*        crosshair;
			Gorilla::MarkupText*        winfo;
			Gorilla::MarkupText*        cinfo;
			Gorilla::Caption*        enemyInfo;
			Gorilla::Caption*        hunger_strc;
			Gorilla::Caption*        thirst_strc;
			Gorilla::Caption*        mini_pos;
			int whpoint;
			int chpoint;
			Ogre::ColourValue green;
			Ogre::ColourValue orange;
			Ogre::ColourValue red;
			Ogre::ColourValue blue;
			Ogre::ColourValue black;
			Ogre::ColourValue white;

			//map coordinates
			float longt, lat;

			std::string longtD, latD;
			std::string hunger_str, thirst_str;
			Gorilla::Rectangle* firn_bg;
			Gorilla::Rectangle* desert_bg;
			Gorilla::Rectangle* valley_bg;
			Gorilla::Rectangle* island_bg;
			Gorilla::Rectangle* warrow;
			Gorilla::Rectangle* wfeces;
			Gorilla::Rectangle* wbeak;
			Gorilla::Rectangle* frame;
			Gorilla::Rectangle* label;
			float hunger, thirst, hunger2, thirst2, hunger3, thirst3, hunger4, thirst4;
			int mini_posx, mini_posy;
			Ogre::Timer* timer;
			Ogre::Timer*  curr;

			//tutorial and instruction information texts
			Gorilla::MarkupText*  warningMsg;
			Gorilla::MarkupText*  mission;
			Gorilla::MarkupText*  detonation;
			Gorilla::Caption*     cleared;
			Gorilla::Caption*     cleared2;
			Gorilla::Caption*     cleared3;

			//texts and symbols use for display option menu information texts
			Gorilla::MarkupText*  optionName;
			Gorilla::Caption*     optionAnchor;
			String     optionSetting;
			String     optionSetting2;
			String     optionSetting3;
			String     optionSetting4;
			String     optionSetting5;
			Gorilla::Caption*     optionSettingFake;
			Gorilla::Caption*     optionSetting2Fake;
			Gorilla::Caption*     optionSetting3Fake;
			Gorilla::Caption*     optionSetting4Fake;
			Gorilla::Caption*     optionSetting5Fake;
			int touched;
			int optionLayer;

			//camera related
			Ogre::Quaternion cameraO;
			Ogre::Quaternion playerO;
			double cameraX;
			double cameraY;
			double cameraZ;
			double playerX;
			double playerY;
			double playerZ;
			Ogre::SceneNode* camera_reference;
			Ogre::SceneNode* rotation_reference;
			float velocity;
			float velocity2;
			bool cam_mode;
			Ogre::Camera* camera;
			bool runOnce;
			void CameraControl(); //function use for camera system
			
			// Keep application active
            void MainLoop();

			//level set up related variables
			int level; //1firn, 2desert, 3valley, 4island

        private:
			// Create root that allows us to access Ogre commands
            std::auto_ptr<Ogre::Root> ogre_root_;
            // Main Ogre window
            Ogre::RenderWindow* ogre_window_;

			//animation parameters
			float timer_;
			float paddle_;
			float run_;
			float run2_;
			float hover_;
			bool runSwitch;
			bool run2Switch;

			// For animating an object
			Ogre::AnimationState *animation_state_; // Keep state of the animation
			bool animating_; // Whether animation is on or off
			bool space_down_; // Whether space key was pressed

			// Input managers
			OIS::InputManager *input_manager_;
			OIS::Mouse *mouse_;
			OIS::Keyboard *keyboard_;

			/* Methods to initialize the application */
			void InitRootNode(void);
			void InitPlugins(void);
			void InitRenderSystem(void);
			void InitWindow(void);
			void InitViewport(void);
			void InitEvents(void);
			void InitOIS(void);
			void LoadMaterials(void);

			/* Methods to handle events */
			bool frameRenderingQueued(const Ogre::FrameEvent& fe);
			void windowResized(Ogre::RenderWindow* rw);

    }; // class OgreApplication
} // namespace ogre_application;


