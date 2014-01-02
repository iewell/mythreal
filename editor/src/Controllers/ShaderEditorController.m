//
//  ShaderEditorController.m
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ShaderEditorController.h"

@implementation CShaderEditorListController

- (id)init
{
    self = [super init];
    if (self)
    {
        mShaderResource = nil;
        mUniformPopupArray = [[NSMutableArray alloc] init];
        mPopupCell = [[NSPopUpButtonCell alloc] init];
        [mPopupCell insertItemWithTitle:@"Float" atIndex:UT_FLOAT];
        [mPopupCell insertItemWithTitle:@"Vector2" atIndex:UT_VECTOR2];
        [mPopupCell insertItemWithTitle:@"Vector3" atIndex:UT_VECTOR3];
        [mPopupCell insertItemWithTitle:@"Vector4" atIndex:UT_VECTOR4];
        [mPopupCell insertItemWithTitle:@"Matrix4" atIndex:UT_MATRIX4];
        [mPopupCell insertItemWithTitle:@"Sampler2D" atIndex:UT_SAMPLER2D];
        [mPopupCell insertItemWithTitle:@"Sampler Cube" atIndex:UT_SAMPLER_CUBE];
    }
    return self;
}

- (void)setShaderResource:(CShaderResource*)resource
{
    if (mShaderResource)
        [mShaderResource release];
    mShaderResource = [resource retain];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    return [mShaderResource getUniformCount];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    if ([(NSString*)[aTableColumn identifier] compare:@"Name"] == NSOrderedSame)
    {
        return [[mShaderResource getUniformAtIndex:rowIndex] getUniformName];
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"Type"] == NSOrderedSame)
    {
        [aTableColumn setDataCell:mPopupCell];
        //return [[mShaderResource getUniformAtIndex:rowIndex] getUniformType];
        return [NSString stringWithFormat:@"%d", [[mShaderResource getUniformAtIndex:rowIndex] getUniformType]];
    }
    return nil;
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    if ([(NSString*)[aTableColumn identifier] compare:@"Name"] == NSOrderedSame)
    {
        [mShaderResource setUniformName:anObject atIndex:rowIndex];
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"Type"] == NSOrderedSame)
    {
        [mShaderResource setUniformType:[anObject integerValue] atIndex:rowIndex];
    }

}

- (void)addNewPopupCell
{
    NSPopUpButtonCell *cell = [[NSPopUpButtonCell alloc] init];
    [mUniformPopupArray addObject:cell];
    [cell release];
}

@end

@implementation CShaderEditorController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) 
    {
        // Initialization code here.
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    [mUniformView setRowHeight:15.0f];
}

- (void)setShaderResource:(CShaderResource*)resource
{
    if (mShaderResource)
        [mShaderResource release];
    mShaderResource = [resource retain];
    [mListController setShaderResource:mShaderResource];
    [mUniformView reloadData];
}

- (IBAction)plusButtonPressed:(id)sender
{
    [mShaderResource addUniformWithName:@"newUniform" ofType:UT_FLOAT];
    [mListController addNewPopupCell];
    [mUniformView reloadData];
}

- (IBAction)loadVertexShaderPressed:(id)sender
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setAllowsMultipleSelection:NO];
    [openPanel setCanChooseDirectories:NO];
    //[openPanel setAllowedFileTypes:[NSImage imageTypes]];
    [openPanel setTitle:@"Select an image"];
    NSUInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSString *shader = [NSString stringWithContentsOfFile:[[openPanel URL] path] encoding:NSUTF8StringEncoding error:nil];
    [mShaderResource setVertexShader:shader];
}

- (IBAction)loadFragmentShaderPressed:(id)sender
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setAllowsMultipleSelection:NO];
    [openPanel setCanChooseDirectories:NO];
    //[openPanel setAllowedFileTypes:[NSImage imageTypes]];
    [openPanel setTitle:@"Select an image"];
    NSUInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSString *shader = [NSString stringWithContentsOfFile:[[openPanel URL] path] encoding:NSUTF8StringEncoding error:nil];
    [mShaderResource setFragmentShader:shader];
}

@end
