/*
 *
 * FILE: 3DLayoutEditorController.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Controllers
 *
 * PURPOSE: Controls the 3D Level Layout editor
 *
 * CREATED ON: 11/02/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import "3DSceneLayoutObjectListController.h"
#import "3DModelResource.h"
#import "3DSceneObjectInstance.h"
#import "3DSceneEditorView.h"
#import "2DSceneObjects.h"
#import "3DSceneLayoutResource.h"

typedef enum
{
    TOOL_NONE = 0,
    TOOL_TRANSLATE,
    TOOL_ROTATE,
    TOOL_SCALE,
    TOOL_2DCOLLISION_LINE,
    TOOL_SELECT_LINE,
    TOOL_CREATE_AREA,
    TOOL_CREATE_LIGHT,
} eToolType;

// Controller for the list that displays all the objects in the scene
@interface C3DLayoutSceneOutlineController : NSObject 
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

// This is the base class for a tool controller. These tool controllers are
// mini-controllers that will recieve input and edit the scene based on what
// tool it is
@interface CLayoutEditorTool : NSObject 
{
@protected
    C3DSceneEditorView *mView;
    CHeirarchyNode *mSceneRoot;
    eToolType mToolType;
    
    // For refreshing
    NSOutlineView *mSceneOutline;
}

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview;

- (void)ViewLeftMouseDown:(NSNotification*)notification;
- (void)ViewLeftMouseDragged:(NSNotification*)notification;

@end

// Tool to draw 2D collision lines
@interface C2DCollisionLineTool : CLayoutEditorTool 
{
@private
    NSPoint mLinePoints[2];
    int mCurrentPoint;
    C2DPhysicsLine *mCurrentLine;
    NSPopUpButton *mLineTypePopup;
}

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview;

- (void)setLinePopup:(NSPopUpButton*)button;

- (void)ViewLeftMouseDown:(NSNotification*)notification;
- (void)ViewLeftMouseDragged:(NSNotification*)notification;

@end

typedef enum
{
    SELECTION_NONE,
    SELECTION_COLLISION_LINE_POINT,
    SELECTION_LIGHT,
    SELECTION_AREA,
} ESelectionType;

@interface C2DSelectCollisionPointTool : CLayoutEditorTool 
{
@private
    int mSelectedPoint;
    C2DPhysicsLine *mSelectedLine;
    
    id mSelection;
    ESelectionType mSelectionType;
    
    NSPoint mClickLocation;
    
    // Area specifics
    NSRect mInitialAreaBounds;
}

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview;

- (void)ViewLeftMouseDown:(NSNotification*)notification;
- (void)ViewLeftMouseDragged:(NSNotification*)notification;

@end

@interface C2DCreateAreaTool : CLayoutEditorTool 
{
@private
    C2DArea *mCurrentArea;
    NSPoint mSourcePoint;
}

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview;

- (void)ViewLeftMouseDown:(NSNotification*)notification;
- (void)ViewLeftMouseDragged:(NSNotification*)notification;

@end

@interface CCreateLightTool : CLayoutEditorTool 
{
@private
    CSceneLight *mCurrentLight;
}

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview;

- (void)ViewLeftMouseDown:(NSNotification*)notification;
- (void)ViewLeftMouseDragged:(NSNotification*)notification;

@end

@interface C3DLayoutEditorController : NSWindowController 
{
    
@private
    IBOutlet NSBox *mRibbonView;
    IBOutlet NSView *mTransformRibbonView;
    IBOutlet NSView *mCreateRibbonView;
    IBOutlet NSView *m2DLayoutRibbonView;
    IBOutlet C3DSceneLayoutObjectListController *mListController;
    IBOutlet C3DLayoutSceneOutlineController *mSceneOutlineController;
    IBOutlet NSOutlineView *mList;
    IBOutlet NSOutlineView *mSceneOutline;
    IBOutlet C3DSceneEditorView *mView;
    IBOutlet NSPopUpButton *mLineTypeButton;
    IBOutlet NSTableView *mObjectPropertyTable;
    CHeirarchyNode *mSceneRoot;
    C3DSceneLayoutResource *mSceneResource;
    eToolType mCurrentTool;
    CLayoutEditorTool *mCurrentToolController;
}

- (void)setLayoutResource:(C3DSceneLayoutResource*)resource;

- (void)initWindow;
- (IBAction)switchRibbon:(id)sender;
- (IBAction)importObject:(id)sender;
- (IBAction)toolInvoked:(id)sender;
- (IBAction)exportCollision:(id)sender;
- (IBAction)sceneOutlineSelected:(id)sender;

- (void)ViewLeftMouseDown:(NSNotification*)notification;
- (void)ViewLeftMouseDragged:(NSNotification*)notification;

@end
