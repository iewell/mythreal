/*
 *
 * FILE: Event.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Events
 *
 * PURSPOSE: Implement a base class for an event.
 *
 * CREATED ON: 10/11/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H

#include "eventReceiver.h"

typedef enum
{
    EVENT_NONE,
    EVENT_TIMER,
    EVENT_TOUCH,
    EVENT_TILT,
} EEventType;

//class CEEventReceiver;

class CEEvent
{
protected:
    // The reciever that recieves this event when the conditions are met
    CEEventReceiver* mReceiver;
    
    // This tells what type of event this is. The subclass will automatically set this.
    EEventType mEventType;
    
    // This tells whether an event will continue even after it has been triggered once
    bool mReusable;
    
    // Internal method that is called when all the event conditions are met
    void _fire();
    
public:
    CEEvent();
    virtual ~CEEvent() {}
    
    void setReciever(CEEventReceiver *reciever);
    CEEventReceiver* getReciever();
    
    // The event type is set by the superclass
    EEventType getEventType();
    
    // Sets if an event is reusable, or if it will delete itself after it is fired
    void setReusable(bool reusable);
    bool getReusable();
    
    // Ticks an event. During this time, an event updates its conditions' statuses and fires if it needs to.
    virtual void tick(float dt) = 0;
    
};

#endif