/*
 *
 * FILE: 3DModelResource.m
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Models
 *
 * PURPOSE: Stores and saves a 3D model.
 *
 * CREATED ON: 07/28/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import "3DModelResource.h"
#import "FileStream.h"
#import "ResourceListModel.h"
#import <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <Eigen/Dense>
using namespace Eigen;

extern CResourceListModel *gModel;

// Endian swap function pulled from here: http://www.efg2.com/Lab/OtherProjects/Endian.htm
void swap(char *x, int size)
{
    unsigned char c;
    unsigned short s;
    unsigned long l;
    
    switch (size)
    {
        case 2: /* swap two bytes */
            c = *x;
            *x = *(x+1);
            *(x+1) = c;
            break;
        case 4: /* swap two shorts (2-byte words) */
            s = *(unsigned short *)x;
            *(unsigned short *)x = *((unsigned short *)x + 1);
            *((unsigned short *)x + 1) = s;
            swap ((char *)x, 2);
            swap ((char *)((unsigned short *)x+1), 2);
            break;
        case 8: /* swap two longs (4-bytes words) */
            l = *(unsigned long *)x;
            *(unsigned long *)x = *((unsigned long *)x + 1);
            *((unsigned long *)x + 1) = l;
            swap ((char *)x, 4);
            swap ((char *)((unsigned long *)x+1), 4);
            break;
    }
}

// Does a write while swapping endian.
void endianWrite(void *data, long length, int endianChunkSize, FILE *fp)
{
    // Copy the data to endian swap it.
    char *copy = (char*)malloc(length);
    memcpy(copy, data, length);
    
    // Loop through and endian swap the data.
    int i = 0;
    for (i = 0; i < length; i += endianChunkSize)
    {
        //swap(&copy[i], endianChunkSize);
    }
    
    // Write the data.
    fwrite(copy, length, 1, fp);
    free(copy);
}

@implementation CModelMaterialGroup

- (id)init
{
    mNodeType = @"MaterialGroup";
    if (self = [super init])
    {
        mTriCount = 0;
        mVertIndices = NULL;
        mUVIndices = NULL;
        mNormalIndices = NULL;
        
        mMaterialData = nil;
        mMaterialDataString = nil;
    }
    return self;
}

- (void)setTriangleCount:(unsigned short)count
{
    mTriCount = count;
}

- (unsigned short)getTriangleCount
{
    return mTriCount;
}

- (void)setVertIndices:(SIndex*)verts
{
    if (mVertIndices)
        free(mVertIndices);
    mVertIndices = verts;
}

- (SIndex*)getVertIndices
{
    return mVertIndices;
}

- (void)setUVIndices:(SIndex*)uvs
{
    if (mUVIndices)
        free(mUVIndices);
    mUVIndices = uvs;
}

- (SIndex*)getUVIndices
{
    return mUVIndices;
}

- (void)setNormalIndices:(SIndex*)normals
{
    if (mNormalIndices)
        free(mNormalIndices);
    mNormalIndices = normals;
}

- (SIndex*)getNormalIndices
{
    return mNormalIndices;
}

- (void)setMaterialData:(CResourceNode*)matData
{
    if (mMaterialData)
        [mMaterialData release];
    mMaterialData = [matData retain];
}

- (CResourceNode*)getMaterialData
{
    // If we have a string but not the actual material data, find it
    if (mMaterialDataString != nil && mMaterialData == nil)
        mMaterialData = [(CResourceNode*)[[gModel getResourceTypeAtIndex:RT_MATERIAL] findChildWithName:mMaterialDataString isRecursive:FALSE] retain];
    return mMaterialData;
}

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc
{
    NSXMLElement *XMLnode = [[NSXMLElement alloc] initWithName:@"node"];
    
    NSXMLNode *nodeName = [NSXMLNode elementWithName:@"name" stringValue:name];
    NSXMLNode *nodeType = [NSXMLNode elementWithName:@"type" stringValue:mNodeType];
    NSXMLNode *triCount = [NSXMLNode elementWithName:@"triCount" stringValue:[NSString stringWithFormat:@"%hu", mTriCount]];
    
    [XMLnode addChild:nodeName];
    [XMLnode addChild:nodeType];
    [XMLnode addChild:triCount];
    
    for (CHeirarchyNode *node in mChildren)
        [node serializeXML:XMLnode bundleLocation:loc];
    
    [parent addChild:XMLnode];
    
    mkdir([[NSString stringWithFormat:@"%@/%@", loc, [NSString stringWithFormat:@"%@.%u", name, [mParent ID]]] UTF8String], S_IRWXU | S_IRWXG | S_IRWXO);
    
    NSData *vertIndices = [NSData dataWithBytes:mVertIndices length:sizeof(SIndex)*mTriCount];
    NSData *UVIndices = [NSData dataWithBytes:mUVIndices length:sizeof(SIndex)*mTriCount];
    NSData *normalIndices = [NSData dataWithBytes:mNormalIndices length:sizeof(SIndex)*mTriCount];
    
    [vertIndices writeToFile:[NSString stringWithFormat:@"%@/%@/verts", loc, [NSString stringWithFormat:@"%@.%u", name, [mParent ID]]] atomically:YES];
    [UVIndices writeToFile:[NSString stringWithFormat:@"%@/%@/uvs", loc, [NSString stringWithFormat:@"%@.%u", name, [mParent ID]]] atomically:YES];
    [normalIndices writeToFile:[NSString stringWithFormat:@"%@/%@/normals", loc, [NSString stringWithFormat:@"%@.%u", name, [mParent ID]]] atomically:YES];
    
    [XMLnode release];
}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    [stream writeUShort:mTriCount];
    
    // Write the indices
    [stream writeData:mVertIndices withLength:sizeof(SIndex)*mTriCount];
    [stream writeData:mUVIndices withLength:sizeof(SIndex)*mTriCount];
    [stream writeData:mNormalIndices withLength:sizeof(SIndex)*mTriCount];
    
    // Write out a reference to the material
    [stream writeString:[[self getMaterialData] name]];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    //name = [stream readString];
    //mNodeType = [stream readString];
    
    // Read our indices
    mTriCount = [stream readUShort];
    mVertIndices = (SIndex*)[stream readDataWithLength:sizeof(SIndex)*mTriCount];
    mUVIndices = (SIndex*)[stream readDataWithLength:sizeof(SIndex)*mTriCount];
    mNormalIndices = (SIndex*)[stream readDataWithLength:sizeof(SIndex)*mTriCount];
    
    // Don't try to find the material yet. It may not be loaded yet
    mMaterialDataString = [[stream readString] retain];
    
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        if ([nodeType compare:@"Resource"] == NSOrderedSame)
            child = [[CResourceNode alloc] init];
        else if ([nodeType compare:@"3DModelRoot"] == NSOrderedSame)
            child = [[CModelRoot alloc] init];
        else if ([nodeType compare:@"MaterialGroup"] == NSOrderedSame)
            child = [[CModelRoot alloc] init];
        else
            child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}


@end

@implementation CModelSkinCluster

- (id)init
{
    if (self = [super init])
    {
        mInfluenceTable = NULL;
    }
    return self;
}

- (void)setInfluenceTable:(NSArray *)table
{
    if (mInfluenceTable)
        [mInfluenceTable release];
    mInfluenceTable = [NSArray arrayWithArray:table];
}

- (NSArray*)getInfluenceTable
{
    return mInfluenceTable;
}

@end

@implementation CModelSkeletonFrame

- (id)init
{
    if (self = [super init])
        return self;
    return NULL;
}

- (void)setTransformMatrix:(float *)matrix
{
    memcpy(mTransformMatrix, matrix, sizeof(float)*16);
}

- (void)getTransformMatrix:(float *)matrix
{
    memcpy(matrix, mTransformMatrix, sizeof(float)*16);
}

- (void)writeToStream:(CFileStream*)stream;
{
    [stream writeData:mTransformMatrix withLength:sizeof(float)*16];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [stream readDataIntoBuffer:mTransformMatrix withLenght:sizeof(float)*16];
}

@end

@implementation CModelAnimation

- (id)init
{
    mNodeType = @"3DModelAnimation";
    if (self = [super init])
    {
        
    }
    return self;
}

- (void)setStartFrame:(unsigned int)frame
{
    mStartFrame = frame;
}

- (unsigned int)getStartFrame
{
    return mStartFrame;
}

- (void)setEndFrame:(unsigned int)frame
{
    mEndFrame = frame;
}

- (unsigned int)getEndFrame
{
    return mEndFrame;
}

- (void)setAnimationMode:(EAnimationMode)mode
{
    mAnimMode = mode;
}

- (EAnimationMode)getAnimationMode
{
    return mAnimMode;
}

- (void)writeToStream:(CFileStream *)stream
{
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    [stream writeUInt:mStartFrame];
    [stream writeUInt:mEndFrame];
    [stream writeUInt:mAnimMode];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream *)stream
{
    mStartFrame = [stream readUInt];
    mEndFrame = [stream readUInt];
    mAnimMode = (EAnimationMode)[stream readUInt];
       
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

@end

@implementation CModelSkeletonNode

- (id)init
{
    mNodeType = @"3DModelSkeletonNode";
    self = [super init];
    mFrames = NULL;
    mBoneID = -1;
    return self;
}

- (void)getInitialMatrix:(float*)matrix
{
    memcpy(matrix, mInitialMatrix, sizeof(float)*16);
}

- (void)setInitialMatrix:(float*)matrix
{
    memcpy(mInitialMatrix, matrix, sizeof(float)*16);
}

- (void)computeTransform:(float*)parentInitial anim:(float*)parentAnim frame:(int)frame
{
    Matrix4f parentInitialT;
    if (parentInitial != NULL)
        memcpy(parentInitialT.data(), parentInitial, sizeof(float)*16);
    else
        parentInitialT.setIdentity();
    
    Matrix4f parentAnimT;
    if (parentAnim != NULL)
        memcpy(parentAnimT.data(), parentAnim, sizeof(float)*16);
    else
        parentAnimT.setIdentity();
    
    Matrix4f initialMatrix;
    memcpy(initialMatrix.data(), mInitialMatrix, sizeof(float)*16);
    initialMatrix = parentInitialT*initialMatrix;
    Matrix4f initialMatrixInv = initialMatrix.inverse();
    
    Matrix4f frameTransform;
    float ft[16];
    [(CModelSkeletonFrame*)[mFrames objectAtIndex:frame] getTransformMatrix:ft];
    memcpy(frameTransform.data(), ft, sizeof(float)*16);
    frameTransform = parentAnimT*frameTransform;
    
    Matrix4f newTrans = frameTransform*initialMatrixInv;
    memcpy(mBoneTransform, newTrans.data(), sizeof(float)*16);
    
    for (CModelSkeletonNode *node in mChildren)
        [node computeTransform:initialMatrix.data() anim:frameTransform.data() frame:frame];
}

- (void)getTransform:(float *)matrix
{
    memcpy(matrix, mBoneTransform, sizeof(float)*16);
}

- (void)setFrameCount:(int)count
{
    mFrameCount = count;
    if (!mFrames)
        mFrames = [[NSMutableArray arrayWithCapacity:mFrameCount] retain];
}

- (int)getFrameCount
{
    return mFrameCount;
}

- (void)setBoneID:(int)Id
{
    mBoneID = Id;
}

- (int)getBoneID
{
    return mBoneID;
}

- (CModelSkeletonNode*)findBoneWithID:(int)bID
{
    if (mBoneID == bID)
        return self;
    for (CModelSkeletonNode *node in mChildren)
    {
        CModelSkeletonNode *bone = [node findBoneWithID:bID];
        if (bone)
            return bone;
    }
    return NULL;
}

- (void)insertFrame:(CModelSkeletonFrame *)frame index:(int)index
{
    if (!mFrames || !frame || index >= mFrameCount)
        return;
    //[mFrames insertObject:frame atIndex:index];
    [frame retain];
    [mFrames addObject:frame];
}

- (CModelSkeletonFrame*)frameAtIndex:(int)index
{
    if (index >= mFrameCount)
        return NULL;
    return [mFrames objectAtIndex:index];
}

- (void)writeToStream:(CFileStream *)stream
{
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    [stream writeData:mInitialMatrix withLength:sizeof(float)*16];
    [stream writeUInt:mFrameCount];
    [stream writeUInt:mBoneID];
    
    for (CModelSkeletonFrame *frame in mFrames)
        [frame writeToStream:stream];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [stream readDataIntoBuffer:mInitialMatrix withLenght:sizeof(float)*16];
    mFrameCount = [stream readUInt];
    mBoneID = [stream readUInt];
    
    mFrames = [[NSMutableArray arrayWithCapacity:mFrameCount] retain];
    for (int i = 0; i < mFrameCount; i++)
    {
        CModelSkeletonFrame *frame = [[CModelSkeletonFrame alloc] init];
        [frame readFromStream:stream];
        [mFrames addObject:frame];
    }
    
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        if ([nodeType compare:@"3DModelSkeletonNode"] == NSOrderedSame)
            child = [[CModelSkeletonNode alloc] init];
        else
            child = [[CHeirarchyNode alloc] init];
    
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

@end

@implementation CModelRoot

- (id)init
{
	mNodeType = @"3DModelRoot";
    
    mVertices = NULL;
    mUVs = NULL;
    mNormals = NULL;
    mWeights = NULL;
    mInfluences = NULL;
    mSkinnedVertices = NULL;
    mSkinnedNormals = NULL;
    
	self = [super init];
	return self;
}

- (void)setVertexCount:(int)vc
{
    mVertexCount = vc;
}

- (int)getVertexCount
{
    return mVertexCount;
}

- (void)setVertices:(SVertex*)v
{
    mVertices = v;
}

- (SVertex*)getVertices
{
    return mVertices;
}

- (void)setUVCount:(int)uvc
{
    mUVCount = uvc;
}

- (int)getUVCount
{
    return mUVCount;
}

- (void)setUVs:(SUV*)uv
{
    mUVs = uv;
}

- (SUV*)getUVs
{
    return mUVs;
}

- (void)setNormalCount:(int)nc
{
    mNormalCount = nc;
}

- (int)getNormalCount
{
    return mNormalCount;
}

- (void)setNormals:(SVertex*)n
{
    mNormals = n;
}

- (SVertex*)getNormals
{
    return mNormals;
}

- (void)setIsSkinnes:(bool)is
{
    mIsSkinned = is;
}

- (bool)isSkinned
{
    return mIsSkinned;
}

- (SVertex*)getSkinnedVertices
{
    return mSkinnedVertices;
}

- (SVertex*)getSkinnedNormals
{
    return mSkinnedNormals;
}

- (void)setWeightCount:(unsigned int)wc
{
    mWeightCount = wc;
}

- (unsigned int)getWeightCount
{
    return mWeightCount;
}

- (void)setWeights:(SBoneWeight *)weights
{
    mWeights = weights;
}

- (SBoneWeight*)getWeights
{
    return mWeights;
}

- (void)setInfluences:(SBoneInfluence *)influence
{
    mInfluences = influence;
}

- (SBoneInfluence*)getInfluences
{
    return mInfluences;
}

- (void)setBones:(NSArray *)bones
{
    if (mBones)
        [mBones release];
    mBones = [bones retain];
}

- (NSArray*)getBones
{
    return mBones;
}

- (void)computeBoundingBox
{
    // Compute a bounding box for the vertices
        if (mVertices)
        {
            CVector3 *min = [[CVector3 alloc] initX:mVertices[0].x Y:mVertices[0].y Z:mVertices[0].z];
            CVector3 *max = [[CVector3 alloc] initX:mVertices[0].x Y:mVertices[0].y Z:mVertices[0].z];
            
            for (int i = 0; i < mVertexCount; i++)
            {
                if (mVertices[i].x > [max getX])
                    [max setX:mVertices[i].x];
                if (mVertices[i].x < [min getX])
                    [min setX:mVertices[i].x];
                if (mVertices[i].y > [max getY])
                    [max setY:mVertices[i].y];
                if (mVertices[i].y < [min getY])
                    [min setY:mVertices[i].y];
                if (mVertices[i].z > [max getZ])
                    [max setZ:mVertices[i].z];
                if (mVertices[i].z < [min getZ])
                    [min setZ:mVertices[i].z];
            }
            
            if (mBoundingBox)
                [mBoundingBox release];
            mBoundingBox = [[CBox3D alloc] initWithMin:min max:max];
            [min release];
            [max release];
        }

}

- (void)setFrameCount:(unsigned int)count
{
    mFrameCount = count;
}

- (unsigned int)getFrameCount
{
    return mFrameCount;
}

- (void)skin:(float)frame
{
    if (!mIsSkinned)
        return;
    
    // First, compute all the required transforms for the bones
    CHeirarchyNode *skeleton = [self findChildWithName:@"Skeleton" isRecursive:false];
    for (CModelSkeletonNode *snode in [skeleton getChildren])
        [snode computeTransform:NULL anim:NULL frame:frame];
    
    // If we don't have buffers for the transformed vertices and normals, create them now
    if (!mSkinnedVertices)
        mSkinnedVertices = (SVertex*)malloc(mVertexCount*sizeof(SVertex));
    if (!mSkinnedNormals)
        mSkinnedNormals = (SVertex*)malloc(mNormalCount*sizeof(SVertex));
    
    // Go through every vertex, and using all the influences, calculate the transformations. There must be a faster way to do this but hey if it works, this is not the performance critical application
    for (int vert = 0; vert < mVertexCount; vert++)
    {
        // Initialize the vertex and normal to 0 so we can accumulate transforms
        memset(&mSkinnedVertices[vert], 0, sizeof(SVertex));
        memset(&mSkinnedNormals[vert], 0, sizeof(SVertex));
        
        // Now loop through all the influences in this vertex and transform, weight, and accumulate
        for (int inf = 0; inf < BONES_PER_VERTEX; inf++)
        {
            // Get the bone corresponding to the current influence
            CModelSkeletonNode *bone = [mBones objectAtIndex:mInfluences[vert].index[inf]];
            
            // Load the bone transform into a matrix
            Matrix4f transform;
            [bone getTransform:transform.data()];
            
            // Create a vector for our vertex and normal
            Vector4f vertex, normal;
            vertex << mVertices[vert].x, mVertices[vert].y, mVertices[vert].z, 1.0f;
            normal << mNormals[vert].x, mNormals[vert].y, mNormals[vert].z, 0.0f;
            
            // Transform and weight
            vertex = (transform*mWeights[vert].weights[inf])*vertex;
            normal = (transform*mWeights[vert].weights[inf])*normal;
            
            // Accumulate
            mSkinnedVertices[vert].x += vertex(0);
            mSkinnedVertices[vert].y += vertex(1);
            mSkinnedVertices[vert].z += vertex(2);
            mSkinnedNormals[vert].x += normal(0);
            mSkinnedNormals[vert].y += normal(1);
            mSkinnedNormals[vert].z += normal(2);
        }
        /*if (mInfluences[vert].influenceCount == 0)
        {
            mSkinnedVertices[vert] = mVertices[vert];
            mSkinnedNormals[vert] = mNormals[vert];
        }*/
    }
}

#define CLOSE_ENOUGH(a, b) fabs(a - b) < 0.000001

- (void)reindex
{
    // First, gather up all the material groups.
    NSMutableArray *matGroups = [[NSMutableArray alloc] init];
    for (CHeirarchyNode *group in mChildren)
    {
        for (CModelMaterialGroup *mat in [group getChildren])
        {
            if ([[mat getNodeType] compare:@"MaterialGroup"] == NSOrderedSame)
                [matGroups addObject:mat];
        }
    }
    
    // Get our total triangle count.
    int triCount = 0;
    for (CModelMaterialGroup *mat in matGroups)
        triCount += [mat getTriangleCount];
    
    // Now, allocate our indices, vertices, UVs, and normals enough room for the worst-case senario.
    SIndex* indices[[matGroups count]];
    for (CModelMaterialGroup *mat in matGroups)
        indices[[matGroups indexOfObject:mat]] = (SIndex*)malloc([mat getTriangleCount]*sizeof(SIndex));
    SVertex *vertices = (SVertex*)malloc(triCount*sizeof(SVertex)*3);
    SUV *uvs = (SUV*)malloc(triCount*sizeof(SUV)*3);
    SVertex *normals = (SVertex*)malloc(triCount*sizeof(SVertex)*3);
    
    // Skin stuff allocation
    SBoneWeight *weights = (SBoneWeight*)malloc(triCount*sizeof(SBoneWeight)*3);
    SBoneInfluence *influences = (SBoneInfluence*)malloc(triCount*sizeof(SBoneInfluence)*3);
    
    // Set variables for the number of indices and vertices (the number of uvs and normals will be the same as the number of vertices).
    int numIndices = 0;
    int numVertices = 0;
    
    // Loop through the material groups and process all the indices there.
    for (CModelMaterialGroup *mat in matGroups)
    {
        numIndices = 0;
        int i = 0;
        SVertex vert[3];
        SVertex normal[3];
        SUV uv[3];
        
        SBoneWeight weight[3];
        SBoneInfluence influence[3];
        
        for (i = 0; i < [mat getTriangleCount]; i++)
        {
            memcpy(&vert[0], &mVertices[[mat getVertIndices][i].i1], sizeof(SVertex));
            memcpy(&vert[1], &mVertices[[mat getVertIndices][i].i2], sizeof(SVertex));
            memcpy(&vert[2], &mVertices[[mat getVertIndices][i].i3], sizeof(SVertex));
            memcpy(&uv[0], &mUVs[[mat getUVIndices][i].i1], sizeof(SUV));
            memcpy(&uv[1], &mUVs[[mat getUVIndices][i].i2], sizeof(SUV));
            memcpy(&uv[2], &mUVs[[mat getUVIndices][i].i3], sizeof(SUV));
            memcpy(&normal[0], &mNormals[[mat getNormalIndices][i].i1], sizeof(SVertex));
            memcpy(&normal[1], &mNormals[[mat getNormalIndices][i].i2], sizeof(SVertex));
            memcpy(&normal[2], &mNormals[[mat getNormalIndices][i].i3], sizeof(SVertex));
            
            memcpy(&weight[0], &mWeights[[mat getVertIndices][i].i1], sizeof(SBoneWeight));
            memcpy(&weight[1], &mWeights[[mat getVertIndices][i].i2], sizeof(SBoneWeight));
            memcpy(&weight[2], &mWeights[[mat getVertIndices][i].i3], sizeof(SBoneWeight));
            memcpy(&influence[0], &mInfluences[[mat getVertIndices][i].i1], sizeof(SBoneInfluence));
            memcpy(&influence[1], &mInfluences[[mat getVertIndices][i].i2], sizeof(SBoneInfluence));
            memcpy(&influence[2], &mInfluences[[mat getVertIndices][i].i3], sizeof(SBoneInfluence));
            
            // Loop through and process the triangles.
            int j;
            for (j = 0; j < 3; j++)
            {
                // Loop through all the vertices and try to find a match.
                int k;
                for (k = 0; k < numVertices; k++)
                {
                    // Compare everything.
                    if (CLOSE_ENOUGH(vert[j].x, vertices[k].x) &&
                        CLOSE_ENOUGH(vert[j].y, vertices[k].y) &&
                        CLOSE_ENOUGH(vert[j].z, vertices[k].z) &&
                        
                        CLOSE_ENOUGH(uv[j].u, uvs[k].u) &&
                        CLOSE_ENOUGH(uv[j].v, uvs[k].v) &&
                        
                        CLOSE_ENOUGH(normal[j].x, normals[k].x) &&
                        CLOSE_ENOUGH(normal[j].y, normals[k].y) &&
                        CLOSE_ENOUGH(normal[j].z, normals[k].z))
                    {
                        // Add our matching vertex to the indices.
                        switch (j)
                        {
                            case 0:
                                indices[[matGroups indexOfObject:mat]][numIndices].i1 = k;
                                break;
                            case 1:
                                indices[[matGroups indexOfObject:mat]][numIndices].i2 = k;
                                break;
                            case 2:
                                indices[[matGroups indexOfObject:mat]][numIndices].i3 = k;
                                break;
                        }
                        break;
                    }
                }
                
                // Check if it went through the whole loop without a match.
                if (k == numVertices)
                {
                    // Copy the new vertex, normal, and uv.
                    memcpy(&vertices[numVertices], &vert[j], sizeof(SVertex));
                    memcpy(&uvs[numVertices], &uv[j], sizeof(SUV));
                    memcpy(&normals[numVertices], &normal[j], sizeof(SVertex));
                    
                    // Copy in the weight and influencer
                    memcpy(&weights[numVertices], &weight[j], sizeof(SBoneWeight));
                    memcpy(&influences[numVertices], &influence[j], sizeof(SBoneInfluence));
                    
                    // Add the new index for this.
                    switch (j)
                    {
                        case 0:
                            indices[[matGroups indexOfObject:mat]][numIndices].i1 = numVertices;
                            break;
                        case 1:
                            indices[[matGroups indexOfObject:mat]][numIndices].i2 = numVertices;
                            break;
                        case 2:
                            indices[[matGroups indexOfObject:mat]][numIndices].i3 = numVertices;
                            break;
                    }
                    
                    // Increment the vertex count
                    numVertices++;
                }
            }
            // Increment the indices count.
            numIndices++;
        }
    }
    
    // Free our current storage so we can put the reindexed ones in.
    free(mVertices);
    free(mUVs);
    free(mNormals);
    free(mWeights);
    free(mInfluences);
    
    // Allocate space for our newly indexed stuff
    mVertices = (SVertex*)malloc(numVertices*sizeof(SVertex));
    mUVs = (SUV*)malloc(numVertices*sizeof(SUV));
    mNormals = (SVertex*)malloc(numVertices*sizeof(SVertex));
    mWeights = (SBoneWeight*)malloc(numVertices*sizeof(SBoneWeight));
    mInfluences = (SBoneInfluence*)malloc(numVertices*sizeof(SBoneInfluence));
    
    // Copy them in.
    memcpy(mVertices, vertices, numVertices*sizeof(SVertex));
    memcpy(mUVs, uvs, numVertices*sizeof(SUV));
    memcpy(mNormals, normals, numVertices*sizeof(SVertex));
    memcpy(mWeights, weights, numVertices*sizeof(SBoneWeight));
    memcpy(mInfluences, influences, numVertices*sizeof(SBoneInfluence));
    
    // Set the correct vertex count.
    mVertexCount = numVertices;
    mUVCount = numVertices;
    mNormalCount = numVertices;
    mWeightCount = numVertices;
    
    // Clean up our new indexes.
    free(vertices);
    free(uvs);
    free(normals);
    free(weights);
    free(influences);
    
    // Now to copy in our new indexes.
    for (CModelMaterialGroup *mat in matGroups)
    {
        // The index array is the same size so no need to reallocate.
        memcpy([mat getVertIndices], indices[[matGroups indexOfObject:mat]], [mat getTriangleCount]*sizeof(SIndex));
        memcpy([mat getNormalIndices], indices[[matGroups indexOfObject:mat]], [mat getTriangleCount]*sizeof(SIndex));
        memcpy([mat getUVIndices], indices[[matGroups indexOfObject:mat]], [mat getTriangleCount]*sizeof(SIndex));
        
        // Now free our useless copy.
        free(indices[[matGroups indexOfObject:mat]]);
    }
    
    // Clean up remaining objects.
    [matGroups release];
    
    // recompute bounding box
    [self computeBoundingBox];
}

- (void)saveToDisk:(NSString*)location
{
    // Open the file to save.
    //FILE *fp;
    //fp = fopen([location UTF8String], "wb");
    CFileStream *stream = [[CFileStream alloc] init];
    [stream openFile:location];
    
    // Write the vertex count, uv count, and normal count.
    //endianWrite(&mVertexCount, sizeof(int), sizeof(int), fp);
    //endianWrite(&mUVCount, sizeof(int), sizeof(int), fp);
    //endianWrite(&mNormalCount, sizeof(int), sizeof(int), fp);
    [stream writeUInt:mVertexCount];
    [stream writeUInt:mUVCount];
    [stream writeUInt:mNormalCount];
    
    // Write the vertices, uvs, and normals.
    //endianWrite(mVertices, mVertexCount*sizeof(SVertex), sizeof(float), fp);
    //endianWrite(mUVs, mUVCount*sizeof(SUV), sizeof(float), fp);
    //endianWrite(mNormals, mNormalCount*sizeof(SVertex), sizeof(float), fp);
    [stream writeData:mVertices withLength:mVertexCount*sizeof(SVertex)];
    [stream writeData:mUVs withLength:mVertexCount*sizeof(SUV)];
    [stream writeData:mNormals withLength:mVertexCount*sizeof(SVertex)];
    
    // Write the material groups.
    for (CHeirarchyNode *group in mChildren)
    {
        for (CModelMaterialGroup *mat in [group getChildren])
        {
            int count = [mat getTriangleCount];
            //endianWrite(&count, sizeof(int), sizeof(int), fp);
            //endianWrite([mat getVertIndices], count*sizeof(SIndex), sizeof(short), fp);
            [stream writeUInt:count];
            [stream writeData:[mat getVertIndices] withLength:count*sizeof(SIndex)];
            [[[mat getMaterialData] getResource] writeToStream:stream];
        }
    }
    
    // Mark the end.
    //fwrite("END", sizeof("END"), 1, fp);
    //[stream writeData:"END" withLength:sizeof("END")];
    [stream close];
    //fclose(fp);
}

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc
{
    NSXMLElement *XMLnode = [[NSXMLElement alloc] initWithName:@"node"];
        
    NSXMLNode *nodeName = [NSXMLNode elementWithName:@"name" stringValue:name];
    NSXMLNode *nodeType = [NSXMLNode elementWithName:@"type" stringValue:mNodeType];
    NSXMLNode *vertexCount = [NSXMLNode elementWithName:@"vertexCount" stringValue:[NSString stringWithFormat:@"%u", mVertexCount]];
    NSXMLNode *UVCount = [NSXMLNode elementWithName:@"UVCount" stringValue:[NSString stringWithFormat:@"%u", mUVCount]];
    NSXMLNode *normalCount = [NSXMLNode elementWithName:@"normalCount" stringValue:[NSString stringWithFormat:@"%u", mNormalCount]];
        
    [XMLnode addChild:nodeName];
    [XMLnode addChild:nodeType];
    [XMLnode addChild:vertexCount];
    [XMLnode addChild:UVCount];
    [XMLnode addChild:normalCount];
    
    NSData *verts = [NSData dataWithBytes:mVertices length:mVertexCount*sizeof(SVertex)];
    NSData *uvs = [NSData dataWithBytes:mUVs length:mUVCount*sizeof(SUV)];
    NSData *normals = [NSData dataWithBytes:mNormals length:mNormalCount*sizeof(SVertex)];
    
    [verts writeToFile:[NSString stringWithFormat:@"%@/verts", loc] atomically:TRUE];
    [uvs writeToFile:[NSString stringWithFormat:@"%@/uvs", loc] atomically:TRUE];
    [normals writeToFile:[NSString stringWithFormat:@"%@/normals", loc] atomically:TRUE];
        
    for (CHeirarchyNode *node in mChildren)
        [node serializeXML:XMLnode bundleLocation:loc];
        
    [parent addChild:XMLnode];
    [XMLnode release];
}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    // Write the element counts
    [stream writeUShort:mVertexCount];
    [stream writeUShort:mUVCount];
    [stream writeUShort:mNormalCount];
    
    // Write the vertices and friends out
    [stream writeData:mVertices withLength:mVertexCount*sizeof(SVertex)];
    [stream writeData:mUVs withLength:mUVCount*sizeof(SUV)];
    [stream writeData:mNormals withLength:mNormalCount*sizeof(SVertex)];
    
    // Write out skinning data
    [stream writeByte:mIsSkinned];
    if (mIsSkinned)
    {
        [stream writeUInt:mWeightCount];
        [stream writeData:mWeights withLength:mWeightCount*sizeof(SBoneWeight)];
        [stream writeData:mInfluences withLength:mWeightCount*sizeof(SBoneInfluence)];
        [stream writeUInt:mFrameCount];
    }
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    //name = [stream readString];
    //mNodeType = [stream readString];
    
    mVertexCount = [stream readUShort];
    mUVCount = [stream readUShort];
    mNormalCount = [stream readUShort];
    
    mVertices = (SVertex*)[stream readDataWithLength:mVertexCount*sizeof(SVertex)];
    mUVs = (SUV*)[stream readDataWithLength:mUVCount*sizeof(SUV)];
    mNormals = (SVertex*)[stream readDataWithLength:mNormalCount*sizeof(SVertex)];
    
    mIsSkinned = [stream readByte];
    if (mIsSkinned)
    {
        mWeightCount = [stream readUInt];
        mWeights = (SBoneWeight*)[stream readDataWithLength:mWeightCount*sizeof(SBoneWeight)];
        mInfluences
        = (SBoneInfluence*)[stream readDataWithLength:mWeightCount*sizeof(SBoneInfluence)];
        mFrameCount = [stream readUInt];
    }
    
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        if ([nodeType compare:@"Resource"] == NSOrderedSame)
            child = [[CResourceNode alloc] init];
        else if ([nodeType compare:@"3DModelRoot"] == NSOrderedSame)
            child = [[CModelRoot alloc] init];
        else if ([nodeType compare:@"MaterialGroup"] == NSOrderedSame)
            child = [[CModelMaterialGroup alloc] init];
        else if ([nodeType compare:@"3DModelSkeletonNode"] == NSOrderedSame)
            child = [[CModelSkeletonNode alloc] init];
        else
            child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
    
    // Now we must create our bone table
    int bID = 0;
    NSMutableArray *bonesTable = [[NSMutableArray alloc] init];
    CHeirarchyNode *skltn = [self findChildWithName:@"Skeleton" isRecursive:FALSE];
    while (1)
    {
        CModelSkeletonNode *searchBone;
        for (CModelSkeletonNode *bone in [skltn getChildren])
        {
            searchBone = [bone findBoneWithID:bID];
            if (searchBone != NULL)
                break;
        }
        if (searchBone == NULL)
            break;
        [bonesTable addObject:searchBone];
        bID++;
    }
    mBones = [[NSArray arrayWithArray:bonesTable] retain];
    [bonesTable release];
}

@end

@implementation CModelResource

- (id)init 
{
    if ((self = [super init])) 
    {
        [mType release];
        mType = @"3DModel";
    }
    
    return self;
}

- (void)dealloc 
{
    if (mRoot)
        [mRoot release];
    [super dealloc];
}

- (void)setRoot:(CModelRoot*)root
{
    if (mRoot)
        [mRoot release];
    mRoot = [root retain];
}

- (CModelRoot*)getRoot
{
    return mRoot;
}

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc name:(NSString *)name
{
    NSXMLElement *XMLResource = [[NSXMLElement alloc] initWithName:@"resource"];
    
    NSXMLNode *resourceType = [NSXMLNode elementWithName:@"type" stringValue:mType];
    
    [XMLResource addChild:resourceType];
    [parent addChild:XMLResource];
    
    mkdir([[NSString stringWithFormat:@"%@/models/%@", loc, name] UTF8String], S_IRWXU | S_IRWXG | S_IRWXO);
    
    NSXMLElement *root = (NSXMLElement*)[NSXMLNode elementWithName:@"MythrealResourceXML"];
    NSXMLDocument *doc = [[NSXMLDocument alloc] initWithRootElement:root];
    [doc setVersion:@"1.0"];
    [doc setCharacterEncoding:@"UTF-8"];
    [root addAttribute:[NSXMLNode attributeWithName:@"version" stringValue:@"0"]];
    
    [mRoot serializeXML:root bundleLocation:[NSString stringWithFormat:@"%@/models/%@", loc, name]];
    
    NSData *xmlFile = [doc XMLDataWithOptions:NSXMLNodePrettyPrint];
    [xmlFile writeToFile:[NSString stringWithFormat:@"%@/models/%@/root.xml", loc, name] atomically:YES];
    
    [XMLResource release];
    [doc release];
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeString:mType];
    [mRoot writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    NSString *name = [stream readString];
    
    // Discard the type. We know who this is
    [stream readString];
    
    mRoot = [[CModelRoot alloc] init];
    [mRoot setName:name];
    [mRoot readFromStream:stream];
}

@end
