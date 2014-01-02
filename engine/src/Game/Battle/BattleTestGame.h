//
//  BattleTestGame.h
//  mythreal
//
//  Created by Ian Ewell on 3/29/13.
//
//

#ifndef __mythreal__BattleTestGame__
#define __mythreal__BattleTestGame__

#include <iostream>
#include "SceneBattleActor.h"
#include "SceneBattleGrid.h"
#include "BattleManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "TouchEvent.h"
#include "CinemaCamera.h"

class _BattleUIController;

typedef enum TouchMode
{
    TM_SELECT,
    TM_MOVE,
    TM_ATTACK,
} ETouchMode;

class CGBBattleTester : public CEEventReceiver
{
protected:
    CSSceneManager *mSceneManager;
    CGBSceneBattleGrid *mSceneGrid;
    CGBBattleGridRef mBattleGrid;
    CGBBattleManager *mBattleManager;
    
    CGBSceneBattleActor *mSelection;
    CGBBattleAttackDefinition *mActiveAttack;
    ETouchMode mTouchMode;
    
    _BattleUIController *mUIController;
    
    CSCinemaCamera *mCamera;
    
    // Camera control
    CMVector2 mPrevTouchLoc;
    
public:
    CGBBattleTester(CSSceneManager *manager);
    virtual ~CGBBattleTester();
    void setupScene();
    
    void setCamera(CSCinemaCamera *camera);
    
    // Touch mode controls
    void setTouchMode(ETouchMode mode);
    
    // Attack mode
    void setAttackMode(CGBBattleAttackDefinition
                       *defn);
    
    // Events
    void receiveEvent(CEEvent *event);
};

#endif /* defined(__mythreal__BattleTestGame__) */
