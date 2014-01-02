//
//  MaterialResource.h
//  Resource Edit
//
//  Created by Ian Ewell on 6/3/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Resource.h"
#import "ShaderResource.h"
#import "TextureResource.h"

typedef enum
{
    VTF_CUSTOM = 0,
    VTF_DT,
    VTF_LIGHT_COUNT,
    VTF_LIGHT_INTENSITY,
} EFloatValueType;

typedef enum
{
    VTV2_CUSTOM = 0,
} EVector2ValueType;

typedef enum
{
    VTV3_CUSTOM = 0,
    VTV3_EYE_POS,
    VTV3_LIGHTPOS,
    VTV3_LIGHT_COLOR,
} EVector3ValueType;

typedef enum
{
    VTV4_CUSTOM = 0,
} EVector4ValueType;

typedef enum
{
    VTM4_CUSTOM = 0,
    VTM4_MODEL_VIEW,
    VTM4_MODEL,
    VTM4_VIEW,
    VTM4_PROJECTION,
    VTM4_BONES,
} EMatrix4ValueType;

@interface CMaterialUniform : NSObject 
{
@protected
    NSString *mName;
    EUniformType mType;
    int mValueType;
}

- (EUniformType)getType;

- (int)getValueType;
- (void)setValueType:(int)type;

- (void)setName:(NSString*)name;
- (NSString*)getName;

- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;

@end

@interface CMaterialFloatUniform : CMaterialUniform 
{
@private
    float mFloat;
}

- (void)setFloat:(float)f;
- (float)getFloat;

@end

@interface CMaterialVector2Uniform : CMaterialUniform 
{
@private
    float mVector[2];
}

- (void)setVector:(float*)vector;
- (float*)getVector;

@end

@interface CMaterialVector3Uniform : CMaterialUniform 
{
@private
    float mVector[3];
}

- (void)setVector:(float*)vector;
- (float*)getVector;

@end

@interface CMaterialVector4Uniform : CMaterialUniform 
{
@private
    float mVector[4];
}

- (void)setVector:(float*)vector;
- (float*)getVector;

@end

@interface CMaterialMatrix4Uniform : CMaterialUniform 
{
@private
    float mMatrix[16];
}

- (void)setMatrix:(float*)matrix;
- (float*)getMatrix;

@end

@interface CMaterialSampler2DUniform : CMaterialUniform 
{
@private
    CResourceNode *mTexture;
    NSString *mTextureResourceName;
}

- (void)setTexture:(CResourceNode*)texture;
- (CResourceNode*)getTexture;

@end

@interface CMaterialResource : CResource 
{
@private
    NSMutableArray *mUniformArray;
    CResourceNode *mShaderResource;
    NSString *mShaderResourceString;
}

- (void)buildUniformListFromShaderResource:(CResourceNode*)resource;
- (NSUInteger)getUniformCount;
- (CMaterialUniform*)getUniformAtIndex:(NSUInteger)index;
- (CResourceNode*)getShaderResource;

- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;

@end
