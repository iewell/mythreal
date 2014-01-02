//
//  PhysicsWorld2D.cpp
//  mythreal
//
//  Created by Ian Ewell on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "PhysicsWorld2D.h"
#include "FileStream.h"
#include "ResourceManager.h"
#include "PlayerMarble.h"

CSPhysicsWorld2D::CSPhysicsWorld2D(CMVector2 gravity)
{
    b2Vec2 g(gravity.getX(), gravity.getY());
    mWorld = new b2World(g, false);
    mWorld->SetContinuousPhysics(true);
    mGroundBody = NULL;
    
    CSPhysicsWorld2DListener *listener = new CSPhysicsWorld2DListener;
    mWorld->SetContactListener(listener);
    
    // Create a temporary ground floor
    //b2BodyDef ground;
    //ground.position.Set(0.0f, -0.25f);
    //b2Body *groundBody = mWorld->CreateBody(&ground);
    //b2PolygonShape shape;
    //shape.SetAsEdge(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
    //groundBody->CreateFixture(&shape, 0);
}

CSPhysicsWorld2D::CSPhysicsWorld2D()
{
    CSPhysicsWorld2D(CMVector2(0, -9.8));
    mGroundBody = NULL;
}

CSPhysicsWorld2D::~CSPhysicsWorld2D()
{
    delete mWorld;
}

b2Body* CSPhysicsWorld2D::createDynamicBody(const CMVector2 &loc)
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(loc.getX(), loc.getY());
    
    b2Body *body = mWorld->CreateBody(&def);
    return body;
}

void CSPhysicsWorld2D::loadCollisionData(const std::string location)
{
    CCFileStream *stream = static_cast<CCFileStream*>(CRResourceManager::Get()->requestStream(location, RLID_APP, RPID_READONLY));
    
    char magic[4];
    stream->read(4, magic);
    
    // Define a new body for our collision
    b2BodyDef ground;
    ground.position.Set(0.0f, 0.0f);
    b2Body *groundBody = mWorld->CreateBody(&ground);
    
    U32 lineCount = stream->readUInt();
    for (int i = 0; i < lineCount; i++)
    {
        float x1 = (float)(stream->readDouble()*0.1f);
        float y1 = (float)(stream->readDouble()*0.1f);
        float x2 = (float)(stream->readDouble()*0.1f);
        float y2 = (float)(stream->readDouble()*0.1f);
        b2PolygonShape shape;
        shape.SetAsEdge(b2Vec2(x1, y1), b2Vec2(x2, y2));
        groundBody->CreateFixture(&shape, 0);
    }
    
    delete stream;
}

void CSPhysicsWorld2D::addCollisionLine(const CMVector2 &p1, const CMVector2 &p2, ELineType lineType)
{
    if (!mGroundBody)
    {
        b2BodyDef ground;
        ground.position.Set(0.0f, 0.0f);
        mGroundBody = mWorld->CreateBody(&ground);
    }
    
    float x1 = (float)(p1.getX()*0.1f);
    float y1 = (float)(p1.getY()*0.1f);
    float x2 = (float)(p2.getX()*0.1f);
    float y2 = (float)(p2.getY()*0.1f);
    b2PolygonShape shape;
    shape.SetAsEdge(b2Vec2(x1, y1), b2Vec2(x2, y2));
    
    b2FixtureDef fixture;
    fixture.shape = &shape;
    switch (lineType)
    {
        case LT_COLLISION:
            fixture.filter.categoryBits = 0x0001;
            break;
        case LT_KILL:
            fixture.filter.categoryBits = 0x0002;
            break;
    }
    mGroundBody->CreateFixture(&fixture);

}

void CSPhysicsWorld2D::setGravity(const CMVector2 &gravity)
{
    mWorld->SetGravity(b2Vec2(gravity.getX(), gravity.getY()));
}

void CSPhysicsWorld2D::tick(F32 dt)
{
    // Step twice
    mWorld->Step(dt/2, 10, 10);
    mWorld->Step(dt/2, 10, 10);
}

void CSPhysicsWorld2DListener::BeginContact(b2Contact* contact)
{
    b2Body *body = contact->GetFixtureB()->GetBody();
    b2Fixture *lineFix = contact->GetFixtureA();
    if (body->GetUserData() && lineFix->GetFilterData().categoryBits == 0x0002)
    {
        ((CSPlayerMarble*)body->GetUserData())->setDead(true);
    }
}