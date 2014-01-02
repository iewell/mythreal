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

#ifndef SCENE_SCENE_OBJECT_H
#define SCENE_SCENE_OBJECT_H

#include "Vector.h"
#include "Matrix.h"
#include "eventReceiver.h"

class CSSceneObject : public CEEventReceiver
{
protected:
    
    CMMatrix4x4 mWorld;
    CMMatrix4x4 mObject;
    CMVector3 mPosition;
    
    F32 mOrientationY;
    
    bool mVisible;
    
    // If this is true, the function prePassRender() will be called before the actual scene is rendered
    // The default framebuffer will not be bound so any drawing must be done into an offscreen framebuffer
    // This is used for shadow maps and reflections
    bool mPreRenderPass;
    
public:
    CSSceneObject();
    void setPosition(CMVector3 &pos);
    CMVector3 getPosition();
    
    void setYOrientation(F32 theta);
    
    void setVisible(bool vis);
    bool isVisible() const;
    
    void setPrePassRender(bool PPR);
    bool isPrePassRender() const;
    
    virtual void tick(F32 dt)=0;
    virtual void prePassRender();
    virtual void render()=0;
};

#endif