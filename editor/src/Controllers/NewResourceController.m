/*
 *
 * FILE: NewResourceController.m
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

#import "NewResourceController.h"


@implementation CNewResourceController

- (void)windowCreated
{
	[mType addItemWithTitle:@"3D Model"];
	[mType addItemWithTitle:@"Texture"];
    [mType addItemWithTitle:@"Shader"];
    [mType addItemWithTitle:@"Material"];
    [mType addItemWithTitle:@"3D Level Layout"];
}

- (IBAction)cancelClicked: (id)sender
{
	[NSApp abortModal];
	[super close];
}

- (IBAction)OKClicked: (id)sender
{
	NSString *name = [mName stringValue];
	
	if ([name length] == 0)
	{
        NSBeep();
		NSRunAlertPanel(@"No Name", @"Your resource must have a name in alphanumeric format.", @"OK", nil, nil);
		return;
	}
	
	NSUInteger i = 0;
	for (i = 0; i < [name length]; i++)
	{
		unichar character = [name characterAtIndex:i];
		if (![[NSCharacterSet alphanumericCharacterSet] characterIsMember:character])
		{
            NSBeep();
			NSRunAlertPanel(@"Invalid Name", @"Your name may only contain alphanumeric characters.", @"OK", nil, nil);
			return;
		}
	}
	
	if (![[mResourceListController mModel] addResourceOfType:[mType indexOfSelectedItem] withName:name])
	{
        NSBeep();
		NSRunAlertPanel(@"Name Already Exists!",
						@"The name you are attempting to give the resource already exists. Try giving it a different name.",
						@"OK", nil, nil);
		return;
	}
	
	[mName setStringValue:@""];
	[NSApp abortModal];
	[super close];
}

@end
