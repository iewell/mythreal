//
//  EngineObject.h
//  mythreal
//
//  Created by Ian Ewell on 4/22/13.
//
//

#ifndef __mythreal__EngineObject__
#define __mythreal__EngineObject__

#include <iostream>
#include <string>
#include <vector>

#include "Reference.h"

// Class to store information on a class so that the engine can instantiate it on demand
class CCClassBase
{
public:
    CCClassBase();
    virtual ~CCClassBase();
    
protected:
    std::string mName;
    CCClassBase *mParent;
    
    bool mIsAbstract;
    
    static std::vector<CCClassBase*> mClasses;
};

template <class T>
class CCClass : public CCClassBase
{
    
};

class CCEngineObject : public CCReferenceBase
{

};

#endif /* defined(__mythreal__EngineObject__) */
