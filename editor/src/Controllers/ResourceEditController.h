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
 * PURPOSE: Controls the main window of the editor
 *
 * CREATED ON: 07/07/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

// Resource File mrrc version history
// 0.1 - initial file version
// 0.2 - Added support for 3D positioning scene objects in a level layout
// 0.3 - Added animation support. This mainly affects the 3D model classes and nodes for skeletons and frames have been added

#import <Cocoa/Cocoa.h>
#import "ResourceListController.h"

#define RESOURCE_FORMAT_VERSION_MAJOR 0
#define RESOURCE_FORMAT_VERSION_MINOR 3

@class CNewResourceController;

@interface CResourceEditController : NSWindowController 
{
	// Member Variables:
	@private
	
	// The window to display when you click "new".
	IBOutlet CNewResourceController *mNewResourceWindow;
	IBOutlet NSOutlineView *mResourceList;
    
    IBOutlet NSWindowController *m3DEditor;
    IBOutlet NSWindowController *mMaterialEditor;
    IBOutlet NSWindowController *mShaderEditor;
    IBOutlet NSWindowController *m3DLayoutEditor;
    IBOutlet NSWindowController *mTextureEditor;
    
    IBOutlet CResourceListController *mResourceListController;
}

// Public Methods:

- (IBAction)NewClicked:(id)sender;
- (IBAction)RemoveClicked:(id)sender;
- (IBAction)EditClicked:(id)sender;
- (IBAction)SaveClicked:(id)sender;
- (IBAction)OpenClicked:(id)sender;
- (IBAction)NewDocumentClicked:(id)sender;

@end
