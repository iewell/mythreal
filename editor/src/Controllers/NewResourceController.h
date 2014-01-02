/*
 *
 * FILE: NewResourceController.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Controllers
 *
 * PURPOSE: Controls the window that creates a new resource.
 *
 * CREATED ON: 07/08/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import "ResourceListController.h"

@interface CNewResourceController : NSWindowController 
{
	IBOutlet NSTextField *mName;
	IBOutlet NSPopUpButton *mType;
	
	IBOutlet CResourceListController *mResourceListController;
}

- (void)windowCreated;

- (IBAction)cancelClicked: (id)sender;
- (IBAction)OKClicked: (id)sender;
@end
