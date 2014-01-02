/*
 *
 * FILE: testm3d.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: Reads and manages a model in the m3d format.
 *
 * CREATED ON: 08/06/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef SCENE_TEST_M3D_H
#define SCENE_TEST_M3D_H

#include <string>
#include "testm3drender.h"
#include "Vector.h"
#include "SceneObject.h"

class CSTestM3D : public CSSceneObject
{
protected:
    CGM3DTestRenderer *mRenderer;
    F32 mTime;
    bool mReflecting;
    
public:
    CSTestM3D();
    ~CSTestM3D();
    
    //void openM3D(std::string file);
    void loadFromResource(const std::string &resource);
    
    void render()
    {
        if (!mVisible)
            return;
        
        mRenderer->setWorldMatrix(mWorld);
        mRenderer->render();
    }
    
    //CMVector3 getPosition();
    
    void setReflecting(bool refl);
    bool isReflecting() const;
    
    virtual void prePassRender();
    
    CGM3DTestRenderer* getRenderer()
    {
        return mRenderer;
    }
    
    void setAnimation(std::string &name);
    
    virtual void tick(F32 dt);
};

#endif