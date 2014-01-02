/*
 *
 * FILE: HeirarchyNode.h
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

#import <Cocoa/Cocoa.h>
#import "FileStream.h"

@interface CHeirarchyNode : NSObject 
{
	// The name of the node.
	NSString *name;
    // The ID number for this node. It starts at zero and increments for each new node added to the parent
	NSUInteger ID;
	// The depth of the node in the tree.
	NSUInteger depth;
	// This node's children.
	NSMutableArray *mChildren;
	// Our Momma.
	CHeirarchyNode *mParent;
	// The type of the node, set by a subclass. This value must be set by the subclass' init before calling
	// [super init].
	NSString *mNodeType;
}

- (id)init;
- (void)dealloc;

- (void)insertChild:(CHeirarchyNode*)child;
- (CHeirarchyNode*)getChildAtIndex:(NSUInteger)index;
- (NSMutableArray*)getChildren;
- (CHeirarchyNode*)findChildWithName:(NSString*)nodename isRecursive:(BOOL)recursive;
- (NSUInteger)numberOfChildren;
- (NSString*)getNodeType;
- (void)setParent:(CHeirarchyNode*)parent;
- (CHeirarchyNode*)getParent;
- (void)removeChildAtIndex:(NSUInteger) index;
- (void)removeSelf;

- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc;
- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;

@property (readwrite, retain) NSString *name;
@property (readwrite, nonatomic, assign) NSUInteger ID;
@property (readwrite, nonatomic, assign) NSUInteger depth;

@end
