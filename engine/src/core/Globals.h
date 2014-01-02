/*
 *
 * FILE: Globals.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Core
 *
 * PURSPOSE: Declares the main global variables and mutexes.
 *
 * CREATED ON: 06/01/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef CORE_GLOBALS_H
#define CORE_GLOBALS_H

//Includes:
#include <pthread.h>
#include "Types.h"

//Global Variables:

// Set to false if the game is paused.
extern bool gSimulationRunning;

// Time elapsed since the last game tick. Used for interpolation.
extern float gTimeFromLastTick;

#endif