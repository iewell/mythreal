/*
 *
 * FILE: ColorPlane.h
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

#include "SceneObject.h"
#include "Plane.h"

#ifndef SCENE_COLOR_PLANE_H
#define SCENE_COLOR_PLANE_H

class CSColorPlane : public CSSceneObject
{
protected:
    CMPlane mPlane;
    F32 mCurrentAlpha;
    F32 mTargetAlpha;
    
public:
    
    CSColorPlane();
    
    void setTargetAlpha(F32 alpha)
    {
        mTargetAlpha = alpha;
    }
    
    void rayCollide(const CMVector3 &vec, const CMVector3 &point);
    void render();
    void tick(F32 dt);
};

#endif