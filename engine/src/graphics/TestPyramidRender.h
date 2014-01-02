/*
 *
 * FILE: TestPyramidRender.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Graphics
 *
 * PURSPOSE: Declares a test object which renders a pyramid.
 *
 * CREATED ON: 06/03/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef GRAPHICS_TEST_PYRAMID_RENDER_H
#define GRAPHICS_TEST_PYRAMID_RENDER_H

//Includes:
#include <OpenGLES/ES1/gl.h>
#include "Globals.h"
#include "Types.h"
#include "TestPyramid.h"

//Classes:

class CGTestPyramidRender
{
private:
	CSTestPyramid *mSimObj;
	
public:
	CGTestPyramidRender();
	
	void setSimObj(CSTestPyramid *obj)
	{
		mSimObj = obj;
	}
	
	void render();
};

#endif