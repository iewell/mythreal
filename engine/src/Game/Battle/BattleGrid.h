//
//  BattleGrid.h
//  mythreal
//
//  Created by Ian Ewell on 1/30/13.
//
//

#ifndef __mythreal__BattleGrid__
#define __mythreal__BattleGrid__

#include <iostream>
#include <list>
#include "Types.h"
#include "Vector.h"
#include "BattleActor.h"
#include "Reference.h"

class CGBBattleActor;

// Type of terrain a terrain cell can be
class CGBBattleTerrainType
{
protected:
    bool mPassable;
    U32 mMovementCost;
    
public:
    CGBBattleTerrainType();
    virtual ~CGBBattleTerrainType();
};

// Grid cell that stores data on terrain features and a pointer to something that may occupy it
class CGBBattleGridCell
{
protected:
    CGBBattleTerrainType *mTerrain;
    CGBBattleActor *mActor;
    
public:
    CGBBattleGridCell();
    CGBBattleGridCell(CGBBattleTerrainType *terrain);
    virtual ~CGBBattleGridCell();
    
    void setTerrain(CGBBattleTerrainType *type);
    CGBBattleTerrainType* getTerrain();
    
    void setActor(CGBBattleActor *actor);
    CGBBattleActor* getActor();
};

// This is the data structure for containing the gameplay portion of the battle. This keeps
// track of the tactical battle field and all the characters and field elements. In the future,
// this may be organized to utilize a quad-tree structure if the battles become complex enough to
// justify it.
class CGBBattleGrid : public CCReferenceBase
{
protected:
    U32 mSizeX;
    U32 mSizeY;
    
    CGBBattleGridCell *mCells;
    std::list<CGBBattleActor*> mActors;
    
    bool mConstructed;
    
public:
    CGBBattleGrid();
    CGBBattleGrid(U32 width, U32 height);
    virtual ~CGBBattleGrid();
    
    void setSize(U32 width, U32 height);
    U32 getWidth();
    U32 getHeight();
    
    CGBBattleGridCell* getCell(U32 locX, U32 locY);
    std::list<CGBBattleActor*>* getActors();
    
    bool construct();
    void deconstruct();
    
    bool isValid(U32 locX, U32 locY);
    bool isOccupied(U32 locX, U32 locY);
    bool insertActorAtLocation(U32 locX, U32 locY, CGBBattleActor *actor);
    CGBBattleActor* getActorAtLocation(U32 locX, U32 locY);
    
    bool canMove(CGBBattleActor *actor, U32 targetX, U32 targetY);
    bool moveActorToLocation(CGBBattleActor *actor, U32 locX, U32 locY);
    
    void printGrid();
};

typedef CCPointerStrong<CGBBattleGrid> CGBBattleGridRef;

#endif /* defined(__mythreal__BattleGrid__) */