/*
 *
 * FILE: TestPyramid.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: Implements a test object which moves a pyramid.
 *
 * CREATED ON: 06/03/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

//Includes:
#include <math.h>
#include "TestPyramid.h"

//CSTestPyramid implementation begin:
CSTestPyramid::CSTestPyramid()
{
	mOldPos.setZ(-5.0f);
	mNewPos.setZ(-5.0f);
}

void CSTestPyramid::tick(float dt)
{
	mLin += dt;
	mOldPos = mNewPos;
	mNewPos.setY(sin((F64)mLin));
}