/*
 *
 * FILE: testm3drender.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Graphics
 *
 * PURSPOSE: Renders a model in the m3d format.
 *
 * CREATED ON: 08/06/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#include "testm3drender.h"
#include "SceneManager.h"

static bool gGPUSkin = true;;

CGM3DTestRenderer::CGM3DTestRenderer()
{
    mVertexList = NULL;
    mUVList = NULL;
    mNormalsList = NULL;
    mAlpha = 1.0f;
    mVertexCount = 0;
    
    mCurrentFrame = 20;
    
    // Reflection defaults
    mReflecting = false;
    mReflectionSize = 128;
    mReflectionFrameSkip = 2;
    mReflectionFramebuffer = 0;
    mRelfectionColorbuffer = 0;
    mReflectionDepthBuffer = 0;
    mReflectionTexture = 0;
    mReflectionFramebufferAllocated = false;
    
    mReflectionProjectionMatrix.makePerspectiveMatrix(90.0f, 1.0f, 2.0f, 200.0f);
}

CGM3DTestRenderer::~CGM3DTestRenderer()
{
    if (mVertexList)
        free(mVertexList);
    if (mUVList)
        free(mUVList);
    if (mNormalsList)
        free(mNormalsList);
}

void CGM3DTestRenderer::_allocateReflectionFramebuffer()
{
    // Create the framebuffer
    glGenFramebuffers(1, &mReflectionFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mReflectionFramebuffer);
    
    // Generate the cube map texture
    glGenTextures(1, &mReflectionTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mReflectionTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    for (GLenum i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i < GL_TEXTURE_CUBE_MAP_POSITIVE_X+6; i++)
    {
        glTexImage2D(i, 0, GL_RGBA, mReflectionSize, mReflectionSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    
    // Depth buffer
    glGenRenderbuffers(1, &mReflectionDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mReflectionDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mReflectionSize, mReflectionSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mReflectionDepthBuffer);
    
    // Check the framebuffer
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(status != GL_FRAMEBUFFER_COMPLETE) 
    {
        // No reflections
        glDeleteFramebuffers(1, &mReflectionFramebuffer);
        glDeleteTextures(1, &mReflectionTexture);
        glDeleteRenderbuffers(1, &mReflectionDepthBuffer);
        mReflecting = false;
        return;
    }
    
    // Add the texture to the render groups
    std::list<SMaterialGroup>::iterator it;
    for (it = mMatGroups.begin(); it != mMatGroups.end(); it++)
    {
        //(*it).material->addTextureUniform("reflectMap", TT_CUBE, mReflectionTexture);
    }
    
    mReflectionFramebufferAllocated = true;
}

void CGM3DTestRenderer::setVertexList(void *vl)
{
    mVertexList = (F32*)vl;
}

void CGM3DTestRenderer::setUVList(void *uvl)
{
    mUVList = uvl;
}

void CGM3DTestRenderer::setNormalList(void *nl)
{
    mNormalsList = (F32*)nl;
}

void CGM3DTestRenderer::setVertexCount(unsigned int vc)
{
    mVertexCount = vc;
}

void CGM3DTestRenderer::set3DModelResource(CRResourceNode *node)
{
    printf("%ld\n", sizeof(CMMatrix4x4));
    
    m3DModel = node;
    m3DModel->loadResource();
    
    // Extract the material groups
    CR3DModelRoot *root = static_cast<CR3DModelResource*>(m3DModel->getResource())->getRoot();
    setVertexCount(root->getVertexCount());
    setVertexList(root->getVertices());
    setUVList(root->getUVs());
    setNormalList(root->getNormals());
    
    // Upload to our new buffer
    CVertexArray vertices(mVertexCount);
    vertices.copyFromBuffer((SVertex*)mVertexList, mVertexCount);
    mVertexBuffer.addBuffer(vertices, ATTRIB_VERTEX);
    
    CUVArray uvs(mVertexCount);
    uvs.copyFromBuffer((SUV*)mUVList, mVertexCount);
    mVertexBuffer.addBuffer(uvs, ATTRIB_UV);
    
    CVertexArray normals(mVertexCount);
    normals.copyFromBuffer((SVertex*)mNormalsList, mVertexCount);
    mVertexBuffer.addBuffer(normals, ATTRIB_NORMAL);
    
    mIsSkinned = root->isSkinned();
    if (mIsSkinned)
    {
        mWeights = root->getBoneWeights();
        mInfluences = root->getBoneInfluences();
        mTotalFrameCount = root->getFrameCount();
        mBoneList = root->getBoneList();
        mBoneMatrices.create(mBoneList.size());
        
        mSkinnedVertices = new F32[mVertexCount*sizeof(F32)*3];
        mSkinnedNormals = new F32[mVertexCount*sizeof(F32)*3];
        mSkeletonRoot = root->findChildWithName("Skeleton");
        
        // normalize the weights
        for (int i = 0; i < mVertexCount; i++)
        {
            mWeights[i].weights[3] = 1.0f - mWeights[i].weights[0] - mWeights[i].weights[1] - mWeights[i].weights[2];
            if (mWeights[i].weights[3] < -0.0001f)
                printf("Warning: negative weight at index: %d %f\n", i, mWeights[i].weights[3]);
        }
        
        // Validate bone indices
        for (int i = 0; i < mVertexCount; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (mInfluences[i].index[j] >= mBoneList.size())
                    printf("Warning: influence %d out of bounds\n", i);
            }
        }
        
        if (gGPUSkin)
        {
            mSubmitInfluences = new F32[mVertexCount*BONES_PER_VERTEX*4];
            for (int i = 0; i < mVertexCount; i++)
            {
                mSubmitInfluences[4*i] = (F32)mInfluences[i].index[0];
                mSubmitInfluences[4*i+1] = (F32)mInfluences[i].index[1];
                mSubmitInfluences[4*i+2] = (F32)mInfluences[i].index[2];
                mSubmitInfluences[4*i+3] = (F32)mInfluences[i].index[3];
            }
            
            CVertex4Array weights(mVertexCount);
            weights.copyFromBuffer((SVertex4*)mWeights, mVertexCount);
            mVertexBuffer.addBuffer(weights, ATTRIB_WEIGHTS);
            
            CVertex4Array influences(mVertexCount);
            influences.copyFromBuffer((SVertex4*)mSubmitInfluences, mVertexCount);
            mVertexBuffer.addBuffer(influences, ATTRIB_INFLUENCES);
        }
        
        // Load animations and use anim 0 as default
        //mAnimationList = root->findChildWithName("Animations");
        mAnimationList = nullptr;
        if (mAnimationList)
        {
            mActiveAnimation = static_cast<CR3DModelAnimation*>(mAnimationList->getChildAtIndex(0));
            mCurrentFrame = 0;
            mFrameCount = mActiveAnimation->getEndFrame() - mActiveAnimation->getStartFrame();
        }
        else
            mFrameCount = 0;
    }
    
    std::vector<CCHeirarchyNode*> children = root->getChildren();
    for (std::vector<CCHeirarchyNode*>::iterator group = children.begin(); group < children.end(); group++)
    {
        if ((*group)->getName() == "Skeleton")
            continue;
        // Get the material groups
        std::vector<CCHeirarchyNode*> matchildren = (*group)->getChildren();
        for (std::vector<CCHeirarchyNode*>::iterator material = matchildren.begin(); material < matchildren.end(); material++)
        {
            SMaterialGroup matGroup;
            matGroup.triangleCount = static_cast<CR3DModelMaterialGroup*>((*material))->getTriangleCount();
            matGroup.indices = (U16*)static_cast<CR3DModelMaterialGroup*>((*material))->getVertIndicies();
            matGroup.material = static_cast<CRMaterialResource*>((static_cast<CR3DModelMaterialGroup*>((*material))->getMaterialData()->getResource()));
            addMatGroup(matGroup);
        }
    }
    
    mVertexBuffer.activate();
}

void CGM3DTestRenderer::setWorldMatrix(const CMMatrix4x4 &mat)
{
    mWorldMatrix = mat;
}

void CGM3DTestRenderer::setAlpha(F32 alpha)
{
    mAlpha = alpha;
}

void CGM3DTestRenderer::addMatGroup(SMaterialGroup group)
{
    // See if we share the material with something else. If so, merge
    for (std::list<SMaterialGroup>::iterator it = mMatGroups.begin(); it != mMatGroups.end(); it++)
    {
        if (group.material == (*it).material)
        {
            // Merge the two
            U32 newTriCount = group.triangleCount + (*it).triangleCount;
            U16 *newIndices = new U16[newTriCount*3];
            memcpy(newIndices, (*it).indices, (*it).triangleCount*sizeof(U16)*3);
            memcpy(&newIndices[(*it).triangleCount*3], group.indices, group.triangleCount*sizeof(U16)*3);
            (*it).indices = newIndices;
            (*it).triangleCount = newTriCount;
            return;
        }
    }
    
    // Calculate the center
    F32 minX = mVertexList[group.indices[0]*3];
    F32 minY = mVertexList[group.indices[0]*3+1];
    F32 maxX = mVertexList[group.indices[0]*3];
    F32 maxY = mVertexList[group.indices[0]*3+1];
    for (int i = 0; i < group.triangleCount*3; i++)
    {
        if (mVertexList[group.indices[i]*3] < minX)
            minX = mVertexList[group.indices[i]*3];
        if (mVertexList[group.indices[i]*3] > maxX)
            maxX = mVertexList[group.indices[i]*3];
        if (mVertexList[group.indices[i]*3+1] < minY)
            minY = mVertexList[group.indices[i]*3+1];
        if (mVertexList[group.indices[i]*3+1] > maxY)
            maxY = mVertexList[group.indices[i]*3+1];
    }
    
    group.center.setX((minX+maxX)/2);
    group.center.setY((minY+maxY)/2);
    
    //printf("CenterX:%f\nCenterY:%f\n", group.center.getX(), group.center.getY());
    
    group.area = NULL;
    
    mMatGroups.push_back(group);
}

void CGM3DTestRenderer::assignAreas(CCHeirarchyNode *areas)
{
    std::list<SMaterialGroup>::iterator it;
    for (it = mMatGroups.begin(); it != mMatGroups.end(); it++)
    {
        // Get all the areas and do an intersection test
        std::vector<CCHeirarchyNode*>::iterator areaIt;
        std::vector<CCHeirarchyNode*> areaList = areas->getChildren();
        for (areaIt = areaList.begin(); areaIt < areaList.end(); areaIt++)
        {
            CRArea *area = static_cast<CRArea*>((*areaIt));
            
            //printf("CenterX:%f\nCenterY:%f\n", (*it).center.getX(), (*it).center.getY());
            //printf("MinX:%f\nMinY:%f\n", area->getMin().getX(), area->getMin().getY());
            //printf("MaxX:%f\nMinY:%f\n", area->getMax().getX(), area->getMax().getY());
            
            // Do the test. If the point is greater than min, but less than max
            if ((*it).center.getX() >= area->getMin().getX() &&
                (*it).center.getY() >= area->getMin().getY() &&
                (*it).center.getX() <= area->getMax().getX() &&
                (*it).center.getY() <= area->getMax().getY())
            {
                (*it).area = area;
                //printf("Passed\n");
                //break;
            }
        }
        //printf("\n");
    }
}

void CGM3DTestRenderer::assignAreasWorld(CCHeirarchyNode *areas)
{
    CMVector4 center = mWorldMatrix.getColumn(3);
    
    std::list<SMaterialGroup>::iterator it;
    for (it = mMatGroups.begin(); it != mMatGroups.end(); it++)
    {
        // Get all the areas and do an intersection test
        std::vector<CCHeirarchyNode*>::iterator areaIt;
        std::vector<CCHeirarchyNode*> areaList = areas->getChildren();
        for (areaIt = areaList.begin(); areaIt < areaList.end(); areaIt++)
        {
            CRArea *area = static_cast<CRArea*>((*areaIt));
            
            //printf("CenterX:%f\nCenterY:%f\n", (*it).center.getX(), (*it).center.getY());
            //printf("MinX:%f\nMinY:%f\n", area->getMin().getX(), area->getMin().getY());
            //printf("MaxX:%f\nMinY:%f\n", area->getMax().getX(), area->getMax().getY());
            
            // Do the test. If the point is greater than min, but less than max
            if (center.getX() >= area->getMin().getX() &&
                center.getY() >= area->getMin().getY() &&
                center.getX() <= area->getMax().getX() &&
                center.getY() <= area->getMax().getY())
            {
                (*it).area = area;
                //printf("Passed\n");
                //break;
            }
        }
        //printf("\n");
    }
}

void CGM3DTestRenderer::assignAreasWorld(std::list<CRArea*> areas)
{
    CMVector4 center = mWorldMatrix.getColumn(3);
    
    std::list<SMaterialGroup>::iterator it;
    for (it = mMatGroups.begin(); it != mMatGroups.end(); it++)
    {
        // Get all the areas and do an intersection test
        std::list<CRArea*>::iterator areaIt;
        for (areaIt = areas.begin(); areaIt != areas.end(); areaIt++)
        {
            CRArea *area = static_cast<CRArea*>((*areaIt));
            
            //printf("CenterX:%f\nCenterY:%f\n", (*it).center.getX(), (*it).center.getY());
            //printf("MinX:%f\nMinY:%f\n", area->getMin().getX(), area->getMin().getY());
            //printf("MaxX:%f\nMinY:%f\n", area->getMax().getX(), area->getMax().getY());
            
            // Do the test. If the point is greater than min, but less than max
            if (center.getX() >= area->getMin().getX() &&
                center.getY() >= area->getMin().getY() &&
                center.getX() <= area->getMax().getX() &&
                center.getY() <= area->getMax().getY())
            {
                (*it).area = area;
                //printf("Passed\n");
                //break;
            }
        }
        //printf("\n");
    }
}

void CGM3DTestRenderer::skin()
{
    if (!mIsSkinned)
        return;
    
    U32 frame = 0;
    if (mActiveAnimation)
        frame = mCurrentFrame + mActiveAnimation->getStartFrame();
    
    
    CMMatrix4x4 identity;
    identity.setIdentity();
    /*for (std::vector<CCHeirarchyNode*>::iterator it = mSkeletonRoot->getChildren().begin(); it != mSkeletonRoot->getChildren().end(); it++)
    {
        static_cast<CR3DModelSkeletonNode*>(*it)->computeTransform(identity, identity, mCurrentFrame);
    }*/
    U32 childrenCount = mSkeletonRoot->getChildren().size();
    for (int i = 0; i < childrenCount; i++)
        static_cast<CR3DModelSkeletonNode*>(mSkeletonRoot->getChildren()[i])->computeTransform(identity, identity, frame);
    
    if (!gGPUSkin)
    {
        for (U32 vert = 0; vert < mVertexCount; vert++)
        {
            memset(&mSkinnedVertices[vert*3], 0, sizeof(F32)*3);
            memset(&mSkinnedNormals[vert*3], 0, sizeof(F32)*3);
            for (int inf = 0; inf < BONES_PER_VERTEX; inf++)
            {
                CR3DModelSkeletonNode *bone = mBoneList[mInfluences[vert].index[inf]];
                CMMatrix4x4 transform = bone->getTransform();
                CMVector4 vertex(mVertexList[vert*3], mVertexList[vert*3+1], mVertexList[vert*3+2], 1.0f);
                CMVector4 normal(mNormalsList[vert*3], mNormalsList[vert*3+1], mNormalsList[vert*3+2], 0.0f);
                transform.multiplyScalar(mWeights[vert].weights[inf]);
                vertex = transform.multiplyVec(vertex);
                normal = transform.multiplyVec(normal);
            
                mSkinnedVertices[vert*3] += vertex.getX();
                mSkinnedVertices[vert*3+1] += vertex.getY();
                mSkinnedVertices[vert*3+2] += vertex.getZ();
                mSkinnedNormals[vert*3] += normal.getX();
                mSkinnedNormals[vert*3+1] += normal.getY();
                mSkinnedNormals[vert*3+2] += normal.getZ();
            }
        }
    }
    else
    {
        U32 i = 0;
        // Build the matrix list to submit to the GPU
        for (std::vector<CR3DModelSkeletonNode*>::iterator it = mBoneList.begin(); it != mBoneList.end(); it++)
        {
            mBoneMatrices[i] = (*it)->getTransform();
            i++;
        }
    }
}

void CGM3DTestRenderer::setReflectionEnabled(bool refl)
{
    mReflecting = refl;
}

bool CGM3DTestRenderer::isReflectionEnabled() const
{
    return mReflecting;
}

void CGM3DTestRenderer::updateReflection()
{
    if (!mReflecting || mReflectionSize == 0)
        return;
    
    if (!mReflectionFramebufferAllocated)
        _allocateReflectionFramebuffer();
    
    // Activate our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, mReflectionFramebuffer);
    
    // Store the current view and projection matrices because we will reset them
    CMMatrix4x4 sceneMV = gGlobalSceneManager->getViewMatrix();
    CMMatrix4x4 sceneProj = gGlobalSceneManager->getProjectionMatrix();
    
    // Set the projection matrix in the scene manager
    gGlobalSceneManager->setProjectionMatrix(mReflectionProjectionMatrix);
    
    CMMatrix4x4 viewMatrix;
    CMVector4 position4 = mWorldMatrix.getColumn(3);
    CMVector3 position(position4.getX(), position4.getY(), position4.getZ());
    
    // Render all six sides of the cube for the reflection map
    for (GLenum i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i < GL_TEXTURE_CUBE_MAP_POSITIVE_X + 6; i++)
    {
        switch (i)
        {
            case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
                viewMatrix.lookAtMatrix(position, CMVector3(position.getX()+1, position.getY(), position.getZ()));
                break;
                
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
                viewMatrix.lookAtMatrix(position, CMVector3(position.getX()-1, position.getY(), position.getZ()));
                break;
                
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
                viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY()+1, position.getZ()), CMVector3( 0.0f, 0.0f,-1.0f ));
                break;
                
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
                viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY()-1, position.getZ()), CMVector3( 0.0f, 0.0f,1.0f ));
                break;
                
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
                viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY(), position.getZ()+1));
                break;
                
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
                viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY(), position.getZ()-1));
                break;
        }
        
        // Attach the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, i, mReflectionTexture, 0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //CMMatrix4x4 rotMat;
        //rotMat.rotate(MPI/4, 1, 0, 0);
        //viewMatrix.multiply(rotMat);
        
        // Set the view matrix and render
        //viewMatrix.inverse();
        gGlobalSceneManager->setViewMatrix(viewMatrix);
        gGlobalSceneManager->renderScene();
    }
    
    // Now create the mipmaps for the cubemap
    //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    // Restore the original matrices
    gGlobalSceneManager->setProjectionMatrix(sceneProj);
    gGlobalSceneManager->setViewMatrix(sceneMV);
}

void CGM3DTestRenderer::render()
{
    glEnable(GL_DEPTH_TEST);
    if (mIsSkinned)
        skin();
    std::list<SMaterialGroup>::iterator it;
    for (it = mMatGroups.begin(); it != mMatGroups.end(); it++)
    {
        SMaterialGroup g = *it;
        if (g.area->getVisible())
        {
            g.material->applyMaterial(mWorldMatrix, g.area, mBoneMatrices);
            /*if (mIsSkinned)
            {
                if (gGPUSkin)
                {
                    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, mVertexList);
                    glEnableVertexAttribArray(ATTRIB_VERTEX);
                    glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, 0, 0, mUVList);
                    glEnableVertexAttribArray(ATTRIB_UV);
                    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, 0, 0, mNormalsList);
                    glEnableVertexAttribArray(ATTRIB_NORMAL);
                    glVertexAttribPointer(ATTRIB_WEIGHTS, 4, GL_FLOAT, 0, 0, mWeights);
                    glEnableVertexAttribArray(ATTRIB_WEIGHTS);
                    glVertexAttribPointer(ATTRIB_INFLUENCES, 4, GL_FLOAT, 0, 0, mSubmitInfluences);
                    glEnableVertexAttribArray(ATTRIB_INFLUENCES);
                }
                else
                {
                    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, mSkinnedVertices);
                    glEnableVertexAttribArray(ATTRIB_VERTEX);
                    glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, 0, 0, mUVList);
                    glEnableVertexAttribArray(ATTRIB_UV);
                    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, 0, 0, mSkinnedNormals);
                    glEnableVertexAttribArray(ATTRIB_NORMAL);
                }
            }
            else
            {
                glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, mVertexList);
                glEnableVertexAttribArray(ATTRIB_VERTEX);
                glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, 0, 0, mUVList);
                glEnableVertexAttribArray(ATTRIB_UV);
                glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, 0, 0, mNormalsList);
                glEnableVertexAttribArray(ATTRIB_NORMAL);
            }*/
            mVertexBuffer.bind();
            glDrawElements(GL_TRIANGLES, g.triangleCount*3, GL_UNSIGNED_SHORT, g.indices);
            mVertexBuffer.unbind();
            /*glDisableVertexAttribArray(ATTRIB_VERTEX);
            glDisableVertexAttribArray(ATTRIB_UV);
            glDisableVertexAttribArray(ATTRIB_NORMAL);
            glDisableVertexAttribArray(ATTRIB_WEIGHTS);
            glDisableVertexAttribArray(ATTRIB_INFLUENCES);*/
        }
    }
}
