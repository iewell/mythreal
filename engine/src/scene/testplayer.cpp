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
 * PURSPOSE: Implement a prototype player ship with a 3D model.
 *
 * CREATED ON: 10/04/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#include "testplayer.h"
#include "AccelerationStream.h"
#include "testCamera.h"
#include "Plane.h"

extern bool gTouched;
extern int gTouchX;
extern int gTouchY;

extern CSTestCamera gCamera;

CSTestPlayer::CSTestPlayer()
{
    mMass = 20;
    mVelocity.setX(0.0f);
    mPosition.setX(0);
    
    mCurrentAlpha = 1;
    mTargetAlpha = 1;
    
    mControlAllowed = false;
}

SDerivative CSTestPlayer::derive(SState &state, float dt, SDerivative &previousDerivative)
{
    // Advanve time by integrating the previous derivative into our state
    SState newState;
    
    // Calculate our new position
    CMVector3 newPos = previousDerivative.dp;
    newPos.multiplyScalar(dt);
    newPos.add(state.pos);
    newState.pos = newPos;
    
    // Calculate our new velocity
    CMVector3 newVel = previousDerivative.dv;
    newVel.multiplyScalar(dt);
    newVel.add(state.vel);
    newState.vel = newVel;
    
    // Calculate the derivative of our new state
    SDerivative der;
    
    // The derivative of the position is velocity
    der.dp = newState.vel;
    
    // Manually calculate acceleration from our state
    der.dv = calculateAcceleration(newState, dt);
    return der;
}

CMVector3 CSTestPlayer::calculateAcceleration(SState &state, float dt)
{
    // Our base acceleration will be the force applied divided by the mass
    // TODO: add aerodynamic drag
    
    // Update accelerometer state
    //CMVector3 acceleration(0, -CIAccelerationStream::getSingleton()->getY()*0.1, 0);
    CMVector3 ret = mForce;
    ret.multiplyScalar(1/mMass);
    return ret;
}

void CSTestPlayer::enableControl(bool control)
{
    mControlAllowed = control;
}

void CSTestPlayer::integrate(float dt)
{
    // Store our previous position for interpolation purposes
    mOldPosition = mPosition;
    
    // Update our y velocity from accelerometer
    //mVelocity.setY(-CIAccelerationStream::getSingleton()->getY()*15);
    
    // Assemble our current state
    SState state;
    state.pos = mPosition;
    state.vel = mVelocity;
    
    // We need four derivatives at different points in time
    
    // We start by deriving our current state
    SDerivative initial;
    SDerivative a = derive(state, dt, initial);
    
    // Derive halfway to our target time using the present as a base
    SDerivative b = derive(state, dt*0.5, a);
    
    // Repeat but with b as a base
    SDerivative c = derive(state, dt*0.5, b);
    
    // Finally, derive at our target time
    SDerivative d = derive(state, dt, c);
    
    // Integrate our position using a weighted sum for error control
    // pos = dt(1/6 (a + 2(b + c) + d))
    CMVector3 pos = b.dp;
    pos.add(c.dp);
    pos.multiplyScalar(2.0f);
    pos.add(a.dp);
    pos.add(d.dp);
    pos.multiplyScalar(1.0f/6.0f);
    pos.multiplyScalar(dt);
    mPosition.add(pos);
    
    // Integrate our velocity using a weighted sum for error control
    CMVector3 vel = b.dv;
    vel.add(c.dv);
    vel.multiplyScalar(2.0f);
    vel.add(a.dv);
    vel.add(d.dv);
    vel.multiplyScalar(1.0f/6.0f);
    vel.multiplyScalar(dt);
    mVelocity.add(vel);
    
    // Update the matrix
    mWorld.setIdentity();
    mWorld.setColumn(3, CMVector4(mPosition));
    
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
    
    mRenderer->setAlpha(mCurrentAlpha);
    
    if (mControlAllowed)
    {
        if (gTouched)
        {
            CMVector3 pos;
            CMVector3 vec;
            gCamera.getRay(gTouchX, gTouchY, pos, vec);
            
            CMVector3 intercect;
            CMPlane plane(CMVector3(0, 1, 0), CMVector3(0, -2.5, 0));
            plane.intercectRay(pos, vec, &intercect);
            mIntercect = intercect;
            CMVector3 diff = intercect;
            diff.subtract(mPosition);
            diff.normalize();
            diff.multiplyScalar(15);
            diff.setY(0);
            mVelocity = diff;
        }
        else
        {
            mVelocity = CMVector3(0,0,0);
        }
    }
}

void CSTestPlayer::setForce(CMVector3 &force)
{
    mForce = force;
}

void CSTestPlayer::setTargetAlpha(F32 alpha)
{
    mTargetAlpha = alpha;
}

F32 CSTestPlayer::getTargetAlpha()
{
    return mTargetAlpha;
}