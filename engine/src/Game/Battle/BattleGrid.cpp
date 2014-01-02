//
//  BattleGrid.cpp
//  mythreal
//
//  Created by Ian Ewell on 1/30/13.
//
//

#include "BattleGrid.h"
#include <iostream>

CGBBattleTerrainType::CGBBattleTerrainType()
{
    mPassable = true;
    mMovementCost = 1;
}

CGBBattleTerrainType::~CGBBattleTerrainType()
{
    
}

/////////////////////////////////////////////////////////

CGBBattleGridCell::CGBBattleGridCell()
{
    mTerrain = NULL;
    mActor = NULL;
}

CGBBattleGridCell::CGBBattleGridCell(CGBBattleTerrainType *terrain)
{
    mTerrain = terrain;
}

CGBBattleGridCell::~CGBBattleGridCell()
{
    
}

void CGBBattleGridCell::setTerrain(CGBBattleTerrainType *type)
{
    mTerrain = type;
}

CGBBattleTerrainType* CGBBattleGridCell::getTerrain()
{
    return mTerrain;
}

void CGBBattleGridCell::setActor(CGBBattleActor *actor)
{
    mActor = actor;
}

CGBBattleActor* CGBBattleGridCell::getActor()
{
    return mActor;
}

/////////////////////////////////////////////////////////

CGBBattleGrid::CGBBattleGrid()
{
    mSizeX = 1;
    mSizeY = 1;
    
    mCells = NULL;
    
    mConstructed = false;
}

CGBBattleGrid::CGBBattleGrid(U32 width, U32 height)
{
    mSizeX = width;
    mSizeY = height;
    
    mCells = NULL;
    
    mConstructed = false;
}

CGBBattleGrid::~CGBBattleGrid()
{
    if (mConstructed)
        deconstruct();
}

void CGBBattleGrid::setSize(U32 width, U32 height)
{
    if (!mConstructed)
    {
        mSizeX = width;
        mSizeY = height;
    }
}

U32 CGBBattleGrid::getWidth()
{
    return mSizeX;
}

U32 CGBBattleGrid::getHeight()
{
    return mSizeY;
}

CGBBattleGridCell* CGBBattleGrid::getCell(U32 locX, U32 locY)
{
    return &mCells[locY*mSizeY+locX];
}

std::list<CGBBattleActor*>* CGBBattleGrid::getActors()
{
    return &mActors;
}

bool CGBBattleGrid::construct()
{
    if (mConstructed)
        return true;
    
    CGBBattleTerrainType *defaultTerrain = new CGBBattleTerrainType();
    mCells = new CGBBattleGridCell[mSizeX*mSizeY];
    for (int i = 0; i < mSizeX*mSizeY; i++)
        mCells[i].setTerrain(defaultTerrain);
    
    mConstructed = true;
    return true;
}

void CGBBattleGrid::deconstruct()
{
    delete[] mCells;
    mConstructed = false;
    mActors.clear();
}

bool CGBBattleGrid::isValid(U32 locX, U32 locY)
{
    if (locX >= mSizeX || locY >= mSizeY)
        return false;
    return true;
}

bool CGBBattleGrid::isOccupied(U32 locX, U32 locY)
{
    if (!isValid(locX, locY))
        return false;
    if (mCells[locY*mSizeY+locX].getActor())
        return true;
    return false;
}

bool CGBBattleGrid::insertActorAtLocation(U32 locX, U32 locY, CGBBattleActor *actor)
{
    if (locX >= mSizeX || locY >= mSizeY)
        return false;
    if (mCells[locY*mSizeY+locX].getActor())
        return false;
    mCells[locY*mSizeY+locX].setActor(actor);
    mActors.push_back(actor);
    return true;
}

CGBBattleActor* CGBBattleGrid::getActorAtLocation(U32 locX, U32 locY)
{
    if (!isOccupied(locX, locY))
        return NULL;
    return mCells[locY*mSizeY+locX].getActor();
}

bool CGBBattleGrid::canMove(CGBBattleActor *actor, U32 targetX, U32 targetY)
{
    if (!mConstructed)
        return false;
    
    // More elaborate check later. Now just see if target cell is occupied
    if (isOccupied(targetX, targetY))
        return false;
    
    if (targetX >= mSizeX || targetY >= mSizeY)
        return false;
    return true;
}

bool CGBBattleGrid::moveActorToLocation(CGBBattleActor *actor, U32 locX, U32 locY)
{
    if (!mConstructed)
        return false;
    
    if (!canMove(actor, locX, locY))
        return false;
    U32 actorX = actor->getLocationX();
    U32 actorY = actor->getLocationY();
    if (!isValid(actorX, actorY))
        return false;
    mCells[actorY*mSizeY+actorX].setActor(NULL);
    mCells[locY*mSizeY+locX].setActor(actor);
    actor->setLocation(locX, locY);
    return true;
}

void CGBBattleGrid::printGrid()
{
    if (!mConstructed)
        return;
    
    for (int i = mSizeY-1; i >= 0; i--)
    {
        for (int j = 0; j < mSizeX; j++)
        {
            if (mCells[i*mSizeY+j].getActor() == NULL)
                std::cout << "X ";
            else
                std::cout << "O ";
        }
        std::cout << "\n";
    }
}