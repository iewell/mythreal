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
 * PURSPOSE: Declares the main loop and thread entry function.
 *
 * CREATED ON: 06/02/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef CORE_MAINLOOP_H
#define CORE_MAINLOOP_H

//Includes:
#include <pthread.h>
#include <sys/time.h>
#include "Types.h"
#include "Globals.h"
#include "BattleTestGame.h"
#include "TestBattleStartScript.h"

//Global Functions:

// _LoopThreadEntry():
// This is where we break free of the Obj-C and Cocoa control and can implement our
// own Main Loop. This runs in a separate thread and does not actually render anything.
// Rendering is in the main thread in order to sync with the refresh rate.
void *_LoopThreadEntry(void *id);

//Classes:
class CCMainLoop
{
private:
	//DO NOT CHANGE THIS VALUE
	//This value tells the engine when to update the simulation.
	#define TICK_MS 33.333
	
    CGBBattleTester *mBTester;
    CSTestBattleStartScript *mScript;
    
	//Timing variables
	unsigned int mTimePrev, mTimeCurrent;
	unsigned int mGameClock;
	unsigned int mDT;
	
	//Get time in milliseconds.
	unsigned int _getMilliseconds() const
	{
		timeval time;
		gettimeofday(&time, NULL);
		return (time.tv_sec * 1000) + (time.tv_usec / 1000);
	}
	
public:
	// Initialize():
	// Initializes the game engine components.
	void Initialize();
    
    // Hack to load shaders in an OpenGL Context. Populates the scene
    void populateScene();
	
	// EnterLoop():
	// Puts the simulation thread in an infinite loop which pretty
	// much does everything but rendering.
	int Frame();
	
	unsigned long getDT()
	{
		return mDT;
	}
	
	
};

#endif