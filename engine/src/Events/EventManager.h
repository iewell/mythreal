//
//  EventManager.h
//  mythreal
//
//  Created by Ian Ewell on 10/15/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_EventManager_h
#define mythreal_EventManager_h

#include <list>
#include "Types.h"
#include "Event.h"

class CEEventManager
{
    std::list<CEEvent*> mActiveEventList;
    
public:
    CEEventManager();
    ~CEEventManager();
    
    void addEvent(CEEvent* event);
    void removeEvent(CEEvent *event);
    
    void tick(F32 dt);
};

extern CEEventManager *gGlobalEventManager;

#endif
