/*
 *
 * FILE: ResourceListController.m
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Controllers
 *
 * PURPOSE: Controls the list that displays the resources.
 *
 * CREATED ON: 07/08/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import "ResourceListController.h"
#import "HeirarchyNode.h"

CResourceListModel *gModel;

@implementation CResourceListController

@synthesize mModel;
@synthesize mRoot;

- (id)init
{
	if (self = [super init])
	{
        if (mModel)
            [mModel release];
    
        if (mRoot)
            [mRoot release];
    
        gModel = mModel = [[CResourceListModel alloc] init];
        mRoot = [[CHeirarchyNode alloc] init];
        [mRoot setName:@"Resource Root"];
        [mModel setRootNode:mRoot init:TRUE];
	}
    
	return self;
}

- (void)reinit
{
    if (mModel)
        [mModel release];
    
    if (mRoot)
        [mRoot release];
    
	gModel = mModel = [[CResourceListModel alloc] init];
	mRoot = [[CHeirarchyNode alloc] init];
	[mRoot setName:@"Resource Root"];
	[mModel setRootNode:mRoot init:TRUE];
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
