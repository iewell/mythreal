/*
 *
 * FILE: ResourceListModel.h
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

#import <Cocoa/Cocoa.h>
#import "HeirarchyNode.h"

// Lists the available resource types. the ID or index starts at zero and increments as the next resource
// type is added.
enum
{
	RT_3D_MODEL = 0,
	RT_TEXTURE,
    RT_SHADER,
    RT_MATERIAL,
    RT_3D_LEVEL_LAYOUT,
	RT_NUMBER_RESOURCE_TYPES,
};

// The class for a single resource type node. A resource type is the catagory and format in which a single
// resource conforms to. A resource is any data that is written to and loaded from the disk. It can be a
// 3D model, a texture, a string table, and even exacutable code.
@interface CResourceType : CHeirarchyNode
{
	
}

- (id)init;

@end

@interface CResourceListModel : NSObject 
{
	// The root node.
	CHeirarchyNode *mRoot;
}

// Initializes the model by allocating the arrays and adding the resource types.
- (id)init;

- (void)dealloc;

// Sets the root node.
- (void)setRootNode:(CHeirarchyNode*) root init:(bool)init;
// Takes the index of the resource type and returns the resource node contaning the resources.
- (CHeirarchyNode*)getResourceTypeAtIndex:(NSUInteger)index;

// Takes the index of the resource type and returns how many resources are of that type.
- (NSUInteger)getNumberOfChildrenAtIndex:(NSUInteger)index;

// Returns the number of resource types available.
- (NSUInteger)getNumberOfResourceTypes;

// Takes the "pretty" string and returns the resource type id associated with it, or -1 if it doesn't exist.
- (NSInteger)getIndexOfResourceType:(CHeirarchyNode*) type;

// Returns the resource with the id of "childID" under the resource type specified as "resourceID".
- (CHeirarchyNode *)getChild:(NSUInteger)childID fromResourceID:(NSUInteger)resourceID;

// Adds a new resource (currently a string) of the resource type identified by "type". Returns TRUE 
// on success or FALSE if the name already exists. The resource is assigned the next available resource
// id in the resource type.
- (BOOL)addResourceOfType:(NSUInteger)type withName:(NSString*)name;

@end
