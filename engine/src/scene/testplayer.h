/*
 *
 * FILE: testm3d.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: Implement a prototype player ship with a 3D model.
 *
 * CREATED ON: 10/04/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef SCENE_TEST_PLAYER_H
#define SCENE_TEST_PLAYER_H

// Includes:
#include "testm3d.h"

// Structures:
typedef struct
{
    CMVector3 pos; // The position state
    CMVector3 vel; // The velocity state
} SState;

typedef struct
{
    CMVector3 dp; // Velocity
    CMVector3 dv; // Acceleration
} SDerivative;

// Class definition:
class CSTestPlayer : public CSTestM3D
{
    
private:
    CMVector3 mOldPosition;
    
    // Some dynamics stuff
    float mMass;
    CMVector3 mVelocity;
    CMVector3 mForce;
    
    CMVector3 mIntercect;
    
    F32 mCurrentAlpha;
    F32 mTargetAlpha;
    
    bool mControlAllowed;
    
    // Private physics functions
    SDerivative derive(SState &state, float dt, SDerivative &previousDerivative);
    CMVector3 calculateAcceleration(SState &state, float dt);
    
public:
    CSTestPlayer();
    
    void integrate(float dt);
    
    // Accessor methods
    void setForce(CMVector3 &force);
    void setMass(float mass);
    
    void enableControl(bool control);
    
    void render()
    {
        mRenderer->setWorldMatrix(mWorld);
        mRenderer->render();
        
        //CMMatrix4x4 mat;
        //mat.setColumn(3, CMVector3(mIntercect));
        //mRenderer->setWorldMatrix(mat);
        //mRenderer->render();
    }
    
    void setTargetAlpha(F32 alpha);
    F32  getTargetAlpha();
};

#endif