//
//  SimpleCineScript.h
//  mythreal
//
//  Created by Ian Ewell on 5/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SCRIPT_SIMPLE_CINE_H
#define SCRIPT_SIMPLE_CINE_H

#include "Types.h"

class CSSimpleCineScript
{
protected:
    F32 mTimer;
    U32 mStage;
    
public:
    CSSimpleCineScript();
    void tick(F32 dt);
};

#endif