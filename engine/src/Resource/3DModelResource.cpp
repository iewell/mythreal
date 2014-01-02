//
//  3DModelResource.cpp
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "3DModelResource.h"

CR3DModelMaterialGroup::CR3DModelMaterialGroup()
{
    mVertIndicies = NULL;
    mUVIndicies = NULL;
    mNormalIndicies = NULL;
    
    mMaterialData = NULL;
    
    mType = "MaterialGroup";
}

CR3DModelMaterialGroup::~CR3DModelMaterialGroup()
{
    if (mVertIndicies)
        free(mVertIndicies);
    if (mUVIndicies)
        free(mUVIndicies);
    if (mNormalIndicies)
        free(mNormalIndicies);
    
    if (mMaterialData)
        mMaterialData->releaseResource();
}

void CR3DModelMaterialGroup::readFromStream(CCFileStream *stream)
{
    mTriCount = stream->readUShort();
    
    mVertIndicies = malloc(mTriCount*6);
    mUVIndicies = malloc(mTriCount*6);
    mNormalIndicies = malloc(mTriCount*6);
    
    stream->read(mTriCount*6, mVertIndicies);
    stream->read(mTriCount*6, mUVIndicies);
    stream->read(mTriCount*6, mNormalIndicies);
    
    mMaterialDataRef = stream->readString();
    
    readChildrenFromStream(stream);
}

//////////////////////////////////////////////////////////////////////////

void CR3DModelSkeletonNode::computeTransform(CMMatrix4x4 parentInitial, CMMatrix4x4 parentAnim, U32 frame)
{
    CMMatrix4x4 initialMatrix = parentInitial;
    initialMatrix.multiply(mInitialMatrix);
    CMMatrix4x4 initialMatrixInv = initialMatrix;
    initialMatrixInv.inverse();
    
    CMMatrix4x4 frameTransform = parentAnim;
    frameTransform.multiply(mFrames[frame]);
    
    mBoneTransform = frameTransform;
    mBoneTransform.multiply(initialMatrixInv);
    
    if (mChildren.size() == 0)
        return;
    for (std::vector<CCHeirarchyNode*>::iterator it = mChildren.begin(); it != mChildren.end(); it++)
        static_cast<CR3DModelSkeletonNode*>((*it))->computeTransform(initialMatrix, frameTransform, frame);
}

CR3DModelSkeletonNode* CR3DModelSkeletonNode::findNodeWithID(U32 ID)
{
    if (mBoneID == ID)
        return this;
    for (std::vector<CCHeirarchyNode*>::iterator it = mChildren.begin(); it != mChildren.end(); it++)
    {
        CR3DModelSkeletonNode *bone = dynamic_cast<CR3DModelSkeletonNode*>((*it))->findNodeWithID(ID);
        if (bone)
            return bone;
    }
    return NULL;
}

void CR3DModelSkeletonNode::readFromStream(CCFileStream *stream)
{
    stream->read(sizeof(F32)*16, mInitialMatrix.getArray());
    //mInitialMatrix = mInitialMatrix.transpose();
    mFrameCount = stream->readUInt();
    mBoneID = stream->readUInt();
    for (int i = 0; i < mFrameCount; i++)
    {
        CMMatrix4x4 mat;
        stream->read(sizeof(F32)*16, mat.getArray());
        //mat = mat.transpose();
        mFrames.push_back(mat);
    }
    readChildrenFromStream(stream);
}

//////////////////////////////////////////////////////////////////////////

void CR3DModelAnimation::readFromStream(CCFileStream *stream)
{
    mStartFrame = stream->readUInt();
    mEndFrame = stream->readUInt();
    mAnimationMode = (EAnimationMode)stream->readUInt();
    readChildrenFromStream(stream);
}

//////////////////////////////////////////////////////////////////////////

CR3DModelRoot::CR3DModelRoot()
{
    mType = "3DModelRoot";
    
    mVerticies = NULL;
    mUVs = NULL;
    mNormals = NULL;
}

CR3DModelRoot::~CR3DModelRoot()
{
    if (mVerticies)
        free(mVerticies);
    if (mUVs)
        free(mUVs);
    if (mNormals)
        free(mNormals);
}

void CR3DModelRoot::readFromStream(CCFileStream *stream)
{
    mVertexCount = stream->readUShort();
    mUVCount = stream->readUShort();
    mNormalCount = stream->readUShort();
    
    mVerticies = malloc(mVertexCount*3*sizeof(float));
    mUVs = malloc(mUVCount*2*sizeof(float));
    mNormals = malloc(mNormalCount*3*sizeof(float));
    
    stream->read(mVertexCount*3*sizeof(float), mVerticies);
    stream->read(mVertexCount*2*sizeof(float), mUVs);
    stream->read(mVertexCount*3*sizeof(float), mNormals);
    
    mIsSkinned = stream->readChar();
    if (mIsSkinned)
    {
        mWeightCount = stream->readUInt();
        mWeights = new SBoneWeight[mWeightCount];
        stream->read(mWeightCount*sizeof(SBoneWeight), mWeights);
        mInfluences = new SBoneInfluence[mWeightCount];
        stream->read(mWeightCount*sizeof(SBoneInfluence), mInfluences);
        mFrameCount = stream->readInt();
    }
    
    readChildrenFromStream(stream);
    
    if (mIsSkinned)
    {
        int bID = 0;
        CCHeirarchyNode *skltn = findChildWithName("Skeleton");
        while (1)
        {
            CR3DModelSkeletonNode *searchBone;
            for (std::vector<CCHeirarchyNode*>::iterator it = skltn->getChildren().begin(); it != skltn->getChildren().end(); it++)
            {
                searchBone = dynamic_cast<CR3DModelSkeletonNode*>((*it))->findNodeWithID(bID);
                if (searchBone != NULL)
                    break;
            }
            if (searchBone == NULL)
                break;
            mBoneList.push_back(searchBone);
            bID++;
        }
    }
}

CR3DModelResource::CR3DModelResource()
{
    mType = "3DModel";
    mRoot = NULL;
}

CR3DModelResource::~CR3DModelResource()
{
    if (mRoot)
        delete mRoot;
}

void CR3DModelResource::readFromStream(CCFileStream *stream)
{
    std::string name = stream->readString();
    stream->readString();
    
    mRoot = new CR3DModelRoot;
    mRoot->setName(name);
    mRoot->readFromStream(stream);
}