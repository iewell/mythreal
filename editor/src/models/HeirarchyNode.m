/*
 *
 * FILE: HeirarchyNode.m
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Models
 *
 * PURPOSE: Class for a single hierarchy node.
 *
 * CREATED ON: 07/17/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import "HeirarchyNode.h"
#import "Resource.h"
#import "ResourceListModel.h"
#import "3DModelResource.h"
#import "2DSceneObjects.h"
#import "3DSceneObjectInstance.h"

@implementation CHeirarchyNode

- (id)init;
{
	if (self = [super init])
	{
        name = [[NSString alloc] init];
        mChildren = [[NSMutableArray alloc] initWithCapacity:5];
	
        depth = 0;
        ID = 0;
	
        if (!mNodeType)
            mNodeType = [[NSString alloc] initWithString:@"Generic"];
    }
	
	return self;
}

- (void)dealloc
{
	[name release];
	[mChildren release];
	[mNodeType release];
	[super dealloc];
}

- (void)insertChild:(CHeirarchyNode*) child
{
	[child setID:[mChildren count]];
	[child setDepth: [self depth] + 1];
	[child setParent:self];
	[child retain];
	[mChildren addObject:child];
}

- (CHeirarchyNode*)getChildAtIndex:(NSUInteger)index
{
	return [mChildren objectAtIndex:index];
}

- (NSMutableArray*)getChildren
{
    return mChildren;
}

- (CHeirarchyNode*)findChildWithName:(NSString*)nodename isRecursive:(BOOL)recursive
{
    for (CHeirarchyNode *node in mChildren)
    {
        if ([[node name] compare:nodename] == NSOrderedSame)
            return node;
        else if (recursive && [node numberOfChildren] > 0)
        {
            CHeirarchyNode* node2;
            if ((node2 = [node findChildWithName:nodename isRecursive:TRUE]))
                return node2;
        }
    }
    return nil;
}

- (NSUInteger)numberOfChildren
{
	return [mChildren count];
}

- (NSString*)getNodeType
{
	return mNodeType;
}

- (void)setParent: (CHeirarchyNode*)parent
{
	mParent = parent;
}

- (CHeirarchyNode*)getParent
{
    return mParent;
}

- (void)removeChildAtIndex:(NSUInteger)index
{	
	NSUInteger i;
	for (i = index; i < [mChildren count]; i++)
		[[self getChildAtIndex:i] setID:[[self getChildAtIndex:i] ID] - 1];
    [mChildren removeObjectAtIndex:index];
}

- (void)removeSelf
{
	if (mParent)
	{
		[mParent removeChildAtIndex:ID];
	}
	
	[self release];
}

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc
{
    NSXMLElement *XMLnode = [[NSXMLElement alloc] initWithName:@"node"];
    
    NSXMLNode *nodeName = [NSXMLNode elementWithName:@"name" stringValue:name];
    NSXMLNode *nodeType = [NSXMLNode elementWithName:@"type" stringValue:mNodeType];
    
    [XMLnode addChild:nodeName];
    [XMLnode addChild:nodeType];
    
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
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    //name = [stream readString];
    //mNodeType = [stream readString];
    
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
        else if ([nodeType compare:@"ResourceType"] == NSOrderedSame)
            child = [[CResourceType alloc] init];
        else if ([nodeType compare:@"3DModelRoot"] == NSOrderedSame)
            child = [[CModelRoot alloc] init];
        else if ([nodeType compare:@"MaterialGroup"] == NSOrderedSame)
            child = [[CModelMaterialGroup alloc] init];
        else if ([nodeType compare:@"2DPhysicsLine"] == NSOrderedSame)
            child = [[C2DPhysicsLine alloc] init];
        else if ([nodeType compare:@"2DArea"] == NSOrderedSame)
            child = [[C2DArea alloc] init];
        else if ([nodeType compare:@"3DSceneObjectInstance"] == NSOrderedSame)
            child = [[C3DSceneObjectInstance alloc] init];
        else if ([nodeType compare:@"3DModelSkeletonNode"] == NSOrderedSame)
            child = [[CModelSkeletonNode alloc] init];
        else if ([nodeType compare:@"3DModelAnimation"] == NSOrderedSame)
            child = [[CModelAnimation alloc] init];
        else
            child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

@synthesize name;
@synthesize ID;
@synthesize depth;

@end
