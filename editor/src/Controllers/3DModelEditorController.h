/*
 *
 * FILE: ResourceEditController.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Controllers
 *
 * PURPOSE: Controls the 3D model editor.
 *
 * CREATED ON: 07/24/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import "3DModelView.h"
#import "3DModelResource.h"

@interface C3DModelOutlineController : NSObject 
{
@private
    CHeirarchyNode *mSceneRoot;
}

- (void)setSceneRoot:(CHeirarchyNode*)root;

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item;
- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item;
- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item;
- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;

@end

@interface C3DModelAnimationListController : NSObject
{
    CHeirarchyNode *mSceneRoot;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView;
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;

@end

@interface CModelEditorController : NSWindowController 
{
    IBOutlet CModelView *mModelView;
    IBOutlet NSOutlineView *mHeirarchy;
    IBOutlet C3DModelOutlineController *mHeirarchyController;
    
    IBOutlet NSTableView *mAnimationList;
    IBOutlet C3DModelAnimationListController *mAnimationListController;
    
    IBOutlet NSTextField *mObjParsingLabel;
    IBOutlet NSProgressIndicator *mObjParsingIndicator;
    
    IBOutlet NSSlider *mAnimationSlider;
    
    CModelResource *mResource;

    NSSlider *sliderChanged;
}

- (void)windowCreated;

- (IBAction)importObjPressed:(id)sender;
- (IBAction)importMayaPressed:(id)sender;
- (IBAction)savePressed:(id)sender;
- (IBAction)frameChanged:(id)sender;

- (IBAction)createAnimation:(id)sender;

- (IBAction)wireframeCheckboxToggled:(NSButton *)sender;

- (void)setResource:(CModelResource*)resource;
- (CModelResource*)getResource;

@end
