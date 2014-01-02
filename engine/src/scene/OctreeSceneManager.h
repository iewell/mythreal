//
//  OctreeSceneManager.h
//  mythreal
//
//  Created by Ian Ewell on 8/31/13.
//
//

#ifndef __mythreal__OctreeSceneManager__
#define __mythreal__OctreeSceneManager__

#include <iostream>
#include <vector>
#include "Vector.h"
#include "Reference.h"
#include "SceneObject.h"

#define OCTREE_COMPLEXITY_CAP 5000 // Number of triangles before subdividing octree

class CSOctreeNode : public CCReferenceBase
{
protected:
    U32 mDepth; // Depth of octree
    bool mHasChildren; // Are we the bottom of the heap?
    
    std::vector<CSSceneObject*> mContents;
    CCPointerStrong<CSOctreeNode> mChildren[8];
    
public:
    CSOctreeNode();
    CSOctreeNode(U32 depth);
    virtual ~CSOctreeNode();
    
    U32 getTreeDepth();
    
    void subdivide();
};

#endif /* defined(__mythreal__OctreeSceneManager__) */
