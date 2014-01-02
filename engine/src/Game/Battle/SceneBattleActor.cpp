//
//  SceneBattleActor.cpp
//  mythreal
//
//  Created by Ian Ewell on 3/28/13.
//
//

#include "SceneBattleActor.h"

CGBSceneBattleActor::CGBSceneBattleActor()
{
    
}

CGBSceneBattleActor::~CGBSceneBattleActor()
{
    
}

bool CGBSceneBattleActor::insertIntoGrid(CGBBattleGrid *grid, U32 locX, U32 locY)
{
    if (!CGBBattleActor::insertIntoGrid(grid, locX, locY))
        return false;
    F32 scale = mSceneGrid->getScale();
    mCurrentPosition.setX(scale*locX+0.5*scale);
    mCurrentPosition.setZ(scale*locY+0.5*scale);
    mCurrentPosition.add(mSceneGrid->getPosition());
    mTargetPosition = mCurrentPosition;
    setPosition(mCurrentPosition);
    return true;
}

void CGBSceneBattleActor::onLocationChange()
{
    F32 scale = mSceneGrid->getScale();
    mTargetPosition.setX(scale*mLocationX+0.5*scale);
    mTargetPosition.setZ(scale*mLocationY+0.5*scale);
    mTargetPosition.add(mSceneGrid->getPosition());
    
    mVelocity = mTargetPosition - mCurrentPosition;
    mMoveTimer = mVelocity.length()/20.0f;
    mVelocity.normalize();
    mVelocity.multiplyScalar(20.0f);
    
    F32 orientation = atan2f(mVelocity.getX(), mVelocity.getY());
    setYOrientation(orientation);
    //mTargetPosition = mCurrentPosition;
    //setPosition(mCurrentPosition);
}

void CGBSceneBattleActor::setSceneGrid(CGBSceneBattleGrid *grid)
{
    mSceneGrid = grid;
}

void CGBSceneBattleActor::tick(F32 dt)
{
    CSTestM3D::tick(dt);
    if (mDead)
        mVisible = false;
    mMoveTimer -= dt;
    if (mMoveTimer >= 0.0f)
    {
        CMVector3 dx = mVelocity;
        dx.multiplyScalar(dt);
        mCurrentPosition += dx;
        setPosition(mCurrentPosition);
    }
}