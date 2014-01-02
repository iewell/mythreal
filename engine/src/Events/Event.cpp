/*
 *
 * FILE: Event.cpp
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

#include "Event.h"
#include "eventReceiver.h"

CEEvent::CEEvent()
{
    mEventType = EVENT_NONE;
}

void CEEvent::_fire()
{
    mReceiver->receiveEvent(this);
    
    if (!mReusable)
        delete this;
}

void CEEvent::setReciever(CEEventReceiver *reciever)
{
    mReceiver = reciever;
}

CEEventReceiver* CEEvent::getReciever()
{
    return mReceiver;
}

EEventType CEEvent::getEventType()
{
    return mEventType;
}

void CEEvent::setReusable(bool reusable)
{
    mReusable = reusable;
}

bool CEEvent::getReusable()
{
    return mReusable;
}