/*
 *
 * FILE: eventReceiver.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Events
 *
 * PURSPOSE: Implement a base class that recieves events.
 *
 * CREATED ON: 10/11/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef EVENT_EVENTRECIEVER_H
#define EVENT_EVENTRECIEVER_H

class CEEvent;

// This class recieves fired events that were told to invoke an instance.
// Any class that needs to recieve and handle any event must be a superclass
// of this class.
class CEEventReceiver
{
    friend class CEEvent;
    
protected:
    void receiveEvent(CEEvent *event);
};

#endif