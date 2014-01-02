//
//  3DModelResource.h
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal__DModelResource_h
#define mythreal__DModelResource_h

#include "MaterialResource.h"

#define BONES_PER_VERTEX 4

typedef struct BONE_WEIGHT
{
    float weights[BONES_PER_VERTEX];
} SBoneWeight;

typedef struct BONE_INFLUENCE
{
    unsigned int index[BONES_PER_VERTEX];
} SBoneInfluence;

class CR3DModelMaterialGroup : public CCHeirarchyNode
{
protected:
    U16 mTriCount;
    
    void* mVertIndicies;
    void* mUVIndicies;
    void* mNormalIndicies;
    
    CRResourceNode *mMaterialData;
    std::string mMaterialDataRef;
    
public:
    CR3DModelMaterialGroup();
    virtual ~CR3DModelMaterialGroup();
    
    U16 getTriangleCount()
    {
        return mTriCount;
    }
    
    void* getVertIndicies()
    {
        return mVertIndicies;
    }
    
    void* getUVIndicies()
    {
        return mUVIndicies;
    }
    
    void* getNormalIndicies()
    {
        return mNormalIndicies;
    }
    
    CRResourceNode* getMaterialData()
    {
        if (!mMaterialData)
        {
            mMaterialData = CRResourceManager::Get()->findResourceOfType(mMaterialDataRef, RT_MATERIAL);
            mMaterialData->loadResource();
        }
        return mMaterialData;
    }
    
    void readFromStream(CCFileStream *stream);
};

class CR3DModelSkeletonNode : public CCHeirarchyNode
{
    CMMatrix4x4 mInitialMatrix;
    CMMatrix4x4 mBoneTransform;
    U32 mFrameCount;
    U32 mBoneID;
    std::vector<CMMatrix4x4> mFrames;
    
public:
    CR3DModelSkeletonNode()
    {
        
    }
    
    CMMatrix4x4 getInitialMatrix()
    {
        return mInitialMatrix;
    }
    
    void computeTransform(CMMatrix4x4 parentInitial, CMMatrix4x4 parentAnim, U32 frame);
    CMMatrix4x4 getTransform()
    {
        return mBoneTransform;
    }
    
    U32 getFrameCount()
    {
        return mFrameCount;
    }
    
    U32 getBoneID()
    {
        return mBoneID;
    }
    
    CR3DModelSkeletonNode* findNodeWithID(U32 ID);
    
    void readFromStream(CCFileStream *stream);
};

typedef enum
{
    AM_PLAY_ONCE,
    AM_LOOP,
    AM_PINGPONG,
} EAnimationMode;

class CR3DModelAnimation : public CCHeirarchyNode
{
protected:
    U32 mStartFrame;
    U32 mEndFrame;
    EAnimationMode mAnimationMode;
    
public:
    CR3DModelAnimation()
    {
        mStartFrame = 0;
        mEndFrame = 0;
        mAnimationMode = AM_PLAY_ONCE;
    }
    
    U32 getStartFrame()
    {
        return mStartFrame;
    }
    
    U32 getEndFrame()
    {
        return mEndFrame;
    }
    
    EAnimationMode getAnimationMode()
    {
        return mAnimationMode;
    }
    
    void readFromStream(CCFileStream *stream);
};

class CR3DModelRoot : public CCHeirarchyNode
{
protected:
    U32 mVertexCount;
    U32 mUVCount;
    U32 mNormalCount;
    
    void *mVerticies;
    void *mUVs;
    void *mNormals;
    
    bool mIsSkinned;
    U32 mWeightCount;
    SBoneWeight *mWeights;
    SBoneInfluence *mInfluences;
    U32 mFrameCount;
    std::vector<CR3DModelSkeletonNode*> mBoneList;
    
public:
    CR3DModelRoot();
    virtual ~CR3DModelRoot();
    
    U32 getVertexCount()
    {
        return mVertexCount;
    }
    
    U32 getUVCount()
    {
        return mUVCount;
    }
    
    U32 getNormalCount()
    {
        return mNormalCount;
    }
    
    void* getVertices()
    {
        return mVerticies;
    }
    
    void* getUVs()
    {
        return mUVs;
    }
    
    void* getNormals()
    {
        return mNormals;
    }
    
    bool isSkinned()
    {
        return mIsSkinned;
    }
    
    SBoneWeight* getBoneWeights()
    {
        return mWeights;
    }
    
    SBoneInfluence* getBoneInfluences()
    {
        return mInfluences;
    }
    
    U32 getFrameCount()
    {
        return mFrameCount;
    }
    
    std::vector<CR3DModelSkeletonNode*> getBoneList()
    {
        return mBoneList;
    }
    
    void readFromStream(CCFileStream *stream);
};

class CR3DModelResource : public CRResource
{
protected:
    CR3DModelRoot *mRoot;
    
public:
    CR3DModelResource();
    ~CR3DModelResource();
    
    CR3DModelRoot* getRoot()
    {
        return mRoot;
    }
    
    void readFromStream(CCFileStream *stream);
};

#endif
