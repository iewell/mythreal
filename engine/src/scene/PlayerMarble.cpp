//
//  PlayerMarble.cpp
//  mythreal
//
//  Created by Ian Ewell on 6/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "PlayerMarble.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "TimerEvent.h"

CSPlayerMarble::CSPlayerMarble(CSPhysicsWorld2D *world, F32 r)
{
    mPhysicsWorld = world;
    mBody = mPhysicsWorld->createDynamicBody(CMVector2(0.0f, 3.0f));
    
    b2CircleShape circleShape;
    circleShape.m_radius = r*0.1;
    
    b2FixtureDef fixDef;
    fixDef.shape = &circleShape;
    fixDef.density = 1.0f;
	fixDef.friction = 0.8f;
	fixDef.restitution = 0.2f;
	mBody->CreateFixture(&fixDef);
    mBody->SetType(b2_dynamicBody);
    
    // Pointer to ourself
    mBody->SetUserData(this);
    
    mDead = false;
    mDying = false;
}

void CSPlayerMarble::tick(F32 dt)
{
    CMVector4 pos(mBody->GetPosition().x*10, mBody->GetPosition().y*10, 0, 1);
    mWorld.setIdentity();
    mWorld.rotate(mBody->GetAngle(), 0, 0, 1);
    mWorld.setColumn(3, pos);
    
    // Are we dead?
    if (mDead)
    {
        mDead = false;
        mDying = false;
        mWorld.setIdentity();
        mBody->SetTransform(b2Vec2(0, 3), 0.0f);
        mVisible = true;
    }
    
    mRenderer->assignAreasWorld(gGlobalSceneManager->getAreas());
}

void CSPlayerMarble::setDead(bool dead)
{
    //mDead = true;
    mVisible = false;
    
    if (!mDying)
    {
        mDying = true;
        CETimerEvent *timer = new CETimerEvent;
        timer->setTargetTime(1.0f);
        timer->setReciever(this);
        gGlobalEventManager->addEvent(timer);
    }
}

void CSPlayerMarble::receiveEvent(CEEvent *event)
{
    if (event->getEventType() == EVENT_TIMER)
    {
        mDead = true;
    }
}