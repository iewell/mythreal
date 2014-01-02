/*
 *
 * FILE: 3DSceneLayoutObjectListController.m
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Controllers
 *
 * PURPOSE: Makes the list of available objects in the list
 *
 * CREATED ON: 11/02/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import "3DSceneLayoutObjectListController.h"


@implementation C3DSceneLayoutObjectListController

- (id)init 
{
    if ((self = [super init])) 
    {
        mModel = [mResourceListController mModel];
        
        // For now it will use 3D models. Later it will be objects with more.
        mRoot = [mModel getResourceTypeAtIndex:0];
    }
    
    return self;
}

- (void)dealloc 
{
    // Clean-up code here.
    
    [super dealloc];
}

- (void)refresh
{
    mModel = [mResourceListController mModel];
    mRoot = [mModel getResourceTypeAtIndex:0];
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item
{
    if (!item)
		return 1;
	
	return [item numberOfChildren];
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
    if ([item numberOfChildren] > 0)
		return TRUE;
	return FALSE;
}

- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item
{
    if (!item)
		return mRoot;
	
	return [item getChildAtIndex:index];
}

- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
	if (!item)
		return [[[NSString alloc] initWithUTF8String:"Resource Root"] autorelease];
	return [item name];
	
}

@end
