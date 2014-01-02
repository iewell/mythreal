/*
 *
 * FILE: testCamera.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: Implement a camera prototype that tracks the player prototype.
 *
 * CREATED ON: 10/11/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef SCENE_TESTCAMERA_H
#define SCENE_TESTCAMERA_H

#include "Vector.h"
#include "testplayer.h"
#include "SceneObject.h"

class CSTestCamera : public CSSceneObject
{
private:
    // The position of the camera
    CMVector3 mOldPosition;
    CMVector3 mNewPosition;
    
    F32 mOrbitSpeed;
    F32 mOrbitAngle;
    
    // The amount to offset the camera from the player
    CMVector3 mOffset;
    
    // The target player to track
    CSSceneObject *mTarget;
    
    CMMatrix4x4 mView;
    
    // Some debug rendering stuff
    CMVector4 point1;
    CMVector4 point2;
    
public:
    CSTestCamera();
    
    void setOffset(CMVector3 offset);
    void setTargetPlayer(CSSceneObject *player);
    void setOrbitSpeed(F32 orbitSpeed);
    void setOrbitAngle(F32 orbitAngle);
    
    virtual void tick(F32 dt);
    virtual void render();
    void debugRender();
    
    void transformScene();
    
    void getRay(U32 x, U32 y, CMVector3 &pos, CMVector3 &dir);
};

#endif