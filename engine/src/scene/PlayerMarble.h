//
//  PlayerMarble.h
//  mythreal
//
//  Created by Ian Ewell on 6/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_PlayerMarble_h
#define mythreal_PlayerMarble_h

#include "testm3d.h"
#include "PhysicsWorld2D.h"

class CSPlayerMarble : public CSTestM3D
{
private:
    CSPhysicsWorld2D *mPhysicsWorld;
    b2Body *mBody;
    F32 mRadius;
    
    bool mDead;
    bool mDying;
    
public:
    CSPlayerMarble(CSPhysicsWorld2D *world, F32 r);
    ~CSPlayerMarble();
    
    void tick(F32 dt);
    
    void setDead(bool dead);
    
    void receiveEvent(CEEvent *event);
};

#endif
