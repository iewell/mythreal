//
//  MaterialResource.m
//  Resource Edit
//
//  Created by Ian Ewell on 6/3/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MaterialResource.h"
#import "ResourceListModel.h"
#include <stdlib.h>

extern CResourceListModel *gModel;

@implementation CMaterialUniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mName = nil;
    }
    return self;
}

- (EUniformType)getType
{
    return mType;
}

- (int)getValueType
{
    return mValueType;
}

- (void)setValueType:(int)type
{
    mValueType = type;
}

- (void)setName:(NSString*)name
{
    if (mName)
        [mName release];
    mName = [name retain];
    
    // Because I hate setting repetitive attributes for matrices
    if ([mName compare:@"modelMatrix"] == NSOrderedSame)
        mValueType = VTM4_MODEL;
    if ([mName compare:@"modelViewMatrix"] == NSOrderedSame)
        mValueType = VTM4_MODEL_VIEW;
    if ([mName compare:@"projMatrix"] == NSOrderedSame)
        mValueType = VTM4_PROJECTION;
}

- (NSString*)getName
{
    return mName;
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeUInt:mType];
    [stream writeString:mName];
    [stream writeUInt:mValueType];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    mName = [[stream readString] retain];
    mValueType = [stream readUInt];
}

@end

@implementation CMaterialFloatUniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mType = UT_FLOAT;
    }
    return self;
}

- (void)setFloat:(float)f
{
    mFloat = f;
}

- (float)getFloat
{
    return mFloat;
}

- (void)writeToStream:(CFileStream*)stream
{
    [super writeToStream:stream];
    [stream writeFloat:mFloat];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [super readFromStream:stream];
    mFloat = [stream readFloat];
}

@end

@implementation CMaterialVector2Uniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mType = UT_VECTOR2;
    }
    return self;
}

- (void)setVector:(float*)vector
{
    memcpy(mVector, vector, sizeof(float)*2);
}

- (float*)getVector
{
    return &mVector[0];
}

- (void)writeToStream:(CFileStream*)stream
{
    [super writeToStream:stream];
    [stream writeData:mVector withLength:sizeof(float)*2];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [super readFromStream:stream];
    [stream readDataIntoBuffer:&mVector[0] withLenght:sizeof(float)*2];
}

@end

@implementation CMaterialVector3Uniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mType = UT_VECTOR3;
    }
    return self;
}

- (void)setVector:(float*)vector
{
    memcpy(mVector, vector, sizeof(float)*3);
}

- (float*)getVector
{
    return &mVector[0];
}

- (void)writeToStream:(CFileStream*)stream
{
    [super writeToStream:stream];
    [stream writeData:mVector withLength:sizeof(float)*3];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [super readFromStream:stream];
    [stream readDataIntoBuffer:&mVector[0] withLenght:sizeof(float)*3];
}

@end

@implementation CMaterialVector4Uniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mType = UT_VECTOR4;
    }
    return self;
}

- (void)setVector:(float*)vector
{
    memcpy(mVector, vector, sizeof(float)*4);
}

- (float*)getVector
{
    return &mVector[0];
}

- (void)writeToStream:(CFileStream*)stream
{
    [super writeToStream:stream];
    [stream writeData:mVector withLength:sizeof(float)*4];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [super readFromStream:stream];
    [stream readDataIntoBuffer:&mVector[0] withLenght:sizeof(float)*4];
}

@end

@implementation CMaterialMatrix4Uniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mType = UT_MATRIX4;
    }
    return self;
}

- (void)setMatrix:(float*)matrix
{
    memcpy(mMatrix, matrix, sizeof(float)*16);
}

- (float*)getMatrix
{
    return &mMatrix[0];
}

- (void)writeToStream:(CFileStream*)stream
{
    [super writeToStream:stream];
    [stream writeData:mMatrix withLength:sizeof(float)*16];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [super readFromStream:stream];
    [stream readDataIntoBuffer:&mMatrix[0] withLenght:sizeof(float)*16];
}

@end

@implementation CMaterialSampler2DUniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mType = UT_SAMPLER2D;
        mTexture = nil;
        mTextureResourceName = nil;
    }
    return self;
}

- (void)setTexture:(CResourceNode *)texture
{
    if (mTexture)
        [mTexture release];
    mTexture = [texture retain];
}

- (CResourceNode*)getTexture
{
    if (mTextureResourceName != nil && mTexture == nil)
        mTexture = [(CResourceNode*)[[gModel getResourceTypeAtIndex:RT_TEXTURE] findChildWithName:mTextureResourceName isRecursive:FALSE] retain];
    return mTexture;
}

- (void)writeToStream:(CFileStream*)stream
{
    [super writeToStream:stream];
    [stream writeString:[[self getTexture] name]];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    [super readFromStream:stream];
    mTextureResourceName = [[stream readString] retain];
}

@end

@implementation CMaterialResource

- (id)init
{
    self = [super init];
    if (self) 
    {
        [mType release];
        mType = @"Material";
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)buildUniformListFromShaderResource:(CResourceNode*)resource
{
    if (!mUniformArray)
        mUniformArray = [[NSMutableArray alloc] initWithCapacity:[(CShaderResource*)[resource getResource] getUniformCount]];
    
    if (mShaderResource)
        [mShaderResource release];
    mShaderResource = [resource retain];
    
    [mUniformArray removeAllObjects];
    NSArray* shaderArray = [(CShaderResource*)[resource getResource] getUniformList];
    for (CShaderUniform *uniform in shaderArray)
    {
        CMaterialUniform *matUniform;
        switch ([uniform getUniformType])
        {
            case UT_FLOAT:
                matUniform = [[CMaterialFloatUniform alloc] init];
                [(CMaterialFloatUniform*)matUniform setFloat:0.0f];
                break;
            case UT_VECTOR2:
                matUniform = [[CMaterialVector2Uniform alloc] init];
                float vec2[2] = {0.0f, 0.0f};
                [(CMaterialVector2Uniform*)matUniform setVector:vec2];
                break;
            case UT_VECTOR3:
                matUniform = [[CMaterialVector3Uniform alloc] init];
                float vec3[3] = {0.0f, 0.0f, 0.0f};
                [(CMaterialVector3Uniform*)matUniform setVector:vec3];
                break;
            case UT_VECTOR4:
                matUniform = [[CMaterialVector4Uniform alloc] init];
                float vec4[4] = {0.0f, 0.0f, 0.0f, 1.0f};
                [(CMaterialVector4Uniform*)matUniform setVector:vec4];
                break;
            case UT_MATRIX4:
                matUniform = [[CMaterialMatrix4Uniform alloc] init];
                float mat4[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f};
                [(CMaterialMatrix4Uniform*)matUniform setMatrix:mat4];
                break;
            case UT_SAMPLER2D:
                matUniform = [[CMaterialSampler2DUniform alloc] init];
                [(CMaterialSampler2DUniform*)matUniform setTexture:nil];
                break;
            default:
                break;
        }
        [matUniform setName:[uniform getUniformName]];
        [mUniformArray addObject:matUniform];
    }
}

- (NSUInteger)getUniformCount
{
    if (mUniformArray)
        return [mUniformArray count];
    return 0;
}

- (CMaterialUniform*)getUniformAtIndex:(NSUInteger)index
{
    return [mUniformArray objectAtIndex:index];
}

- (CResourceNode*)getShaderResource
{
    if (mShaderResource == nil && mShaderResourceString != nil)
        mShaderResource = [(CResourceNode*)[[gModel getResourceTypeAtIndex:RT_SHADER] findChildWithName:mShaderResourceString isRecursive:FALSE] retain];
    return mShaderResource;
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeString:mType];
    [stream writeUInt:[mUniformArray count]];
    for (CMaterialUniform *uniform in mUniformArray)
        [uniform writeToStream:stream];
    [stream writeString:[[self getShaderResource] name]];
    //[mShaderResource writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    unsigned int uniformCount = [stream readUInt];
    mUniformArray = [[NSMutableArray alloc] initWithCapacity:uniformCount];
    int i;
    for (i = 0; i < uniformCount; i++)
    {
        CMaterialUniform *uniform;
        unsigned int type = [stream readUInt];
        switch (type)
        {
            case UT_FLOAT:
                uniform = [[CMaterialFloatUniform alloc] init];
                break;
            case UT_VECTOR2:
                uniform = [[CMaterialVector2Uniform alloc] init];
                break;
            case UT_VECTOR3:
                uniform = [[CMaterialVector3Uniform alloc] init];
                break;
            case UT_VECTOR4:
                uniform = [[CMaterialVector4Uniform alloc] init];
                break;
            case UT_MATRIX4:
                uniform = [[CMaterialMatrix4Uniform alloc] init];
                break;
            case UT_SAMPLER2D:
                uniform = [[CMaterialSampler2DUniform alloc] init];
                break;
            default:
                uniform = [[CMaterialUniform alloc] init];
                break;
        }
        [uniform readFromStream:stream];
        [mUniformArray addObject:uniform];
    }
    mShaderResourceString = [[stream readString] retain];
}


@end
