//
//  TestBattleStartScript.h
//  mythreal
//
//  Created by Ian Ewell on 3/30/13.
//
//

#ifndef __mythreal__TestBattleStartScript__
#define __mythreal__TestBattleStartScript__

#include <iostream>
#include "CinemaCamera.h"

class CSTestBattleStartScript
{
protected:
    CSCinemaCamera *mCamera;
    F32 mTimer;
    U32 mStage;
    
public:
    CSTestBattleStartScript(CSCinemaCamera *camera);
    void tick(F32 dt);
};

#endif /* defined(__mythreal__TestBattleStartScript__) */
