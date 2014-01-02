/*
 *
 * FILE: ColorPlane.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: A plane that is colored :D
 *
 * CREATED ON: 5/14/11
 *
 * COPYRIGHT: (C) Copyright 2011 Mythreal Studios. All rights reserved.
 *
 */

#include "ColorPlane.h"
#include <OpenGLES/ES1/gl.h>

const F32 vertices[] = 
{
    200, 0, 200,
    -200, 0, 200,
    -200, 0, -200,
    200, 0, -200,
};

GLfloat colors[] = 
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    1, 1, 0, 0,
};

const GLubyte indices[] = 
{
    0, 1, 2,
    2, 3, 0,
};

CSColorPlane::CSColorPlane()
{
    mCurrentAlpha = 1;
    mTargetAlpha = 1;
}

void CSColorPlane::render()
{
    glPushMatrix();
    mWorld.multMatrixGL();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    glDisable(GL_BLEND);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glPopMatrix();
}

void CSColorPlane::tick(F32 dt)
{
    if (mCurrentAlpha < mTargetAlpha)
    {
        mCurrentAlpha += dt/10;
        if (mCurrentAlpha > mTargetAlpha)
            mCurrentAlpha = mTargetAlpha;
    }
    else if (mCurrentAlpha > mTargetAlpha)
    {
        mCurrentAlpha -= dt/10;
        if (mCurrentAlpha < mTargetAlpha)
            mCurrentAlpha = mTargetAlpha;
    }
    
    colors[3] = mCurrentAlpha;
    colors[7] = mCurrentAlpha;
    colors[11] = mCurrentAlpha;
    colors[15] = mCurrentAlpha;
}