/*
 *
 * FILE: ResourceListModel.m
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Models
 *
 * PURPOSE: Stores a hierarchy of the resource list.
 *
 * CREATED ON: 07/08/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import "ResourceListModel.h"
#import "3DModelResource.h"
#import "ShaderResource.h"
#import "MaterialResource.h"
#import "TextureResource.h"
#import "3DSceneLayoutResource.h"
#import "Resource.h"

@implementation CResourceType

- (id)init;
{
	mNodeType = @"ResourceType";
	self = [super init];
	return self;
}

@end

@implementation CResourceListModel

- (id)init
{
	self = [super init];
	return self;
}

- (void)dealloc
{
	[mRoot release];
	[super dealloc];
}

- (void)setRootNode:(CHeirarchyNode*) root init:(bool)init
{
	if (mRoot)
		[mRoot release];
	
	mRoot = [root retain];
    
    if (init)
    {
        CResourceType *mdls, *textures, *shaders, *materials, *levels3D;
        mdls = [[[CResourceType alloc] init] autorelease];
        textures = [[[CResourceType alloc] init] autorelease];
        shaders = [[[CResourceType alloc] init] autorelease];
        materials = [[[CResourceType alloc] init] autorelease];
        levels3D = [[[CResourceType alloc] init] autorelease];
	
        mdls.name = [[[NSString alloc] initWithUTF8String:"3D Models"] autorelease];
        mdls.ID = RT_3D_MODEL;
	
        textures.name = [[[NSString alloc] initWithUTF8String:"Textures"] autorelease];
        textures.ID = RT_TEXTURE;
    
        shaders.name = [[[NSString alloc] initWithUTF8String:"Shaders"] autorelease];
        shaders.ID = RT_SHADER;
    
        materials.name = [[[NSString alloc] initWithUTF8String:"Materials"] autorelease];
        materials.ID = RT_MATERIAL;
    
        levels3D.name = [[[NSString alloc] initWithUTF8String:"3D Level Layouts"] autorelease];
        levels3D.ID = RT_3D_LEVEL_LAYOUT;
	
        [mRoot insertChild:mdls];
        [mRoot insertChild:textures];
        [mRoot insertChild:shaders];
        [mRoot insertChild:materials];
        [mRoot insertChild:levels3D];
    }
}

- (CHeirarchyNode*)getResourceTypeAtIndex:(NSUInteger)index
{
	return [mRoot getChildAtIndex:index];
}

- (NSUInteger)getNumberOfChildrenAtIndex:(NSUInteger)index
{
	return [[mRoot getChildAtIndex:index] numberOfChildren];
}

- (NSUInteger)getNumberOfResourceTypes
{
	return [mRoot numberOfChildren];
}

- (NSInteger)getIndexOfResourceType:(CHeirarchyNode*) type
{
	if ([[type getNodeType] compare:@"ResourceType"] == NSOrderedSame)
		return [type ID];
	else
		return -1;
}

- (CHeirarchyNode *)getChild:(NSUInteger)childID fromResourceID:(NSUInteger)resourceID
{
	return [(CHeirarchyNode *)[mRoot getChildAtIndex:resourceID] getChildAtIndex:childID];
}

- (BOOL)addResourceOfType:(NSUInteger)type withName:(NSString*)name
{
	NSInteger i, j;
	for (i = 0; i < [mRoot numberOfChildren]; i++)
	{
		for (j = 0; j < [[mRoot getChildAtIndex:i] numberOfChildren]; j++)
		{
			NSString *str = [[[mRoot getChildAtIndex:i] getChildAtIndex:j] name];
			if ([str compare:name] == NSOrderedSame)
				return NO;
		}
			 
	}
	CResourceNode *node = [[CResourceNode alloc] init];
	[node setName:name];
    
    switch (type)
    {
        case RT_3D_MODEL:
            [node setResource:[[[CModelResource alloc] init] autorelease]];
            break;
        case RT_TEXTURE:
            [node setResource:[[[CTextureResource alloc] init] autorelease]];
            break;
        case RT_SHADER:
            [node setResource:[[[CShaderResource alloc] init] autorelease]];
            break;
        case RT_MATERIAL:
            [node setResource:[[[CMaterialResource alloc] init] autorelease]];
            break;
        case RT_3D_LEVEL_LAYOUT:
            [node setResource:[[[C3DSceneLayoutResource alloc] init] autorelease]];
            break;
        default:
            break;
    }
	
	[[mRoot getChildAtIndex:type] insertChild:node];
	[node release];
	return YES;
}

@end
