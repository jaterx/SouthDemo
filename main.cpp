#include <iostream>
#include <exception>
#include "ogre_application.h"

/* Macro for printing exceptions */
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

/* Main function that builds and runs the application */
int main(void){
    ogre_application::OgreApplication application;

	try {
        // Initialize application
		std::cout << "INITIALZING THE GAME ENGINE" << std::endl;
		application.Init();
		std::cout << "CONSTRUCTING THE GAME WORLD" << std::endl;
		//create environment for all scenes
		std::cout << "  Create Lighting..." << std::endl;
		application.CreateLighting();

		std::cout << "  Create Reference Models..." << std::endl;
		application.cube_reference = application.CreateCube();
		application.cube_reference->setVisible(false);
		application.cube_array = application.CreateCubes();
		application.camera_reference = application.cube_array[0];
		

		std::cout << "  Initialzing User Interfaces..." << std::endl;
		//init ui
		application.play = application.CreatePlay();
		application.option = application.CreateOption();
		application.credits = application.CreateCredits();
		application.exit = application.CreateExit();
		application.title = application.CreateTitle();
		application.title->rotate(application.tq);

		std::cout << "  Building Game Characters..." << std::endl;
		//init character
		application.body = application.CreateBody();
		application.lwing = application.CreateLeftWing();
		application.rwing = application.CreateRightWing();
		application.wolfbody = application.CreateWolfBody();
		application.wolffront = application.CreateWolfFront();
		application.wolfrear = application.CreateWolfRear();
		application.cbody = application.CreateCrowBody();
		application.clwing = application.CreateCrowL();
		application.crwing = application.CreateCrowR();

		//init weapon
		application.farrow = application.CreateFeather();

		std::cout << "  Transforming the Game World..." << std::endl;
		application.BodyTransformation(application.body, 0);
		application.CrowBodyTransformation(application.cbody, 0);
		application.WolfBodyTransformation(application.wolfbody);
		application.MenuTransformation(application.play);
		application.MenuTransformation(application.option);
		application.MenuTransformation(application.credits);
		application.MenuTransformation(application.exit);
		application.MenuTransformation(application.title);

		std::cout << "  Visualizing the Game World..." << std::endl;
		application.sky = application.CreateSky();
		application.sunset = application.CreateSunset();
		application.cloudy = application.CreateCloudy();
		application.night = application.CreateNight();
		application.water = application.CreateWater();
		application.firn = application.CreateFirn();
		application.desert = application.CreateDesert();
		application.valley = application.CreateValley();
		application.island = application.CreateIsland();
		application.gate = application.CreateGate();
		application.gate2 = application.CreateGate2();
		application.beacon = application.CreateBeacon();
		application.house = application.CreateHouse();

		std::cout << "  Setting Visualization Logics..." << std::endl;
		application.camera_reference->setVisible(false);
		application.body->setVisible(false);
		application.lwing->setVisible(false);
		application.rwing->setVisible(false);
		application.wolfbody->setVisible(false);
		application.wolffront->setVisible(false);
		application.wolfrear->setVisible(false);
		application.firn->setVisible(true);
		application.desert->setVisible(false);
		application.valley->setVisible(false);
		application.island->setVisible(false);

		application.firn->showBoundingBox(false);

		//init particle
		std::cout << "  Initialzing Particle System..." << std::endl;
		application.CreateParticleGeometry("Particles", 1000000);
		application.CreateParticleGeometry("Splines", 300000);
		application.CreateBloodGeometry("Particles2", 10000);
		application.CreateBloodGeometry("Hysteria", 20);
		application.CreateRingGeometry("Rings", 100000);
		application.CreateControlPoints("ControlPoints1", 64, "SplineParticleMaterial");
		application.snow = application.CreateParticleEntity("ParticleEnt", "Particles", "ExhaustMaterial");
		application.blood = application.CreateParticleEntity2("ParticleEnt2", "Particles2", "BloodMaterial");
		application.tail = application.CreateParticleEntity3("ParticleEnt3", "Particles2", "TailMaterial");
		application.shockwave = application.CreateParticleEntity4("ParticleEnt4", "Rings", "TailMaterial2");
		application.splines = application.CreateParticleEntity5("ParticleEnt5", "Splines", "SplineParticleMaterial");
		application.hysteriaFire = application.CreateParticleEntity6("ParticleEnt6", "Hysteria", "HysteriaMaterial");
		application.hysteriaGreen = application.CreateParticleEntity7("ParticleEnt7", "Hysteria", "HysteriaGreenMaterial");
		application.hysteriaBlue = application.CreateParticleEntity8("ParticleEnt8", "Hysteria", "HysteriaBlueMaterial");
		application.hysteriaFire->setVisible(false);
		application.hysteriaGreen->setVisible(false);
		application.hysteriaBlue->setVisible(false);
		//application.hysteria->scale(15.0f,15.0f,15.0f); //first person scale
		//application.hysteria->scale(5.0f,5.0f,5.0f);

		application.shockwave->setVisible(false);
		application.blood->setVisible(false);
		application.tail->setVisible(false);
		application.splines->setPosition(0,2000,0);

		std::cout << "  Animating the Game World..." << std::endl;
		application.worldRotation();
		application.runOnce = true;
		application.InitUI();

		std::cout << "  Generating Trees..." << std::endl;
		application.piness = application.CreatePines();
		application.catcuss = application.CreateCatcus();
		application.cocoss = application.CreateCoco();
		application.oaks = application.CreateOak();

		//application.valley->showBoundingBox(true);
		
		std::cout << "  Setting AI logics..." << std::endl;
		application.setUp();
        // Run application
		std::cout << "STARTING THE GAME..." << std::endl;
		application.MainLoop();
	}
	catch (std::exception &e){
		PrintException(e);
	}

    return 0;
}
