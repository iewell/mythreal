/*
 *
 * FILE: SceneObject.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: This is a base object that all objects in the scene are derived from
 *
 * CREATED ON: 5/6/11
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */


#include "SceneObject.h"

CSSceneObject::CSSceneObject()
{
    mWorld.setIdentity();
    mObject.setIdentity();
    
    mOrientationY = 0.0f;
    mVisible = true;
}

void CSSceneObject::setPosition(CMVector3 &pos)
{
    mPosition = pos;
    mWorld.setIdentity();
    mWorld.rotate(mOrientationY, 0.0f, 1.0f, 0.0f);
    mWorld.setColumn(3, CMVector4(pos));
}

CMVector3 CSSceneObject::getPosition()
{
    CMVector4 pos4 = mWorld.getColumn(3);
    return CMVector3(pos4.getX(),
                     pos4.getY(),
                     pos4.getZ());
}

void CSSceneObject::setYOrientation(F32 theta)
{
    mOrientationY = theta;
    mWorld.setIdentity();
    mWorld.rotate(theta, 0.0f, 1.0f, 0.0f);
    mWorld.setColumn(3, CMVector4(mPosition));
}

void CSSceneObject::setVisible(bool vis)
{
    mVisible = vis;
}

bool CSSceneObject::isVisible() const
{
    return mVisible;
}

void CSSceneObject::setPrePassRender(bool PPR)
{
    mPreRenderPass = PPR;
}

bool CSSceneObject::isPrePassRender() const
{
    return mPreRenderPass;
}

void CSSceneObject::prePassRender()
{
    
}