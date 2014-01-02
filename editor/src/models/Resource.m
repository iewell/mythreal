/*
 *
 * FILE: Resource.m
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Models
 *
 * PURPOSE: Generic resource classes.
 *
 * CREATED ON: 07/20/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import "Resource.h"
#import "3DModelResource.h"
#import "TextureResource.h"
#import "ShaderResource.h"
#import "MaterialResource.h"
#import "3DSceneLayoutResource.h"

@implementation CResourceNode

- (id)init
{
	mNodeType = @"Resource";
	mEditor = nil;
	self = [super init];
	return self;
}

- (void)dealloc
{
	[mNodeType release];
	[mEditor release];
	[mResource release];
	[super dealloc];
}

- (void)setResource:(CResource*)resource
{
	mResource = [resource retain];
}

- (CResource*)getResource
{
	return mResource;
}

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc
{
    NSXMLElement *XMLnode = [[NSXMLElement alloc] initWithName:@"node"];
    
    NSXMLNode *nodeName = [NSXMLNode elementWithName:@"name" stringValue:name];
    NSXMLNode *nodeType = [NSXMLNode elementWithName:@"type" stringValue:mNodeType];
    
    [XMLnode addChild:nodeName];
    [XMLnode addChild:nodeType];
    
    [mResource serializeXML:XMLnode bundleLocation:loc name:name];
    
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
    
    // Write the resource
    [mResource writeToStream:stream];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    //name = [stream readString];
    //mNodeType = [stream readString];
    
    // Read the resource type
    NSString *resourcetype = [stream readString];
    CResource *resource;
    if ([resourcetype compare:@"3DModel"] == NSOrderedSame)
        resource = [[CModelResource alloc] init];
    else if ([resourcetype compare:@"Texture"] == NSOrderedSame)
        resource = [[CTextureResource alloc] init];
    else if ([resourcetype compare:@"Shader"] == NSOrderedSame)
        resource = [[CShaderResource alloc] init];
    else if ([resourcetype compare:@"Material"] == NSOrderedSame)
        resource = [[CMaterialResource alloc] init];
    else if ([resourcetype compare:@"3DSceneLayout"] == NSOrderedSame)
        resource = [[C3DSceneLayoutResource alloc] init];
    else
        resource = [[CResource alloc] init];
    
    [resource readFromStream:stream];
    mResource = resource;
    
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
        else
            child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

@end


@implementation CResource

- (id)init
{
    if (self = [super init])
        mType = @"NULLResource";
    return self;
}

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc name:(NSString*)name
{
    NSXMLElement *XMLResource = [[NSXMLElement alloc] initWithName:@"resource"];
    NSXMLNode *resourceType = [NSXMLNode elementWithName:@"type" stringValue:mType];
    [XMLResource addChild:resourceType];
    [parent addChild:XMLResource];
    [XMLResource release];
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeString:mType];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    
}

@end
