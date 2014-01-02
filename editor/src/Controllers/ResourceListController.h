/*
 *
 * FILE: ResourceListController.h
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

#import <Cocoa/Cocoa.h>
#import "ResourceListModel.h"


@interface CResourceListController : NSObject 
{
	CResourceListModel *mModel;
	CHeirarchyNode *mRoot;
}

- (id)init;
- (void)reinit;

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item;
- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item;
- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item;
- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;

@property (readonly, retain, nonatomic) CResourceListModel *mModel;
@property (readwrite, retain, nonatomic) CHeirarchyNode *mRoot;

@end
