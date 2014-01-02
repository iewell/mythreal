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
 * PURPOSE: Controls the main window of the editor
 *
 * CREATED ON: 07/07/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>

#import "ResourceEditController.h"
#import "NewResourceController.h"
#import "ResourceListModel.h"
#import "3DModelEditorController.h"
#import "3DLayoutEditorController.h"
#import "ShaderEditorController.h"
#import "MaterialEditorController.h"
#import "TextureEditorController.h"
#import "FileStream.h"

@implementation CResourceEditController

- (IBAction)NewClicked: (id)sender
{
	if (!mNewResourceWindow)
		mNewResourceWindow = [[CNewResourceController alloc] init];
	
	[mNewResourceWindow showWindow:self];
	[mNewResourceWindow windowCreated];
	[NSApp runModalForWindow:[mNewResourceWindow window]];
	[mResourceList reloadItem:nil reloadChildren:YES];
}

- (IBAction)RemoveClicked: (id)sender
{
	NSInteger selection = [mResourceList selectedRow];
	CHeirarchyNode *node = [mResourceList itemAtRow:selection];
	if ([[node getNodeType] compare:@"Resource"] == NSOrderedSame)
		[node removeSelf];
	[mResourceList reloadItem:nil reloadChildren:YES];
}

- (IBAction)EditClicked:(id)sender
{
    NSInteger selection = [mResourceList selectedRow];
    if (selection == -1)
        return;
    
	CResourceNode *node = [mResourceList itemAtRow:selection];
    
    if (![[node getNodeType] compare:@"Resource"] == NSOrderedSame)
        return;
    
    NSWindowController *editor;
    
    switch ([[node getParent] ID])
    {
        case RT_3D_MODEL:
            editor = m3DEditor;
            [(CModelEditorController*)editor setResource:(CModelResource*)[node getResource]];
            break;
        case RT_TEXTURE:
            editor = mTextureEditor;
            [(CTextureEditorController*)editor setTextureResource:(CTextureResource*)[node getResource]];
            break;
        case RT_SHADER:
            editor = mShaderEditor;
            [(CShaderEditorController*)editor setShaderResource:(CShaderResource*)[node getResource]];
            break;
        case RT_MATERIAL:
            editor = mMaterialEditor;
            [(CMaterialEditorController*)editor setMaterialResource:(CMaterialResource*)[node getResource]];
            break;
        case RT_3D_LEVEL_LAYOUT:
            editor = m3DLayoutEditor;
            [(C3DLayoutEditorController*)editor setLayoutResource:(C3DSceneLayoutResource*)[node getResource]];
            [(C3DLayoutEditorController*)editor initWindow];
            break;
        default:
            editor = nil;
    }
    
    [editor showWindow:self];
}

- (IBAction)SaveClicked:(id)sender
{
    NSSavePanel *savePanel = [NSSavePanel savePanel];
    [savePanel setAllowedFileTypes:[[[NSArray alloc] initWithObjects:@"mrrc", nil] autorelease]];
    [savePanel setTitle:@"Save the file"];
    NSUInteger result = [savePanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSString *path = [[savePanel URL] path];
    
    CFileStream *stream = [[CFileStream alloc] init];
    [stream openFile:path];
    [stream writeData:"MRRC" withLength:4];
    
    // Write version
    [stream writeUShort:RESOURCE_FORMAT_VERSION_MAJOR];
    [stream writeUShort:RESOURCE_FORMAT_VERSION_MINOR];
    
    // Write data
    [[mResourceListController mRoot] writeToStream:stream];
    
    [stream close];
    
    /*mkdir([path UTF8String], S_IRWXU | S_IRWXG | S_IRWXO);
    mkdir([[NSString stringWithFormat:@"%@/models", path] UTF8String], S_IRWXU | S_IRWXG | S_IRWXO);
    
    NSXMLElement *root = (NSXMLElement*)[NSXMLNode elementWithName:@"MythrealResourceXML"];
    NSXMLDocument *doc = [[NSXMLDocument alloc] initWithRootElement:root];
    [doc setVersion:@"1.0"];
    [doc setCharacterEncoding:@"UTF-8"];
    [root addAttribute:[NSXMLNode attributeWithName:@"version" stringValue:@"0"]];
    
    [[mResourceListController mRoot] serializeXML:root bundleLocation:path];
    
    NSData *xmlFile = [doc XMLDataWithOptions:NSXMLNodePrettyPrint];
    [xmlFile writeToFile:[NSString stringWithFormat:@"%@/root.xml", path] atomically:YES];
    [doc release];*/
}

- (IBAction)OpenClicked:(id)sender
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setAllowsMultipleSelection:NO];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowedFileTypes:[[[NSArray alloc] initWithObjects:@"mrrc", nil] autorelease]];
    [openPanel setTitle:@"Select a reource to open"];
    NSUInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    CFileInputStream *stream = [[CFileInputStream alloc] init];
    [stream openFile:[[openPanel URL] path]];
    
    char magic[4];
    [stream readDataIntoBuffer:&magic[0] withLenght:4];
    
    unsigned short versionMajor = [stream readUShort];
    unsigned short versionMinor = [stream readUShort];
    
    [stream setVersion:versionMinor Major:versionMajor];
    
    if (versionMajor != RESOURCE_FORMAT_VERSION_MAJOR)
    {
        NSBeep();
        NSRunAlertPanel(@"Incompatible Version", @"The version of this resource is too old or new to work with this program", @"OK", nil, nil);
        [stream close];
        return;
    }
    
    CHeirarchyNode *root = [[CHeirarchyNode alloc] init];
    [root setName:[stream readString]];
    [stream readString];
    [root readFromStream:stream];
    [[mResourceListController mModel] setRootNode:root init:false];
    [mResourceListController setMRoot:root];
    [mResourceList reloadItem:nil reloadChildren:YES];
}

- (IBAction)NewDocumentClicked:(id)sender
{
    // Re-init the controller
    [mResourceListController reinit];
    
    // Refresh the view
    [mResourceList reloadItem:nil reloadChildren:YES];
}

@end
