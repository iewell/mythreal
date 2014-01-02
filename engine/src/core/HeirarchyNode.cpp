//
//  HeirarchyNode.cpp
//  mythreal
//
//  Created by Ian Ewell on 7/28/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "HeirarchyNode.h"
#include "Resource.h"
#include "3DModelResource.h"
#include "LevelLayoutResource.h"

CCHeirarchyNode::CCHeirarchyNode()
{
    mParent = NULL;
    
    mName = "";
    mType = "Generic";
    
    mDepth = 0;
    mID = 0;
}

CCHeirarchyNode::~CCHeirarchyNode()
{
    
    for (std::vector<CCHeirarchyNode*>::iterator it = mChildren.begin(); it < mChildren.end(); it++)
    {
        delete (*it);
    }
}

void CCHeirarchyNode::insertChild(CCHeirarchyNode *child)
{
    child->setID(mChildren.size());
    child->setDepth(mDepth+1);
    child->setParent(this);
    mChildren.push_back(child);
}

CCHeirarchyNode* CCHeirarchyNode::getChildAtIndex(U32 index)
{
    return mChildren.at(index);
}

std::vector<CCHeirarchyNode*> CCHeirarchyNode::getChildren()
{
    return mChildren;
}

CCHeirarchyNode* CCHeirarchyNode::findChildWithName(const std::string &name, bool resursive)
{
    for (std::vector<CCHeirarchyNode*>::iterator it = mChildren.begin(); it < mChildren.end(); it++)
    {
        if ((*it)->getName().compare(name) == 0)
            return (*it);
        else if (resursive && (*it)->getNumberOfChildren() > 0)
        {
            CCHeirarchyNode *node2;
            if ((node2 = (*it)->findChildWithName(name, true)))
                return node2;
        }
    }
    return NULL;
}

U32 CCHeirarchyNode::getNumberOfChildren()
{
    return mChildren.size();
}

std::string CCHeirarchyNode::getNodeType()
{
    return mType;
}

void CCHeirarchyNode::setParent(CCHeirarchyNode *parent)
{
    mParent = parent;
}

CCHeirarchyNode* CCHeirarchyNode::getParent()
{
    return mParent;
}

void CCHeirarchyNode::removeChildAtIndex(U32 index)
{
    mChildren.erase(mChildren.begin()+index);
}

void CCHeirarchyNode::removeSelf()
{
    if (mParent)
        mParent->removeChildAtIndex(mID);
}

void CCHeirarchyNode::readFromStream(CCFileStream *stream)
{
    readChildrenFromStream(stream);
}

void CCHeirarchyNode::readChildrenFromStream(CCFileStream *stream)
{
    U32 childrenCount = stream->readUInt();
    for (int i = 0; i < childrenCount; i++)
    {
        CCHeirarchyNode *child;
        std::string name = stream->readString();
        std::string type = stream->readString();
        
        if (type.compare("Resource") == 0)
            child = new CRResourceNode;
        else if (type.compare("MaterialGroup") == 0)
            child = new CR3DModelMaterialGroup;
        else if (type.compare("3DModelSkeletonNode") == 0)
            child = new CR3DModelSkeletonNode;
        else if (type.compare("3DModelAnimation") == 0)
            child = new CR3DModelAnimation;
        else if (type.compare("2DPhysicsLine") == 0)
            child = new CR2DPhysicsLine;
        else if (type.compare("2DArea") == 0)
            child = new CRArea;
        else if (type.compare("SceneLight") == 0)
            child = new CRLight;
        else if (type.compare("3DSceneObjectInstance") == 0)
            child = new CRSceneObject;
        else
            child = new CCHeirarchyNode;
        
        child->setName(name);
        insertChild(child);
        child->readFromStream(stream);
    }
}