/*
 *
 * FILE: 3DModelResource.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Models
 *
 * PURPOSE: Stores a 3D model.
 *
 * CREATED ON: 07/28/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import "Resource.h"
#import "MaterialResource.h"
#import "Box3D.h"

#define BONES_PER_VERTEX 4

typedef struct VERTEX
{
    float x;
    float y;
    float z;
} SVertex;

typedef struct UV
{
    float u;
    float v;
} SUV;

typedef struct INDEX
{
    // 16-bit indices save a lot of ram and disk space.
    unsigned short i1;
    unsigned short i2;
    unsigned short i3;
} SIndex;

typedef struct BONE_WEIGHT
{
    float weights[BONES_PER_VERTEX];
} SBoneWeight;

typedef struct BONE_INFLUENCE
{
    unsigned int index[BONES_PER_VERTEX];
} SBoneInfluence;

@interface CModelMaterialGroup : CHeirarchyNode 
{
    unsigned short mTriCount;
    SIndex *mVertIndices;
    SIndex *mUVIndices;
    SIndex *mNormalIndices;
    
    CBox3D *mBoundingBox;
    
    CResourceNode *mMaterialData;
    NSString *mMaterialDataString;
}

- (void)setTriangleCount:(unsigned short)count;
- (unsigned short)getTriangleCount;

- (void)setVertIndices:(SIndex*)verts;
- (SIndex*)getVertIndices;

- (void)setUVIndices:(SIndex*)uvs;
- (SIndex*)getUVIndices;

- (void)setNormalIndices:(SIndex*)normals;
- (SIndex*)getNormalIndices;

- (void)setMaterialData:(CResourceNode*)matData;
- (CResourceNode*)getMaterialData;

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc;
- (void)writeToStream:(CFileStream*)stream;

@end

@interface CModelSkinCluster : NSObject
{
@private
    NSArray *mInfluenceTable;
}

- (void)setInfluenceTable:(NSArray*)table;
- (NSArray*)getInfluenceTable;

@end

@interface CModelSkeletonFrame : NSObject
{
@private
    float mTransformMatrix[16];
}

- (void)setTransformMatrix:(float*)matrix;
- (void)getTransformMatrix:(float*)matrix;

- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;

@end

typedef enum
{
    AM_PLAY_ONCE,
    AM_LOOP,
    AM_PINGPONG,
} EAnimationMode;

@interface CModelAnimation : CHeirarchyNode
{
@private
   // NSString *mName;
    unsigned int mStartFrame;
    unsigned int mEndFrame;
    EAnimationMode mAnimMode;
}

//- (void)setName:(NSString*)inname;
//- (NSString*)getName;

- (void)setStartFrame:(unsigned int)frame;
- (unsigned int)getStartFrame;
- (void)setEndFrame:(unsigned int)frame;
- (unsigned int)getEndFrame;

- (void)setAnimationMode:(EAnimationMode)mode;
- (EAnimationMode)getAnimationMode;

- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;


@end

@interface CModelSkeletonNode : CHeirarchyNode 
{
@private
    float mInitialMatrix[16];
    float mBoneTransform[16];
    int mFrameCount;
    int mBoneID;
    NSMutableArray *mFrames;
}

- (void)setInitialMatrix:(float*)matrix;
- (void)getInitialMatrix:(float*)matrix;

- (void)computeTransform:(float*)parentInitial anim:(float*)parentAnim frame:(int)frame;
- (void)getTransform:(float*)matrix;

- (void)setFrameCount:(int)count;
- (int)getFrameCount;

- (void)setBoneID:(int)Id;
- (int)getBoneID;

- (CModelSkeletonNode*)findBoneWithID:(int)bID;

- (void)insertFrame:(CModelSkeletonFrame*)frame index:(int)index;
- (CModelSkeletonFrame*)frameAtIndex:(int)index;

- (void)writeToStream:(CFileStream*)stream;
@end

@interface CModelRoot : CHeirarchyNode 
{
    // Stores all the vertices.
    unsigned int mVertexCount;
    SVertex      *mVertices;
    
    // Stores all the UVs.
    unsigned int mUVCount;
    SUV          *mUVs;
    
    // Stores all the normals.
    unsigned int mNormalCount;
    SVertex      *mNormals;
    
    // Bones, weights, and influnce references
    bool mIsSkinned;
    unsigned int mWeightCount;
    SBoneWeight *mWeights;
    SBoneInfluence *mInfluences;
    NSArray *mBones;
    
    // Storage for deformed vertices and normals
    SVertex *mSkinnedVertices;
    SVertex *mSkinnedNormals;
    
    // Bounding box
    CBox3D *mBoundingBox;
    
    // Animation length
    unsigned int mFrameCount;
}

- (void)setVertexCount:(int)vc;
- (int)getVertexCount;

- (void)setVertices:(SVertex*)v;
- (SVertex*)getVertices;

- (void)setUVCount:(int)uvc;
- (int)getUVCount;

- (void)setUVs:(SUV*)uv;
- (SUV*)getUVs;

- (void)setNormalCount:(int)nc;
- (int)getNormalCount;

- (void)setNormals:(SVertex*)n;
- (SVertex*)getNormals;

- (void)setIsSkinnes:(bool)is;
- (bool)isSkinned;

- (SVertex*)getSkinnedVertices;
- (SVertex*)getSkinnedNormals;

- (void)setWeightCount:(unsigned int)wc;
- (unsigned int)getWeightCount;
- (void)setWeights:(SBoneWeight*)weights;
- (SBoneWeight*)getWeights;
- (void)setInfluences:(SBoneInfluence*)influence;
- (SBoneInfluence*)getInfluences;
- (void)setBones:(NSArray*)bones;
- (NSArray*)getBones;

- (void)computeBoundingBox;

- (void)setFrameCount:(unsigned int)count;
- (unsigned int)getFrameCount;

// Skins the mesh and transforms the vertices and normals for the specified frame
- (void)skin:(float)frame;

// Reindexes all the vertices and indices. This must be called before the model can be rendered properly.
- (void)reindex;

// Save the model to disk using a simple format for testing purposes. In production, it should be saved into the resource heirarchy.
- (void)saveToDisk:(NSString*)location;

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc;
- (void)writeToStream:(CFileStream*)stream;

@end

@interface CModelResource : CResource
{
    CModelRoot *mRoot;
}

- (void)setRoot:(CModelRoot*)root;
- (CModelRoot*)getRoot;
- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*)loc name:(NSString*)name;
- (void)writeToStream:(CFileStream*)stream;

@end
