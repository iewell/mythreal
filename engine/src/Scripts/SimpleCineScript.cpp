//
//  SimpleCineScript.cpp
//  mythreal
//
//  Created by Ian Ewell on 5/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SimpleCineScript.h"
#include <stdio.h>
#include "MainLoop.h"
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "TestPyramid.h"
#include "testplayer.h"
#include "Vector.h"
#include "ResourceManager.h"
#include "testCamera.h"
#include "CineText.h"
#include "ColorPlane.h"

//Globals:
extern CSTestPlayer gShip;
extern CSTestCamera gCamera;
extern CUICineText text;
extern CSColorPlane gColorPlane;



CSSimpleCineScript::CSSimpleCineScript()
{
    mTimer = 0;
    mStage = 0;
}

void CSSimpleCineScript::tick(F32 dt)
{
    mTimer += dt;
    switch (mStage) 
    {
        case 0:
            if (mTimer >= 1)
            {
                text.setText("...");
                mStage++;
                mTimer = 0;
            }
            break;
        case 1:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 2:
            if (mTimer >= 2)
            {
                text.setText("...");
                mStage++;
                mTimer = 0;
            }
            break;
        case 3:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 4:
            if (mTimer >= 2)
            {
                text.setText("...Where am I?");
                mStage++;
                mTimer = 0;
            }
            break;
        case 5:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 6:
            if (mTimer >= 2)
            {
                text.setText("Is there anybody here?");
                mStage++;
                mTimer = 0;
            }
            break;
        case 7:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 8:
            if (mTimer >= 2)
            {
                text.setText(" ");
                gShip.setTargetAlpha(0.5f);
                mStage++;
                mTimer = 0;
            }
            break;
        case 9:
            if (mTimer >= 2)
            {
                text.setText("What is this place?\nI can't see anything. Is this some weird dream?");
                gShip.setTargetAlpha(0.5f);
                mStage++;
                mTimer = 0;
            }
            break;
        case 10:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 11:
            if (mTimer >= 2)
            {
                text.setText("I see something appearing. What is this?");
                gColorPlane.setTargetAlpha(0.2f);
                mStage++;
                mTimer = 0;
            }
            break;
        case 12:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 13:
            if (mTimer >= 2)
            {
                text.setText(" ");
                gColorPlane.setTargetAlpha(1.0f);
                gShip.setTargetAlpha(1.0f);
                mStage++;
                mTimer = 0;
            }
            break;
        case 14:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 15:
            if (mTimer >= 2)
            {
                text.setText("Oooooh, a rainbow floor!    \nI like rainbows!");
                mStage++;
                mTimer = 0;
            }
            break;
        case 16:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 17:
            if (mTimer >= 2)
            {
                gCamera.setOrbitSpeed(10.0f);
                text.setText("Wait a minute... I just realized something...   \nI AM A CUBE! Stupid lazy 3D artists.");
                mStage++;
                mTimer = 0;
            }
            break;
        case 18:
            if (text.getFinishedScrolling())
            {
                mStage++;
                mTimer = 0;
            }
            break;
        case 19:
            if (mTimer >= 2)
            {
                gCamera.setOrbitSpeed(0.0f);
                gCamera.setOrbitAngle(0.0f);
                text.setText("Well, I want to explore. Hey you, in the camera, tap somewhere to have me go there. I am too stupid to move on my own.");
                mStage++;
                mTimer = 0;
            }
            break;
        case 20:
            if (text.getFinishedScrolling())
            {
                mStage++;
                gShip.enableControl(true);
                mTimer = 0;
            }
            break;


        default:
            break;
    }
}