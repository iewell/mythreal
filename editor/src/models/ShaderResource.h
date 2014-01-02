//
//  ShaderResource.h
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Resource.h"
#import "FileStream.h"

typedef enum
{
    UT_FLOAT,
    UT_VECTOR2,
    UT_VECTOR3,
    UT_VECTOR4,
    UT_MATRIX4,
    UT_SAMPLER2D,
    UT_SAMPLER_CUBE,
} EUniformType;

@interface CShaderUniform : NSObject 
{
@private
    NSString *mUniformName;
    EUniformType mType;
}

- (void)setUniformName:(NSString*)name;
- (NSString*)getUniformName;

- (void)setUniformType:(EUniformType)type;
- (EUniformType)getUniformType;

- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;

@end

@interface CShaderResource : CResource
{
    NSMutableArray *mUniformList;
    NSUInteger mUniformCount;
    NSString *mVertexShader;
    NSString *mFragmentShader;
}

- (void)addUniformWithName:(NSString*)name ofType:(EUniformType)type;
- (void)setUniformType:(EUniformType)type atIndex:(NSUInteger)index;
- (void)setUniformName:(NSString*)name atIndex:(NSUInteger)index;
- (CShaderUniform*)getUniformAtIndex:(NSUInteger)index;
- (void)removeUniformAtIndex:(NSUInteger)index;
- (NSUInteger)getUniformCount;
- (NSArray*)getUniformList;

- (void)setVertexShader:(NSString*)shader;
- (void)setFragmentShader:(NSString*)shader;

- (void)writeToStream:(CFileStream*)stream;

@end
