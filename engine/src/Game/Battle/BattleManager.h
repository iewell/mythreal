//
//  BattleManager.h
//  mythreal
//
//  Created by Ian Ewell on 3/1/13.
//
//

#ifndef __mythreal__BattleManager__
#define __mythreal__BattleManager__

#include <iostream>
#include <list>
#include "BattleGrid.h"
#include "BattleActor.h"

typedef enum
{
    DIR_NORTH,
    DIR_NORTHEAST,
    DIR_EAST,
    DIR_SOUTHEAST,
    DIR_SOUTH,
    DIR_SOUTHWEST,
    DIR_WEST,
    DIR_NORTHWEST,
    DIR_INVALID,
} EDirection;

class CGBBattleManager
{
protected:
    CGBBattleGrid *mBattleGrid;
    CGBBattleActor *mActiveActor;
    
    std::list<CGBBattleActor*>* mTeamActorLists;
    U32 mActiveActorIndex;
    
    U32 mActiveTeam;
    U32 mTeamCount;
    
public:
    CGBBattleManager(U32 teamCount);
    virtual ~CGBBattleManager();
    
    void createTestFormation();
    
    CGBBattleGrid* getGrid();
    
    void nextActor();
    void setActiveActor(CGBBattleActor *actor);
    CGBBattleActor* getActiveActor();
    void checkCurrentActor();
    void nextTeam();
    void setActiveTeam(U32 team);
    U32 getActiveTeam();
    
    void updateActorLists();
    
    // Actor controls
    bool moveActorToLoc(U32 x, U32 y);
    void getLocationFromDirection(EDirection dir, U32 *loc);
    bool moveActorInDirection(EDirection dir);
    bool attackInDirection(EDirection dir, CGBBattleAttackDefinition *attack);
};

#endif /* defined(__mythreal__BattleManager__) */
