//
//  CinemaCamera.cpp
//  mythreal
//
//  Created by Ian Ewell on 3/30/13.
//
//

#include "CinemaCamera.h"
#include "SceneManager.h"

CSCinemaCamera::CSCinemaCamera()
{
    mActive = true;
    mStarted = false;
    mTimer = 0.0f;
}

CSCinemaCamera::~CSCinemaCamera()
{
    
}

void CSCinemaCamera::setStart(CMVector3 pos, CMVector3 aim)
{
    mStartPos = pos;
    mStartAim = aim;
}

void CSCinemaCamera::setEnd(CMVector3 pos, CMVector3 aim)
{
    mTargetPos = pos;
    mTargetAim = aim;
}

CMVector3 CSCinemaCamera::getCurrentPosition()
{
    return mCurrentPos;
}

void CSCinemaCamera::setDuration(F32 duration)
{
    mDuration = duration>0.01f?duration:0.01f;
}

void CSCinemaCamera::start()
{
    mStarted = true;
    mCurrentPos = mStartPos;
    mCurrentAim = mStartAim;
    mTimer = 0.0f;
}

#define LERP(v0, v1, t) v0+(v1-v0)*t

void CSCinemaCamera::tick(F32 dt)
{
    if (mStarted)
        mTimer += dt;
    if (mTimer >= mDuration)
    {
        mTimer = mDuration;
        mStarted = false;
    }
    // Perform a lerp
    mCurrentPos.setX(LERP(mStartPos.getX(), mTargetPos.getX(), mTimer/mDuration));
    mCurrentPos.setY(LERP(mStartPos.getY(), mTargetPos.getY(), mTimer/mDuration));
    mCurrentPos.setZ(LERP(mStartPos.getZ(), mTargetPos.getZ(), mTimer/mDuration));
    mCurrentAim.setX(LERP(mStartAim.getX(), mTargetAim.getX(), mTimer/mDuration));
    mCurrentAim.setY(LERP(mStartAim.getY(), mTargetAim.getY(), mTimer/mDuration));
    mCurrentAim.setZ(LERP(mStartAim.getZ(), mTargetAim.getZ(), mTimer/mDuration));
    
    // Create the view matrix
    CMMatrix4x4 viewmat;
    viewmat.lookAtMatrix(mCurrentPos, mCurrentAim);
    
    CMMatrix4x4 screenTrans;
    //screenTrans.rotate(D_TO_R(90), 0, 0, 1);
    screenTrans.multiply(viewmat);
    mWorld = screenTrans;
    if (mActive)
        gGlobalSceneManager->setViewMatrix(mWorld);
}

void CSCinemaCamera::getRay(U32 x, U32 y, CMVector3 &pos, CMVector3 &dir)
{
    F64 dx, dy;
    CMMatrix4x4 world;
    
    /*dx = tan(D_TO_R(60.0f*0.5f))*(((768.0-(F64)x)/(768.0/2.0) - 1.0f))/0.75;
    dy = tan(D_TO_R(60.0f*0.5f))*(1.0f-(1024.0-(F64)y)/(1024.0/2.0));
    world = mWorld;
    world.inverse();
    CMVector3 p1(dx*0.2f, dy*0.2f, 0.2f);
    CMVector3 p2(dx*500.0f, dy*500.0f, 500.0f);*/
    // Undo viewport transform
    dx = (-(F64)y/(1024.0)*2.0f + 1.0f);
    dy = (1.0f - (F64)x/(768.0f)*2.0f);
    
    // Build points undoing perspective divide
    CMVector3 p1(dx, dy, -1.0f);
    CMVector3 p2(dx, dy, 1.0f);
    
    // Undo perspective
    world = mWorld;
    //world.inverse();
    CMMatrix4x4 persp = gGlobalSceneManager->getProjectionMatrix();
    persp.multiply(world);
    persp.inverse();
    
    CMVector4 p1t = persp.multiplyVec(CMVector4(p1));
    CMVector4 p2t = persp.multiplyVec(CMVector4(p2));
    p1t.multiplyScalar(1.0/p1t.getW());
    p2t.multiplyScalar(1.0/p2t.getW());
    CMVector4 vec = p2t;
    vec.subtract(p1t);
    
    dir.setX(vec.getX());
    dir.setY(vec.getY());
    dir.setZ(vec.getZ());
    dir.normalize();
    
    pos.setX(p1t.getX());
    pos.setY(p1t.getY());
    pos.setZ(p1t.getZ());
}


void CSCinemaCamera::render()
{
    
}