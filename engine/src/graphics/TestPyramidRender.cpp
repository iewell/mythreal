/*
 *
 * FILE: TestPyramidRender.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Graphics
 *
 * PURSPOSE: Implements a test object which renders a pyramid.
 *
 * CREATED ON: 06/03/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

//Includes:
#include "TestPyramidRender.h"

static const GLfloat Vertices[36] = {
	0.0f, 1.0f, 0.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, -1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f,-1.0f, -1.0f,
	-1.0f,-1.0f, -1.0f,
	0.0f, 1.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
};

static const GLubyte Colors[] = {
	255, 255,   0, 255,
	0,   255, 255, 255,
	0,     0,   0,   0,
	255,   0, 255, 255,
};


//CGTestPyramidRender implementation begin:
CGTestPyramidRender::CGTestPyramidRender()
{
	
}

void CGTestPyramidRender::render()
{
	glPushMatrix();
	glTranslatef(mSimObj->mNewPos.getX() * gTimeFromLastTick + mSimObj->mOldPos.getX() * (1 - gTimeFromLastTick),
				 mSimObj->mNewPos.getY() * gTimeFromLastTick + mSimObj->mOldPos.getY() * (1 - gTimeFromLastTick),
				 mSimObj->mNewPos.getZ() * gTimeFromLastTick + mSimObj->mOldPos.getZ() * (1 - gTimeFromLastTick));
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, Colors);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glDisableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_LINES, 0, 12);
	glPopMatrix();
	
}