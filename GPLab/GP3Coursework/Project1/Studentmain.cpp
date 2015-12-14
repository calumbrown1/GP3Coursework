#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


//#include <windows.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"
#include "cPyramid.h"
#include "cCube.h"
#include "cSphere.h"
#include "cMaterial.h"
#include "cLight.h"
#include "cStarfield.h"
#include "cFontMgr.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cLaser.h"
#include "tardisWarsGame.h"
#include "cAlien.h"

void GameOver(int newGameState); // foward declaration for game over method
int gameState = 3;
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;
	float spawnTime= 3.0f;
	float curSpawnTime = 0.0f;
	int enemyNumber = 0;
	int enemiesPast = 0;
	float starSpawnTime = 0.5f;
	float curStarSpawnTime = 0.0f;
	bool camType = true;
	bool musicOn = true;
    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
	
	//The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);


    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	// Create Texture map
	cTexture tardisTexture;
	tardisTexture.createTexture("Models/tardis.png");
	cTexture spaceShipTexture;
	spaceShipTexture.createTexture("Models/SpaceShip/sh3.jpg");
	cTexture laserTexture;
	laserTexture.createTexture("Models/Laser/Red.png");
	cTexture starTexture;
	starTexture.createTexture("Models/Star/StarTex.png");
	cTexture playerTex;
	playerTex.createTexture("Models/Player/Grey.png");
	cTexture expTex;
	expTex.createTexture("Models/Explosion/Explosion.png");

	// the starfield
	//cStarfield theStarField(starTexture.getTexture(), glm::vec3(50.0f, 50.0f, 50.0f));

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);

	// Create Light
	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 20, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight lfLight(GL_LIGHT1, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight rfLight(GL_LIGHT2, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(-30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight cbLight(GL_LIGHT3, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, -100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	// load game fonts
	// Load Fonts
	LPCSTR gameFonts[3] = {"Fonts/JustinFont12Bold.ttf" };

	theFontMgr->addFont("ftFont", gameFonts[0], 24);

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/music.wav", "Audio/shoot.wav", "Audio/boom.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// Create a camera
	cCamera theCamera;
	theCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 75.0f));
	theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera.setTheProjectionMatrix(45.0f, theCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
	theCamera.update();

	//Create second camera
	cCamera thirdPersCamera;
	thirdPersCamera.setTheCameraPos(glm::vec3(0.0f, -65.0f, 20.0f));
	thirdPersCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	thirdPersCamera.setTheCameraUpVector(glm::vec3(0.0f, -1.0f, 0.0f)); // pointing upwards in world space
	thirdPersCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	thirdPersCamera.setTheProjectionMatrix(25.0f, thirdPersCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	// Model
	cModelLoader tardisMdl;
	tardisMdl.loadModel("Models/Player/thePlayer.obj", playerTex); // Player

	cModelLoader spaceShipMdl;
	spaceShipMdl.loadModel("Models/Enemy/newEnemy.obj", starTexture); // Enemy
	
	cModelLoader theLaser;
	theLaser.loadModel("Models/Laser/Laser.obj", laserTexture);

	cModelLoader star;
	star.loadModel("Models/Star/StarModelNew.obj", starTexture);

	cModelLoader explosion;
	explosion.loadModel("Models/Explosion/Explosion.obj", expTex);

	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, -20, -20), 180.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(tardisMdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);
	thePlayer.setScale(glm::vec3(0.15, 0.15, 0.15));
	thePlayer.setSpeed(20.0f);

	float tCount = 0.0f;
	string outputMsg;

	theSoundMgr->getSnd("Theme")->playAudio(1);

	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;
	std::vector<cStar*> starList;
	std::vector<cStar*>::iterator starIndex;

   //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();
		
		// Lab code goes here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth,windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (camType == true)
		{
			glLoadMatrixf((GLfloat*)&theCamera.getTheViewMatrix());
			theCamera.update();
		}
		else
		{
			//Position third person camera behind player
			glm::vec3 newCamPos;
			newCamPos.x = thePlayer.getPosition().x;
			newCamPos.y = -70.0f;
			newCamPos.z = 5.0f;
			thirdPersCamera.setTheCameraPos(newCamPos);

			//calc where third person camera should be looking
			glm::vec3 newCamLookPos;
			newCamLookPos.x = thePlayer.getPosition().x;
			newCamLookPos.y = 0;
			newCamLookPos.z = 0;
			thirdPersCamera.setTheCameraLookAt(newCamLookPos);
			//update third person camera
			glLoadMatrixf((GLfloat*)&thirdPersCamera.getTheViewMatrix());
			thirdPersCamera.update();
		}


		sunMaterial.useMaterial();
		sunLight.lightOn();
		lfLight.lightOn();
		rfLight.lightOn();
		cbLight.lightOn();

		if (gameState == 0)
		{
		
			/*
			==============================================================
			| Enemy Update and Render
			==============================================================
			*/

			for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
			{
				if ((*enemyIterator)->isActive())
				{
					if ((*enemyIterator)->getPosition().y <= -25.0f)
					{
						// if enemy has gone past player "delete"
						(*enemyIterator)->setIsActive(false);
						// increment enemies gone past screen
						enemiesPast++;
					}
					(*enemyIterator)->update(elapsedTime);
					spaceShipMdl.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
				}
			}


			/*
			==============================================================
			| Stars Update and Render
			==============================================================
			*/

			for (vector<cStar*>::iterator starsIterator = theStars.begin(); starsIterator != theStars.end(); ++starsIterator)
			{
				if ((*starsIterator)->isActive())
				{
					star.renderMdl((*starsIterator)->getPosition(), (*starsIterator)->getRotation(), (*starsIterator)->getScale());
					(*starsIterator)->update(elapsedTime);
				}
			}

			/*
			==============================================================
			| Explosions Update and Render
			==============================================================
			*/

			for (vector<cExploison*>::iterator expIterator = theExplosions.begin(); expIterator != theExplosions.end(); ++expIterator)
			{
				if ((*expIterator)->isActive())
				{
					explosion.renderMdl((*expIterator)->getPosition(), (*expIterator)->getRotation(), (*expIterator)->getScale());
					(*expIterator)->update(elapsedTime);
				}
			}


			/*
			==============================================================
			| Player Update and Render
			==============================================================
			*/
			tardisMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
			thePlayer.update(elapsedTime);


			/*
			==============================================================
			| Laser Update and Render
			==============================================================
			*/
			for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
			{
				if ((*laserIterartor)->isActive())
				{
					theLaser.renderMdl((*laserIterartor)->getPosition(), (*laserIterartor)->getRotation(), (*laserIterartor)->getScale());
					(*laserIterartor)->update(elapsedTime);
				}
			}




			if (gameState == 0) //only do if game state is zero (play)
			{
				/*
				==============================================================
				| Enemy Spawning
				==============================================================
				*/
				if (curSpawnTime > spawnTime)
				{
					//generate number of enemies to spawn
					int numSpawns = (((float)rand()) / (float)RAND_MAX) * (5.0F);
					for (int i = 0; i < numSpawns; i++)
					{
						//Spawn
						theEnemy.push_back(new cEnemy);
						int enemyNumber = theEnemy.size() - 1;
						theEnemy[enemyNumber]->initialise();
						theEnemy[enemyNumber]->setMdlDimensions(spaceShipMdl.getModelDimensions());
						theEnemy[enemyNumber]->setScale(glm::vec3(5, 5, 5));
					}
					curSpawnTime = 0.0f;
				}
				else
				{
					curSpawnTime += 0.01;
				}
				/*
				==============================================================
				| Star Spawning
				==============================================================
				*/
				if (curStarSpawnTime > starSpawnTime)
				{
					//generate number of enemies to spawn
					int numSpawns = (((float)rand()) / (float)RAND_MAX) * (10.0F);
					for (int i = 0; i < numSpawns; i++)
					{
						//Spawn Star
						theStars.push_back(new cStar);
						int starNumber = theStars.size() - 1;
						float randX = rand() % (int)((50 - -50) + 1) + -50;
						float randY = rand() % (int)((25 - 20) + 1) + 20;
						float randZ = rand() % (int)((30 - -30) + 1) + -30;
						glm::vec3 starPos = glm::vec3(randX, randY, randZ);
						theStars[starNumber]->initialise(starPos, 180.0f, glm::vec3(1, 1, 1), glm::vec3(0, -1, 0), 5.0f, true);
					}
					curStarSpawnTime = 0.0f;
				}
				else
				{
					curStarSpawnTime += 0.01;
				}
			
			}

			/*
			==============================================================
			| Player Wrapping
			==============================================================
			*/
			if (thePlayer.getPosition().x < -32)
			{
				glm::vec3 playPos;
				playPos.x = thePlayer.getPosition().x + 64;
				playPos.y = thePlayer.getPosition().y;
				playPos.z = thePlayer.getPosition().z;
				thePlayer.setPosition(playPos);
			}
			else if (thePlayer.getPosition().x > 32)
			{
				glm::vec3 playPos;
				playPos.x = thePlayer.getPosition().x - 64;
				playPos.y = thePlayer.getPosition().y;
				playPos.z = thePlayer.getPosition().z;
				thePlayer.setPosition(playPos);
			}
			/*
			==============================================================
			| Game Over
			==============================================================
			*/
			//Lose
			if (enemiesPast >= 5)
			{
				GameOver(1);
			}
			//Win
			if (tCount >= 60)
			{
				GameOver(2);
			}
					/*
		==============================================================
		| CAMERA SWITCHING
		==============================================================
		*/
			if (camType == true && theInputMgr->isKeyDown(0x43))
			{
				camType = false;
			}
			else if (camType == false && theInputMgr->isKeyDown(0x43))
			{
				camType = true;
			}
		}

		/*
		==============================================================
		| RESTART
		==============================================================
		*/
		if (theInputMgr->isKeyDown(VK_RETURN) && gameState != 0)
		{
			gameState = 0;
			enemiesPast = 0;
			thePlayer.score = 0;
			tCount = 0;
		}

		/*
		==============================================================
		| SOUND TOGGLE
		==============================================================
		*/
		if (theInputMgr->isKeyDown(0x4D) && music == true)
		{
			music = false;
			theSoundMgr->getSnd("Theme")->stopAudio();
		}
		else if (theInputMgr->isKeyDown(0x4D) && music == false)
		{
			music = true;
			theSoundMgr->getSnd("Theme")->playAudio(1);
		}
		/*
		==============================================================
		| GUI
		==============================================================
		*/
		glPushMatrix();
		theOGLWnd.setOrtho2D(windowWidth, windowHeight);
		string scoreString = "YOU SCORED : " + to_string(thePlayer.score);
		string inGameScoreString = "SCORE : " + to_string(thePlayer.score);
		string timeString = "Time : " + (to_string((int)tCount) + " s");
		string laserChargeString = "Laser Charge : " + (to_string((int)thePlayer.charge));
		string soundOnOff;
		if (music == true)
		{
			soundOnOff = "SOUND : ON";
		}
		else
		{
			soundOnOff = "SOUND : OFF";
		}
		switch (gameState)
		{
		case (0) :
			//GAME GUI
			outputMsg = "Enemies Through : " + to_string(enemiesPast); // convert float to string
			theFontMgr->getFont("ftFont")->printText(soundOnOff.c_str(), FTPoint(10, 35, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("ftFont")->printText(outputMsg.c_str(), FTPoint(750, 35, 0.0f), colour3f(255.0f, 255.0f, 255.0f)); // uses c_str to convert string to LPCSTR
			theFontMgr->getFont("ftFont")->printText(timeString.c_str(), FTPoint(0, 760, 0.0f), colour3f(255.0f, 255.0f, 255.0f)); // uses c_str to convert string to LPCSTR
			theFontMgr->getFont("ftFont")->printText(laserChargeString.c_str(), FTPoint(770, 760, 0.0f), colour3f(255.0f, 255.0f, 255.0f)); // uses c_str to convert string to LPCSTR
			theFontMgr->getFont("ftFont")->printText(inGameScoreString.c_str(), FTPoint(450, 35, 0.0f), colour3f(255.0f, 255.0f, 255.0f)); // uses c_str to convert string to LPCSTR
			break;
		case (1) :
			//GAME OVER GUI
			theFontMgr->getFont("ftFont")->printText("GAME OVER", FTPoint(450, 384, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("ftFont")->printText("PRESS ENTER TO RESTART", FTPoint(370, 484, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			break;
		case (2) :
			//WIN GUI
			theFontMgr->getFont("ftFont")->printText("YOU WIN", FTPoint(450, 300, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("ftFont")->printText(scoreString.c_str(), FTPoint(415, 400, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("ftFont")->printText("PRESS ENTER TO PLAY AGAIN", FTPoint(350, 500, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			break;
		case (3) :
			//MAIN GAME SCREEN GUI
			theFontMgr->getFont("ftFont")->printText("SPACE DEFENCE", FTPoint(425, 200, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("ftFont")->printText("DONT LET MORE THAN FIVE ENEMIES PAST FOR 60 SECONDS", FTPoint(190, 300, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("ftFont")->printText("PRESS ENTER TO START", FTPoint(380, 400, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("ftFont")->printText("C SWITCHES CAMERA, M ENABLES/DISABLES MUSIC", FTPoint(220, 500, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			break;

		}
		glPopMatrix();
		pgmWNDMgr->swapBuffers();
		tCount += elapsedTime;
		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	}
	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}

void GameOver(int newGameState)
{
	//Set Gamestate to paused
	gameState = newGameState;
	//Delete all entities
	for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
	{
		if ((*enemyIterator)->isActive())
		{
			(*enemyIterator)->setIsActive(false);
		}
	}
	for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
	{
		if ((*laserIterartor)->isActive())
		{
			(*laserIterartor)->setIsActive(false);
		}
	}
	
}

