//
//  TimerEvent.h
//  mythreal
//
//  Created by Ian Ewell on 11/24/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_TimerEvent_h
#define mythreal_TimerEvent_h

#include "Event.h"
#include "Types.h"

class CETimerEvent : public CEEvent
{
protected:
    F32 mClock;
    F32 mTargetTime;
    
public:
    CETimerEvent();
    virtual ~CETimerEvent();
    
    void setTargetTime(F32 timeS);
    F32 getTargetTime();
    
    F32 getCurrentClock();
    
    void resetTimer();
    
    virtual void tick(float dt);
};


#endif
