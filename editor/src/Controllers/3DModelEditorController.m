/*
 *
 * FILE: ResourceEditController.m
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

#import "3DModelEditorController.h"
#import "ObjParser.h"
#import "MayaScene.h"

@implementation C3DModelOutlineController

- (void)setSceneRoot:(CHeirarchyNode *)root
{
    if (mSceneRoot)
        [mSceneRoot release];
    mSceneRoot = [root retain];
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
		return mSceneRoot;
	
	return [item getChildAtIndex:index];
}

- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
	if (!item)
		return [[[NSString alloc] initWithUTF8String:"Model Root"] autorelease];
	return [item name];
	
}

@end

@implementation C3DModelAnimationListController

- (void)setSceneRoot:(CHeirarchyNode*)root
{
    mSceneRoot = [root retain];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    CHeirarchyNode *animNode = [mSceneRoot findChildWithName:@"Animations" isRecursive:FALSE];
    if (animNode)
    {
        return [[animNode getChildren] count];
    }
    return 0;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    CHeirarchyNode *animNode = [mSceneRoot findChildWithName:@"Animations" isRecursive:FALSE];
    CModelAnimation *animation = (CModelAnimation*)[animNode getChildAtIndex:rowIndex];
    if ([(NSString*)[aTableColumn identifier] compare:@"Name"] == NSOrderedSame)
    {
        return [animation name];
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"StartFrame"] == NSOrderedSame)
    {
        return [NSString stringWithFormat:@"%d", [animation getStartFrame]];
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"EndFrame"] == NSOrderedSame)
    {
        return [NSString stringWithFormat:@"%d", [animation getEndFrame]];
    }
    return nil;
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    CHeirarchyNode *animNode = [mSceneRoot findChildWithName:@"Animations" isRecursive:FALSE];
    CModelAnimation *animation = (CModelAnimation*)[animNode getChildAtIndex:rowIndex];
    if ([(NSString*)[aTableColumn identifier] compare:@"Name"] == NSOrderedSame)
    {
        [animation setName:anObject];
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"StartFrame"] == NSOrderedSame)
    {
        [animation setStartFrame:[anObject floatValue]];
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"EndFrame"] == NSOrderedSame)
    {
        [animation setEndFrame:[anObject floatValue]];
    }
    //[aTableView reloadData];
}

@end


@implementation CModelEditorController

- (void)windowCreated
{
    
}

- (IBAction)importObjPressed:(id)sender
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setAllowsMultipleSelection:NO];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowedFileTypes:[[[NSArray alloc] initWithObjects:@"obj", nil] autorelease]];
    [openPanel setTitle:@"Select an Obj model"];
    NSUInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSString *path = [[openPanel URL] path];
    
    __block CModelRoot *root;
    
    dispatch_async(dispatch_get_global_queue(0, 0), 
    ^{
        [mObjParsingLabel setHidden:FALSE];
        [mObjParsingIndicator setHidden:FALSE];
        [mObjParsingIndicator startAnimation:self];
        CObjParser *parser = [[CObjParser alloc] init];
        root = [[parser parse:path] retain];
        [parser release];
        
        dispatch_async(dispatch_get_main_queue(), 
        ^{
            [mObjParsingLabel setHidden:TRUE];
            [mObjParsingIndicator setHidden:TRUE];
            [mObjParsingIndicator stopAnimation:self];
            
            [mResource setRoot:root];
            [mModelView setRenderModel:root];
            [mModelView setNeedsDisplay:TRUE];
            
            [root release];
         }
        );
     }
    );
}

- (IBAction)importMayaPressed:(id)sender
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setAllowsMultipleSelection:NO];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowedFileTypes:[[[NSArray alloc] initWithObjects:@"mb", @"ma", nil] autorelease]];
    [openPanel setTitle:@"Select a Maya scene"];
    NSUInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSString *path = [[openPanel URL] path];
    
    CModelRoot *root;
    
    //[mObjParsingLabel setHidden:FALSE];
    [mObjParsingIndicator setHidden:FALSE];
    [mObjParsingIndicator startAnimation:self];
    MayaScene *scene = [[MayaScene alloc] init];
    root = [scene importModel:path];
    if (root)
        [root retain];
    [scene release];
                       
    [mObjParsingLabel setHidden:TRUE];
    [mObjParsingIndicator setHidden:TRUE];
    [mObjParsingIndicator stopAnimation:self];
    
    if (root)
    {
        [mResource setRoot:root];
        [mModelView setRenderModel:root];
        [mModelView setNeedsDisplay:TRUE];
        [mHeirarchyController setSceneRoot:[mResource getRoot]];
        [mHeirarchy reloadItem:nil];
    }
                                          
}


- (IBAction)savePressed:(id)sender
{
    NSSavePanel *savePanel = [NSSavePanel savePanel];
    [savePanel setAllowedFileTypes:[[[NSArray alloc] initWithObjects:@"m3d", nil] autorelease]];
    [savePanel setTitle:@"Save the file"];
    NSUInteger result = [savePanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSString *path = [[savePanel URL] path];
    [[mResource getRoot] saveToDisk:path];
}

- (IBAction)frameChanged:(id)sender
{
    float val = [mAnimationSlider floatValue];
    [mModelView setFrameRel:val];
    [mModelView setNeedsDisplay:TRUE];
}

- (IBAction)createAnimation:(id)sender
{
    if (![mResource getRoot])
        return;
    
    CModelAnimation *anim = [[CModelAnimation alloc] init];
    [anim setStartFrame:1];
    [anim setEndFrame:10];
    [anim setName:@"New Animation"];
    [anim setAnimationMode:AM_LOOP];
    CHeirarchyNode *animations = [[mResource getRoot] findChildWithName:@"Animations" isRecursive:FALSE];
    if (!animations)
    {
        animations = [[CHeirarchyNode alloc] init];
        [animations setName:@"Animations"];
        [[mResource getRoot] insertChild:animations];
    }
    [animations insertChild:anim];
    [mAnimationList reloadData];
}

- (IBAction)wireframeCheckboxToggled:(NSButton*)sender
{
    if ([sender state] == NSOnState)
        [mModelView setWireframeMode:TRUE];
    else
        [mModelView setWireframeMode:FALSE];
    [mModelView setNeedsDisplay:TRUE];
}

- (void)setResource:(CModelResource*)resource
{
    if (mResource)
        [mResource release];
    mResource = [resource retain];
    [mModelView setRenderModel:[mResource getRoot]];
    [mHeirarchyController setSceneRoot:[mResource getRoot]];
    [mAnimationListController setSceneRoot:[mResource getRoot]];
    [mHeirarchy reloadItem:nil];
}

- (CModelResource*)getResource
{
    return mResource;
}

@end
