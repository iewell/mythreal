//
//  TouchEvent.mm
//  mythreal
//
//  Created by Ian Ewell on 4/12/13.
//
//

#import <UIKit/UIKit.h>
#include "TouchEvent.h"
//#import "EAGLView.h"

@interface TouchView : UIView
{
    NSMutableArray *mTouchReceivers;
}

- (void)addTouchReceiver:(UIResponder*)receiver;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

@end

TouchView *touchView;

@implementation TouchView

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder])
    {
        touchView = self;
    }
    return self;
}

- (void)addTouchReceiver:(UIResponder *)receiver
{
    if (!mTouchReceivers)
        mTouchReceivers = [[NSMutableArray alloc] init];
    [mTouchReceivers addObject:receiver];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UIResponder *receiver in mTouchReceivers)
        [receiver touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UIResponder *receiver in mTouchReceivers)
        [receiver touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UIResponder *receiver in mTouchReceivers)
        [receiver touchesEnded:touches withEvent:event];
}

@end

@interface TouchReceiver : UIResponder
{
    TouchWrapper *_wrapper;
}

- (id)initWithWrapper:(TouchWrapper*)wrapper;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

@end

class TouchWrapper
{
protected:
    TouchReceiver *mReceiver;
    CETouchEvent *mEvent;
    
public:
    TouchWrapper(CETouchEvent *event)
    {
        mReceiver = [[TouchReceiver alloc] initWithWrapper:this];
        mEvent = event;
    }
    
    virtual ~TouchWrapper()
    {
        [mReceiver release];
    }
    
    void passTouchInfo(STouchInfo info)
    {
        mEvent->_updateTouchInfo(info);
    }
};

@implementation TouchReceiver

- (id)initWithWrapper:(TouchWrapper*)wrapper
{
    if (self = [super init])
    {
        _wrapper = wrapper;
        [touchView addTouchReceiver:self];
        
        return self;
    }
    return nil;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [touches anyObject];
    CGPoint loc = [touch locationInView:nil];
    STouchInfo info;
    info.touchLoc.setX(loc.x);
    info.touchLoc.setY(loc.y);
    info.type = TOUCH_BEGIN;
    _wrapper->passTouchInfo(info);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [touches anyObject];
    CGPoint loc = [touch locationInView:nil];
    STouchInfo info;
    info.touchLoc.setX(loc.x);
    info.touchLoc.setY(loc.y);
    info.type = TOUCH_MOVED;
    _wrapper->passTouchInfo(info);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [touches anyObject];
    CGPoint loc = [touch locationInView:nil];
    STouchInfo info;
    info.touchLoc.setX(loc.x);
    info.touchLoc.setY(loc.y);
    info.type = TOUCH_END;
    _wrapper->passTouchInfo(info);
    
    if ([touch tapCount] == 1)
    {
        info.type = TOUCH_TAP;
        _wrapper->passTouchInfo(info);
    }
    else if ([touch tapCount] == 2)
    {
        info.type = TOUCH_DOUBLE_TAP;
        _wrapper->passTouchInfo(info);
    }
}

@end

CETouchEvent::CETouchEvent()
{
    mEventType = EVENT_TOUCH;
    mWrapper = new TouchWrapper(this);
}

CETouchEvent::~CETouchEvent()
{
    delete mWrapper;
}

void CETouchEvent::_updateTouchInfo(STouchInfo info)
{
    mTouchInfo = info;
    _fire();
}