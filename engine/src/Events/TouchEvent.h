//
//  TouchEvent.h
//  mythreal
//
//  Created by Ian Ewell on 4/12/13.
//
//

#ifndef __mythreal__TouchEvent__
#define __mythreal__TouchEvent__

#include <iostream>
#include "Event.h"
#include "Vector.h"

class TouchWrapper;

typedef enum
{
    TOUCH_BEGIN,
    TOUCH_MOVED,
    TOUCH_END,
    TOUCH_TAP,
    TOUCH_DOUBLE_TAP,
} ETouchEventType;


typedef struct
{
    CMVector2 touchLoc;
    ETouchEventType type;
} STouchInfo;

class CETouchEvent : public CEEvent
{
protected:
    friend class TouchWrapper;
    TouchWrapper *mWrapper;
    STouchInfo mTouchInfo;
    
    void _updateTouchInfo(STouchInfo info);
    
public:
    CETouchEvent();
    virtual ~CETouchEvent();
    
    STouchInfo getTouchInfo()
    {
        return mTouchInfo;
    }
    
    void tick(float dt) {}
};

#endif /* defined(__mythreal__TouchEvent__) */
