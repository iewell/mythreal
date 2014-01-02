//
//  EventManager.cpp
//  mythreal
//
//  Created by Ian Ewell on 10/15/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "EventManager.h"

CEEventManager *gGlobalEventManager;

CEEventManager::CEEventManager()
{
    
}

CEEventManager::~CEEventManager()
{
    std::list<CEEvent*>::iterator it;
    for (it = mActiveEventList.begin(); it != mActiveEventList.end(); it++)
        delete (*it);
}

void CEEventManager::addEvent(CEEvent *event)
{
    mActiveEventList.push_back(event);
}

void CEEventManager::removeEvent(CEEvent *event)
{
    mActiveEventList.remove(event);
}

void CEEventManager::tick(F32 dt)
{
    std::list<CEEvent*>::iterator it;
    for (it = mActiveEventList.begin(); it != mActiveEventList.end(); it++)
        (*it)->tick(dt);
}