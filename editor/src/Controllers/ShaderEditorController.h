//
//  ShaderEditorController.h
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ShaderResource.h"

@interface CShaderEditorListController : NSObject 
{
@private
    CShaderResource *mShaderResource;
    NSPopUpButtonCell *mPopupCell;
    NSMutableArray *mUniformPopupArray;
}

- (void)setShaderResource:(CShaderResource*)resource;

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;

- (void)addNewPopupCell;

@end

@interface CShaderEditorController : NSWindowController
{
    IBOutlet NSTableView *mUniformView;
    IBOutlet CShaderEditorListController *mListController;
    CShaderResource *mShaderResource;
}

- (void)setShaderResource:(CShaderResource*)resource;

- (IBAction)plusButtonPressed:(id)sender;

- (IBAction)loadVertexShaderPressed:(id)sender;
- (IBAction)loadFragmentShaderPressed:(id)sender;

@end
