/*
 *
 * FILE: 3DSceneLayoutObjectListController.h
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

#import <Cocoa/Cocoa.h>
#import "ResourceListModel.h"
#import "ResourceListController.h"

@interface C3DSceneLayoutObjectListController : NSObject 
{
@private
    CHeirarchyNode *mRoot;
    CResourceListModel *mModel;
    
    // Very hacky way to get the model. Model needs to be in a more central location.
    IBOutlet CResourceListController *mResourceListController;
}

- (void)refresh;

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item;
- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item;
- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item;
- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;

@end
