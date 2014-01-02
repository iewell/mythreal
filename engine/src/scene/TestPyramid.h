/*
 *
 * FILE: TestPyramid.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: Declares a test object which moves a pyramid.
 *
 * CREATED ON: 06/03/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef SCENE_TESTPYRAMID_H
#define SCENE_TESTPYRAMID_H

//Includes:
#include "Types.h"
#include "Globals.h"
#include "Vector.h"

//Classes:

// CSTestPyramid:
// Simulates a pyramid following a sine wave.
class CSTestPyramid
{
	friend class CGTestPyramidRender;
private:
	CMVector3 mOldPos;
	CMVector3 mNewPos;
	float mLin;
	
public:
	CSTestPyramid();
	void tick(float dt);
};

#endif