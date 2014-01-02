//
//  TestBattleStartScript.cpp
//  mythreal
//
//  Created by Ian Ewell on 3/30/13.
//
//

#include "TestBattleStartScript.h"

CSTestBattleStartScript::CSTestBattleStartScript(CSCinemaCamera *camera)
{
    mCamera = camera;
    mTimer = 0.0f;
    mStage = 6;
}

void CSTestBattleStartScript::tick(F32 dt)
{
    mTimer += dt;
    switch (mStage)
    {
        case 0:
            mCamera->setStart(CMVector3(75.0f, 40.0f, 200.0f), CMVector3(75.0f, 20.0f, 0.0f));
            mCamera->setEnd(CMVector3(75.0f, 30.0f, 100.0f), CMVector3(75.0f, 20.0f, 0.0f));
            mCamera->setDuration(5.0f);
            mCamera->start();
            mStage++;
            mTimer = 0.0f;
            break;
        case 1:
            if (mTimer >= 5.0f)
                mStage++;
            break;
        case 2:
            mCamera->setStart(CMVector3(0.0f, 40.0f, 60.0f), CMVector3(0.0f, 0.0f, 0.0f));
            mCamera->setEnd(CMVector3(150.0f, 40.0f, 60.0f), CMVector3(150.0f, 40.0f, 60.0f));
            mCamera->setDuration(5.0f);
            mCamera->start();
            mStage++;
            mTimer = 0.0f;
            break;
        case 3:
            if (mTimer >= 5.0f)
                mStage++;
            break;
        case 4:
            mCamera->setStart(CMVector3(75.0f, 40.0f, 80.0f), CMVector3(75.0f, 20.0f, 0.0f));
            mCamera->setEnd(CMVector3(75.0f, 30.0f, 55.0f), CMVector3(75.0f, 20.0f, 0.0f));
            mCamera->setDuration(5.0f);
            mCamera->start();
            mStage++;
            mTimer = 0.0f;
            break;
        case 5:
            if (mTimer >= 8.0f)
                mStage++;
            break;
        case 6:
            mCamera->setStart(CMVector3(75.0f, 30.0f, 55.0f), CMVector3(75.0f, 20.0f, 0.0f));
            mCamera->setEnd(CMVector3(75.0f, 135.0f, 75.0f), CMVector3(75.0f, 0.0f, 0.0f));
            mCamera->setDuration(5.0f);
            mCamera->start();
            mStage++;
            mTimer = 0.0f;
            break;
    }
}