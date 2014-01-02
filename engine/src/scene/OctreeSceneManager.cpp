//
//  OctreeSceneManager.cpp
//  mythreal
//
//  Created by Ian Ewell on 8/31/13.
//
//

#include "OctreeSceneManager.h"

CSOctreeNode::CSOctreeNode():
            mDepth(0), mHasChildren(false)
{
    
}

CSOctreeNode::CSOctreeNode(U32 depth):
            mDepth(depth), mHasChildren(false)
{
    
}

CSOctreeNode::~CSOctreeNode()
{
    
}

U32 CSOctreeNode::getTreeDepth()
{
    U32 ret = mDepth;
    for (U32 i = 0; i < 8; i++)
    {
        if (mHasChildren)
        {
            U32 depth = mChildren[i]->getTreeDepth();
            if (depth > ret)
                ret = depth;
        }
    }
    return ret;
}

void CSOctreeNode::subdivide()
{
    for (U32 i = 0; i < 8; i++)
    {
        mChildren[i] = new CSOctreeNode(mDepth+1);
    }
    mHasChildren = true;
}