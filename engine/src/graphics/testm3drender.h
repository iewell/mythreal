/*
 *
 * FILE: testm3drender.h
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

#ifndef GRAPHICS_TEST_M3D_RENDER_H
#define GRAPHICS_TEST_M3D_RENDER_H

// Includes:
#include <list>
#include <OpenGLES/ES2/gl.h>
#include "Matrix.h"
#include "MaterialResource.h"
#include "3DModelResource.h"
#include "LevelLayoutResource.h"
#include "VertexBufferGLES.h"
//#include "Material.h"

// A structure holding a material group.
typedef struct MATERIAL_GROUP
{
    unsigned short triangleCount;
    U16 *indices;
    CRMaterialResource *material;
    
    // The area we are assigned to
    CRArea *area;
    
    // The 2D center of this object (for area assignment)
    CMVector2 center;
} SMaterialGroup;

class CGM3DTestRenderer
{
private:
    // The arrays of the vertices, uvs, and normals.
    F32 *mVertexList;
    void *mUVList;
    F32 *mNormalsList;
    
    // Animation stuff
    bool mIsSkinned;
    U32 mCurrentFrame;
    U32 mFrameCount;
    U32 mTotalFrameCount;
    SBoneWeight *mWeights;
    F32 *mSubmitInfluences;
    SBoneInfluence *mInfluences;
    CCHeirarchyNode *mSkeletonRoot;
    CCHeirarchyNode *mAnimationList;
    CR3DModelAnimation *mActiveAnimation;
    std::vector<CR3DModelSkeletonNode*> mBoneList;
    CCSafeArray<CMMatrix4x4> mBoneMatrices;
    
    F32 *mSkinnedVertices;
    F32 *mSkinnedNormals;
    
    // Ugly, but test the new Vertex Buffer
	CGRVertexBufferGLES mVertexBuffer;
    
	// The matrix to transform the object
    CMMatrix4x4 mWorldMatrix;
    
    // The vertex count. The number of vertices and uvs are the same.
    U32 mVertexCount;
    
    // The alpha
    F32 mAlpha;
    
    // The 3D model resource
    CRResourceNode *m3DModel;
    
    // The material groups that hold the indices.
    std::list<SMaterialGroup> mMatGroups;
    
    // Reflection atributes
    U32 mReflectionSize;
    bool mReflecting;
    U32 mReflectionFrameSkip;
    GLuint mReflectionFramebuffer;
    GLuint mRelfectionColorbuffer;
    GLuint mReflectionDepthBuffer;
    GLuint mReflectionTexture;
    
    // Reflection projection matrix stored so that we don't have to regenerate it every frame
    CMMatrix4x4 mReflectionProjectionMatrix;
    
    // Has the reflection framebuffer been allocated
    bool mReflectionFramebufferAllocated;
    
    // This will allocate a framebuffer used to render a dynamic cubemap for reflections
    void _allocateReflectionFramebuffer();
    
public:
    CGM3DTestRenderer();
    ~CGM3DTestRenderer();
    
    void setVertexList(void *vl);
    void setUVList(void *uvl);
    void setNormalList(void *nl);
    void setVertexCount(unsigned int vc);
    
    void set3DModelResource(CRResourceNode *node);
    
    void setWorldMatrix(const CMMatrix4x4 &mat);
    
    void setAlpha(F32 alpha);
    void addMatGroup(SMaterialGroup group);
    void assignAreas(CCHeirarchyNode *areas);
    void assignAreasWorld(CCHeirarchyNode *areas);
    void assignAreasWorld(std::list<CRArea*> areas);
    
    void skin();
    
    void setCurrentFrame(U32 frame)
    {
        if (!mActiveAnimation)
            return;
        mCurrentFrame = frame % mFrameCount;
    };
    
    void setCurrentAnimation(const std::string &name)
    {
        CR3DModelAnimation *anim = static_cast<CR3DModelAnimation*>(mAnimationList->findChildWithName(name));
        if (!anim)
            return;
        mActiveAnimation = anim;
        mCurrentFrame = 0;
        mFrameCount = mActiveAnimation->getEndFrame()-mActiveAnimation->getStartFrame();
    }
    
    void setReflectionEnabled(bool refl);
    bool isReflectionEnabled() const;
    
    // Updates the reflection. This needs to be done in a render prepass or the default framebuffer wont be bound after this
    void updateReflection();
    
    // Render!
    void render();
};

#endif