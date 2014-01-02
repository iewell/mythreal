//
//  ShaderResource.m
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ShaderResource.h"

@implementation CShaderUniform

- (id)init
{
    self = [super init];
    if (self)
    {
        mUniformName = nil;
        mType = UT_FLOAT;
    }
    
    return self;
}

- (void)setUniformName:(NSString*)name
{
    if (mUniformName)
        [mUniformName release];
    mUniformName = [name retain];
}

- (NSString*)getUniformName
{
    return mUniformName;
}

- (void)setUniformType:(EUniformType)type
{
    mType = type;
}

- (EUniformType)getUniformType
{
    return mType;
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeString:mUniformName];
    [stream writeUInt:mType];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    mUniformName = [[stream readString] retain];
    mType = [stream readUInt];
}

@end

@implementation CShaderResource

- (id)init
{
    self = [super init];
    if (self) 
    {
        [mType release];
        mType = @"Shader";
        
        mUniformList = [[NSMutableArray alloc] init];
        mUniformCount = 0;
    }
    
    return self;
}

- (void)addUniformWithName:(NSString*)name ofType:(EUniformType)type
{
    CShaderUniform *uniform = [[CShaderUniform alloc] init];
    [uniform setUniformName:name];
    [uniform setUniformType:type];
    [mUniformList addObject:uniform];
    [uniform release];
    mUniformCount++;
}

- (void)setUniformType:(EUniformType)type atIndex:(NSUInteger)index
{
    [[mUniformList objectAtIndex:index] setUniformType:type];
}

- (void)setUniformName:(NSString*)name atIndex:(NSUInteger)index
{
    [[mUniformList objectAtIndex:index] setUniformName:name];
}

- (CShaderUniform*)getUniformAtIndex:(NSUInteger)index
{
    return [mUniformList objectAtIndex:index];
}

- (void)removeUniformAtIndex:(NSUInteger)index
{
    [mUniformList removeObjectAtIndex:index];
    mUniformCount--;
}

- (NSUInteger)getUniformCount
{
    return mUniformCount;
}

- (NSArray*)getUniformList
{
    return [NSArray arrayWithArray:mUniformList];
}

-(void)setVertexShader:(NSString *)shader
{
    if (mVertexShader)
        [mVertexShader release];
    mVertexShader = [shader retain];
}

-(void)setFragmentShader:(NSString *)shader
{
    if (mFragmentShader)
        [mFragmentShader release];
    mFragmentShader = [shader retain];
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeString:mType];
    [stream writeString:mVertexShader];
    [stream writeString:mFragmentShader];
    
    [stream writeUInt:[mUniformList count]];
    for (CShaderUniform *uniform in mUniformList)
        [uniform writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    if (!mVertexShader)
        mVertexShader = @"";
    if (!mFragmentShader)
        mFragmentShader = @"";
    
    mVertexShader = [[stream readString] retain];
    mFragmentShader = [[stream readString] retain];
    
    unsigned int uniformCount = [stream readUInt];
    int i;
    for (i = 0; i < uniformCount; i++)
    {
        CShaderUniform *uniform = [[CShaderUniform alloc] init];
        [uniform readFromStream:stream];
        [mUniformList addObject:uniform];
    }
    mUniformCount = uniformCount;
}

@end
