//
//  SceneBattleActor.h
//  mythreal
//
//  Created by Ian Ewell on 3/28/13.
//
//

#ifndef __mythreal__SceneBattleActor__
#define __mythreal__SceneBattleActor__

#include <iostream>
#include "BattleActor.h"
#include "testm3d.h"
#include "SceneBattleGrid.h"

class CGBSceneBattleActor : public CGBBattleActor, public CSTestM3D
{
protected:
    CMVector3 mTargetPosition;
    CMVector3 mCurrentPosition;
    CMVector3 mVelocity;
    F32 mMoveTimer;
    CGBSceneBattleGrid *mSceneGrid;
    
public:
    
    CGBSceneBattleActor();
    virtual ~CGBSceneBattleActor();
    void setSceneGrid(CGBSceneBattleGrid *grid);
    bool insertIntoGrid(CGBBattleGrid *grid, U32 locX, U32 locY);
    virtual void onLocationChange();
    virtual void tick(F32 dt);
};

#endif /* defined(__mythreal__SceneBattleActor__) */
