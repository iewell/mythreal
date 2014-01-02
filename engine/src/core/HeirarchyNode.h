//
//  HeirarchyNode.h
//  mythreal
//
//  Created by Ian Ewell on 7/28/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_HeirarchyNode_h
#define mythreal_HeirarchyNode_h

#include <string>
#include <vector>
#include "Types.h"
#include "FileStream.h"

class CCHeirarchyNode
{
protected:
    std::string mName;
    std::string mType;
    
    U32 mDepth;
    U32 mID;
    
    std::vector<CCHeirarchyNode*> mChildren;
    CCHeirarchyNode *mParent;
    
public:
    CCHeirarchyNode();
    virtual ~CCHeirarchyNode();
    
    void insertChild(CCHeirarchyNode* child);
    CCHeirarchyNode* getChildAtIndex(U32 index);
    std::vector<CCHeirarchyNode*> getChildren();
    CCHeirarchyNode* findChildWithName(const std::string &name, bool resursive=false);
    U32 getNumberOfChildren();
    std::string getNodeType();
    void setParent(CCHeirarchyNode *parent);
    CCHeirarchyNode* getParent();
    void removeChildAtIndex(U32 index);
    void removeSelf();
    
    virtual void readFromStream(CCFileStream *stream);
    
protected:
    void readChildrenFromStream(CCFileStream *stream);
    
public:
    void setName(const std::string &name)
    {
        mName = name;
    }
    
    std::string getName()
    {
        return mName;
    }
    
    void setID(U32 ID)
    {
        mID = ID;
    }
    
    U32 getID()
    {
        return mID;
    }
    
    U32 getDepth()
    {
        return mDepth;
    }
    
    void setDepth(U32 depth)
    {
        mDepth = depth;
    }
};

#endif
