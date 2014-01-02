//
//  TimerEvent.cpp
//  mythreal
//
//  Created by Ian Ewell on 11/24/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "TimerEvent.h"

CETimerEvent::CETimerEvent()
{
    mClock = 0.0f;
    mTargetTime = 0.0f;
    mEventType = EVENT_TIMER;
}

CETimerEvent::~CETimerEvent()
{
    
}

void CETimerEvent::setTargetTime(F32 timeS)
{
    mTargetTime = timeS;
}

F32 CETimerEvent::getTargetTime()
{
    return mTargetTime;
}

F32 CETimerEvent::getCurrentClock()
{
    return mClock;
}

void CETimerEvent::resetTimer()
{
    mClock = 0.0f;
}

void CETimerEvent::tick(F32 dt)
{
    mClock += dt;
    if (mClock >= mTargetTime)
    {
        mClock = 0.0f;
        _fire();
    }
}