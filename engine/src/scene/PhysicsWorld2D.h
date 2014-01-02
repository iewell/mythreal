//
//  PhysicsWorld2D.h
//  mythreal
//
//  Created by Ian Ewell on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_PhysicsWorld2D_h
#define mythreal_PhysicsWorld2D_h

#include <Box2D/Box2D.h>

#include "Vector.h"
#include "LevelLayoutResource.h"

class CSPhysicsWorld2D
{
private:
    b2World *mWorld;
    b2Body *mGroundBody;
    
public:
    CSPhysicsWorld2D();
    CSPhysicsWorld2D(CMVector2 gravity);
    ~CSPhysicsWorld2D();
    
    b2Body* createDynamicBody(const CMVector2 &loc);
    
    void loadCollisionData(const std::string location);
    void addCollisionLine(const CMVector2 &p1, const CMVector2 &p2, ELineType lineType);
    
    void setGravity(const CMVector2 &gravity);
    
    void tick(F32 dt);
};

class CSPhysicsWorld2DListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact);
};

#endif
