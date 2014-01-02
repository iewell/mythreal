/*
 *
 * FILE: MainLoop.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Core
 *
 * PURSPOSE: Defines the main loop and thread entry function.
 *
 * CREATED ON: 06/02/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

//Includes:
#include <stdio.h>
#include "MainLoop.h"
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "TestPyramid.h"
#include "testplayer.h"
#include "Vector.h"
#include "ResourceManager.h"
#include "testCamera.h"
#include "CinemaCamera.h"
#include "CineText.h"
#include "ColorPlane.h"
#include "SimpleCineScript.h"
#include "PlayerMarble.h"
#include "AccelerationStream.h"
#include "SceneManager.h"
#include "EventManager.h"

//Globals:
bool gSimulationRunning = false;
float gTimeFromLastTick = 0;
CSTestPyramid *gPyramid = new CSTestPyramid();
extern CSTestPlayer gShip;
extern CSTestCamera gCamera;
extern CUICineText text;
extern CSColorPlane gColorPlane;
CSSimpleCineScript gScript;
extern CSPlayerMarble *gPlayerMarble;
extern CSPhysicsWorld2D *gPhysicsWorld;

//Global Functions:

//CCMainLoop implementation begin:
void CCMainLoop::Initialize()
{
	//TODO: Add some stuff.
	mTimePrev = mTimeCurrent = _getMilliseconds();
	mGameClock = _getMilliseconds();
	mDT = _getMilliseconds() - mGameClock;
	
	// Initialize the resource manager.
	CRResourceManager::Init();
    
    //gPhysicsWorld = new CSPhysicsWorld2D(CMVector2(0, -9.8));
    //gPhysicsWorld->loadCollisionData("testlevel01.mcol");
    
    //CMVector3 force(0, 0, 0);
    //gShip.setForce(force);
    
    gGlobalEventManager = new CEEventManager;
    //gGlobalSceneManager->getPhysicsWorld()->loadCollisionData("castleCol.mcol");
    
    //gCamera.setOffset(offset);
    //gCamera.setTargetPlayer(gPlayerMarble);
    //text.setText("");
    //gColorPlane.setTargetAlpha(1.0f);
    //gShip.setTargetAlpha(1.0f);
}

void CCMainLoop::populateScene()
{
    // Create the camera
    CSCinemaCamera *camera = new CSCinemaCamera();
    //CMVector3 offset(0, 0, 80);
    //camera->setOffset(offset);
    gGlobalSceneManager->insertObject(camera);
    
    CRResourceManager::Get()->loadResourceFile("bowserscene2");
    
    // Load our level geometry
    //CSTestM3D *level = new CSTestM3D();
    //level->loadFromResource("castle");
    //level->openM3D("castleModel01.m3d");
    //gGlobalSceneManager->insertObject(level);
    //gGlobalSceneManager->loadSceneFromLevelLayout("bowserScene");
    
    mBTester = new CGBBattleTester(gGlobalSceneManager);
    mBTester->setupScene();
    mBTester->setCamera(camera);
    
    //CRResourceManager::Get()->loadResourceFile("marble");
    
    // Create our player
    CSPlayerMarble *player = new CSPlayerMarble(gGlobalSceneManager->getPhysicsWorld(), 2.0f);
    //player->setReflecting(true);
    //player->openM3D("marble.m3d");
    //player->loadFromResource("marble");
    gGlobalSceneManager->insertObject(player);
    
    // Attacth the player to the camera
    //camera->setTargetPlayer(player);
    
    mScript = new CSTestBattleStartScript(camera);
}

int CCMainLoop::Frame()
{
	// Don't let the pause button interupt while in the middle of timing calculations.
		
	// Update elapsed time.
	mTimeCurrent = _getMilliseconds();
	mTimePrev = mTimeCurrent;
		
	// Calculate how many ticks we need to process.
	mDT = _getMilliseconds() - mGameClock;
		
	// Process the ticks.
	while (mDT >= TICK_MS)
	{
		mDT -= TICK_MS;
		mGameClock += TICK_MS;
			
		// Insert per-tick updating here.
		//gPyramid->tick(1.0f/TICK_MS);
        //gShip.integrate(TICK_MS/1000.0f);
        //gCamera.tick(TICK_MS/1000.0f);
        //text.tick(TICK_MS/1000.0f);
        //gColorPlane.tick(TICK_MS/1000.0f);
        
        // HAXX: set the new gravity RIGHT in the main loop. This code is already messy though so what is a little more :)
#if !(TARGET_IPHONE_SIMULATOR)
        CMVector2 gravity(CIAccelerationStream::getSingleton()->getY(), -CIAccelerationStream::getSingleton()->getX());
#else
        CMVector2 gravity(0.0f, -1.0f);
#endif
        gravity.normalize();
        gravity.multiplyScalar(/*9.8*/0);
        gGlobalSceneManager->getPhysicsWorld()->setGravity(gravity);
        
        gGlobalSceneManager->tick(TICK_MS/1000.0f);
        mScript->tick(TICK_MS/1000.0f);
        gGlobalEventManager->tick(TICK_MS/1000.0f);
        //gPlayerMarble->tick(TICK_MS/1000.0f);
        //gScript.tick(TICK_MS/1000.0f);
	}
	if (mDT != 0)
		gTimeFromLastTick = (float)mDT/TICK_MS;
	else
		gTimeFromLastTick = 0;
		
	return 0;
}