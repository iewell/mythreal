//
//  MaterialEditorController.h
//  Resource Edit
//
//  Created by Ian Ewell on 6/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MaterialResource.h"
#import "ShaderResource.h"
#import "ResourceListModel.h"

extern CResourceListModel *gModel;

@interface CMaterialUniformEditor : NSWindowController 
{
@private
    IBOutlet NSMatrix *mValueMatrix;
    IBOutlet NSPopUpButton *mPopup;
    
    CMaterialUniform *mUniform;
    CHeirarchyNode *mTextureRoot;
}

- (void)setUniform:(CMaterialUniform*)uniform;
- (IBAction)closePressed:(id)sender;

- (void)setTextureRoot:(CHeirarchyNode*)root;

@end

@interface CMaterialEditorUniformListController : NSObject 
{
@private
    CMaterialResource *mCurrentMaterial;
    IBOutlet CMaterialUniformEditor *mUniformEditor;
    NSButtonCell *mButtonCell;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView;
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;

- (void)setCurrentMaterial:(CMaterialResource*)material;

@end

@interface CMaterialEditorController : NSWindowController 
{
@private
    IBOutlet NSTableView *mUniformTable;
    IBOutlet NSTableColumn *mValueColumn;
    IBOutlet NSPopUpButton *mShaderPopup;
    IBOutlet CMaterialEditorUniformListController *mListController;
    
    CMaterialResource *mCurrentMaterial;
    CResourceNode *mCurrentShader;
    CHeirarchyNode *mShaderRoot;
}

- (void)setMaterialResource:(CMaterialResource*)material;
- (IBAction)shaderPopupChanged:(id)sender;

@end
