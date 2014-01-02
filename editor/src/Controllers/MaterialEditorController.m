//
//  MaterialEditorController.m
//  Resource Edit
//
//  Created by Ian Ewell on 6/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MaterialEditorController.h"

@implementation CMaterialUniformEditor

- (void)setUniform:(CMaterialUniform *)uniform
{
    if (mUniform)
        [mUniform release];
    mUniform = [uniform retain];
    
    NSTextFieldCell *c11 = [mValueMatrix cellAtRow:0 column:0];
    NSTextFieldCell *c12 = [mValueMatrix cellAtRow:0 column:1];
    NSTextFieldCell *c13 = [mValueMatrix cellAtRow:0 column:2];
    NSTextFieldCell *c14 = [mValueMatrix cellAtRow:0 column:3];
    NSTextFieldCell *c21 = [mValueMatrix cellAtRow:1 column:0];
    NSTextFieldCell *c22 = [mValueMatrix cellAtRow:1 column:1];
    NSTextFieldCell *c23 = [mValueMatrix cellAtRow:1 column:2];
    NSTextFieldCell *c24 = [mValueMatrix cellAtRow:1 column:3];
    NSTextFieldCell *c31 = [mValueMatrix cellAtRow:2 column:0];
    NSTextFieldCell *c32 = [mValueMatrix cellAtRow:2 column:1];
    NSTextFieldCell *c33 = [mValueMatrix cellAtRow:2 column:2];
    NSTextFieldCell *c34 = [mValueMatrix cellAtRow:2 column:3];
    NSTextFieldCell *c41 = [mValueMatrix cellAtRow:3 column:0];
    NSTextFieldCell *c42 = [mValueMatrix cellAtRow:3 column:1];
    NSTextFieldCell *c43 = [mValueMatrix cellAtRow:3 column:2];
    NSTextFieldCell *c44 = [mValueMatrix cellAtRow:3 column:3];
    
    [c11 setEnabled:FALSE];
    [c12 setEnabled:FALSE];
    [c13 setEnabled:FALSE];
    [c14 setEnabled:FALSE];
    [c21 setEnabled:FALSE];
    [c22 setEnabled:FALSE];
    [c23 setEnabled:FALSE];
    [c24 setEnabled:FALSE];
    [c31 setEnabled:FALSE];
    [c32 setEnabled:FALSE];
    [c33 setEnabled:FALSE];
    [c34 setEnabled:FALSE];
    [c41 setEnabled:FALSE];
    [c42 setEnabled:FALSE];
    [c43 setEnabled:FALSE];
    [c44 setEnabled:FALSE];
    
    switch ([uniform getType])
    {
        case UT_FLOAT:
            [c11 setEnabled:TRUE];
            [c11 setFloatValue:[(CMaterialFloatUniform*)uniform getFloat]];
            [mPopup removeAllItems];
            [mPopup insertItemWithTitle:@"Custom" atIndex:VTF_CUSTOM];
            [mPopup insertItemWithTitle:@"Delta Time" atIndex:VTF_DT];
            [mPopup insertItemWithTitle:@"Light Count" atIndex:VTF_LIGHT_COUNT];
            [mPopup insertItemWithTitle:@"Light Intensity" atIndex:VTF_LIGHT_INTENSITY];
            break;
        case UT_VECTOR2:
            [c11 setEnabled:TRUE];
            [c12 setEnabled:TRUE];
            [c11 setFloatValue:[(CMaterialVector2Uniform*)uniform getVector][0]];
            [c12 setFloatValue:[(CMaterialVector2Uniform*)uniform getVector][1]];
            [mPopup removeAllItems];
            [mPopup insertItemWithTitle:@"Custom" atIndex:VTV2_CUSTOM];
            break;
        case UT_VECTOR3:
            [c11 setEnabled:TRUE];
            [c12 setEnabled:TRUE];
            [c13 setEnabled:TRUE];
            [c11 setFloatValue:[(CMaterialVector3Uniform*)uniform getVector][0]];
            [c12 setFloatValue:[(CMaterialVector3Uniform*)uniform getVector][1]];
            [c13 setFloatValue:[(CMaterialVector3Uniform*)uniform getVector][2]];
            [mPopup removeAllItems];
            [mPopup insertItemWithTitle:@"Custom" atIndex:VTV3_CUSTOM];
            [mPopup insertItemWithTitle:@"Eye Position" atIndex:VTV3_EYE_POS];
            [mPopup insertItemWithTitle:@"Light Position" atIndex:VTV3_LIGHTPOS];
            [mPopup insertItemWithTitle:@"Light Color" atIndex:VTV3_LIGHT_COLOR];
            break;
        case UT_VECTOR4:
            [c11 setEnabled:TRUE];
            [c12 setEnabled:TRUE];
            [c13 setEnabled:TRUE];
            [c14 setEnabled:TRUE];
            [c11 setFloatValue:[(CMaterialVector4Uniform*)uniform getVector][0]];
            [c12 setFloatValue:[(CMaterialVector4Uniform*)uniform getVector][1]];
            [c13 setFloatValue:[(CMaterialVector4Uniform*)uniform getVector][2]];
            [c14 setFloatValue:[(CMaterialVector4Uniform*)uniform getVector][3]];
            [mPopup removeAllItems];
            [mPopup insertItemWithTitle:@"Custom" atIndex:VTV4_CUSTOM];
            break;
        case UT_MATRIX4:
            [c11 setEnabled:TRUE];
            [c12 setEnabled:TRUE];
            [c13 setEnabled:TRUE];
            [c14 setEnabled:TRUE];
            [c21 setEnabled:TRUE];
            [c22 setEnabled:TRUE];
            [c23 setEnabled:TRUE];
            [c24 setEnabled:TRUE];
            [c31 setEnabled:TRUE];
            [c32 setEnabled:TRUE];
            [c33 setEnabled:TRUE];
            [c34 setEnabled:TRUE];
            [c41 setEnabled:TRUE];
            [c42 setEnabled:TRUE];
            [c43 setEnabled:TRUE];
            [c44 setEnabled:TRUE];
            [c11 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][0]];
            [c21 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][1]];
            [c31 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][2]];
            [c41 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][3]];
            [c12 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][4]];
            [c22 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][5]];
            [c32 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][6]];
            [c42 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][7]];
            [c13 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][8]];
            [c23 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][9]];
            [c33 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][10]];
            [c43 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][11]];
            [c14 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][12]];
            [c24 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][13]];
            [c34 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][14]];
            [c44 setFloatValue:[(CMaterialMatrix4Uniform*)uniform getMatrix][15]];
            [mPopup removeAllItems];
            [mPopup insertItemWithTitle:@"Custom" atIndex:VTM4_CUSTOM];
            [mPopup insertItemWithTitle:@"Model View" atIndex:VTM4_MODEL_VIEW];
            [mPopup insertItemWithTitle:@"Model" atIndex:VTM4_MODEL];
            [mPopup insertItemWithTitle:@"View" atIndex:VTM4_VIEW];
            [mPopup insertItemWithTitle:@"Projection" atIndex:VTM4_PROJECTION];
            [mPopup insertItemWithTitle:@"Bones" atIndex:VTM4_BONES];
            break;
        case UT_SAMPLER2D:
            [self setTextureRoot:[gModel getResourceTypeAtIndex:RT_TEXTURE]];
            [mPopup removeAllItems];
            for (CHeirarchyNode *textureNode in [mTextureRoot getChildren])
            {
                [mPopup addItemWithTitle:[textureNode name]];
            }
            if ([(CMaterialSampler2DUniform*)uniform getTexture])
                [mPopup selectItemAtIndex:[[(CMaterialSampler2DUniform*)uniform getTexture] ID]];
            break;
        default:
            [mPopup removeAllItems];
            break;
    }
    if ([uniform getType] != UT_SAMPLER2D)
        [mPopup selectItemAtIndex:[uniform getValueType]];
}

- (IBAction)closePressed:(id)sender
{
    // Store the value
    [mUniform setValueType:[mPopup indexOfSelectedItem]];
    
    NSTextFieldCell *c11 = [mValueMatrix cellAtRow:0 column:0];
    NSTextFieldCell *c12 = [mValueMatrix cellAtRow:0 column:1];
    NSTextFieldCell *c13 = [mValueMatrix cellAtRow:0 column:2];
    NSTextFieldCell *c14 = [mValueMatrix cellAtRow:0 column:3];
    NSTextFieldCell *c21 = [mValueMatrix cellAtRow:1 column:0];
    NSTextFieldCell *c22 = [mValueMatrix cellAtRow:1 column:1];
    NSTextFieldCell *c23 = [mValueMatrix cellAtRow:1 column:2];
    NSTextFieldCell *c24 = [mValueMatrix cellAtRow:1 column:3];
    NSTextFieldCell *c31 = [mValueMatrix cellAtRow:2 column:0];
    NSTextFieldCell *c32 = [mValueMatrix cellAtRow:2 column:1];
    NSTextFieldCell *c33 = [mValueMatrix cellAtRow:2 column:2];
    NSTextFieldCell *c34 = [mValueMatrix cellAtRow:2 column:3];
    NSTextFieldCell *c41 = [mValueMatrix cellAtRow:3 column:0];
    NSTextFieldCell *c42 = [mValueMatrix cellAtRow:3 column:1];
    NSTextFieldCell *c43 = [mValueMatrix cellAtRow:3 column:2];
    NSTextFieldCell *c44 = [mValueMatrix cellAtRow:3 column:3];
    
    float data[4];
    data[0] = [c11 floatValue];
    data[1] = [c12 floatValue];
    data[2] = [c13 floatValue];
    data[3] = [c14 floatValue];
    
    float matrix[16];
    matrix[0] = [c11 floatValue];
    matrix[1] = [c21 floatValue];
    matrix[2] = [c31 floatValue];
    matrix[3] = [c41 floatValue];
    matrix[4] = [c12 floatValue];
    matrix[5] = [c22 floatValue];
    matrix[6] = [c32 floatValue];
    matrix[7] = [c42 floatValue];
    matrix[8] = [c13 floatValue];
    matrix[9] = [c23 floatValue];
    matrix[10] = [c33 floatValue];
    matrix[11] = [c43 floatValue];
    matrix[12] = [c14 floatValue];
    matrix[13] = [c24 floatValue];
    matrix[14] = [c34 floatValue];
    matrix[15] = [c44 floatValue];
    
    switch ([mUniform getType]) 
    {
        case UT_FLOAT:
            [(CMaterialFloatUniform*)mUniform setFloat:[c11 floatValue]];
            break;
        case UT_VECTOR2:
            [(CMaterialVector2Uniform*)mUniform setVector:data];
            break;
        case UT_VECTOR3:
            [(CMaterialVector3Uniform*)mUniform setVector:data];
            break;
        case UT_VECTOR4:
            [(CMaterialVector4Uniform*)mUniform setVector:data];
            break;
        case UT_MATRIX4:
            [(CMaterialMatrix4Uniform*)mUniform setMatrix:matrix];
            break;
        case UT_SAMPLER2D:
            [(CMaterialSampler2DUniform*)mUniform setTexture:(CResourceNode*)[mTextureRoot getChildAtIndex:[mPopup indexOfSelectedItem]]];
            break;
        default:
            break;
    }
    
    [self close];
}

- (void)setTextureRoot:(CHeirarchyNode *)root
{
    if (mTextureRoot)
        [mTextureRoot release];
    mTextureRoot = [root retain];
}

@end

@implementation CMaterialEditorUniformListController

- (id)init
{
    self = [super init];
    if (self)
    {
        mButtonCell = [[NSButtonCell alloc] init];
        [mButtonCell setTitle:@"Edit"];
    }
    return self;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    if (mCurrentMaterial)
        return [mCurrentMaterial getUniformCount];
    return 0;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    if ([(NSString*)[aTableColumn identifier] compare:@"uniform"] == NSOrderedSame)
    {
        return [[mCurrentMaterial getUniformAtIndex:rowIndex] getName];
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"value"] == NSOrderedSame)
    {
        [aTableColumn setDataCell:mButtonCell];
        return @"Edit";
    }
    return nil;

}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    if ([(NSString*)[aTableColumn identifier] compare:@"value"] == NSOrderedSame)
    {
        [mUniformEditor setUniform:[mCurrentMaterial getUniformAtIndex:rowIndex]];
        [mUniformEditor showWindow:self];
    }
}

- (void)setCurrentMaterial:(CMaterialResource*)material
{
    if (mCurrentMaterial)
        [mCurrentMaterial release];
    mCurrentMaterial = [material retain];
}

@end

@implementation CMaterialEditorController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    NSButtonCell *cell = [[NSButtonCell alloc] init];
    [cell setTitle:@"Edit"];
    [mValueColumn setDataCell:cell];
    [cell release];
}

- (void)setMaterialResource:(CMaterialResource*)material
{
    if (mCurrentMaterial)
        [mCurrentMaterial release];
    mCurrentMaterial = [material retain];
    if (mShaderRoot)
        [mShaderRoot release];
    mShaderRoot = [[gModel getResourceTypeAtIndex:RT_SHADER] retain];
    [mShaderPopup removeAllItems];
    for (CHeirarchyNode *shaderNode in [mShaderRoot getChildren])
    {
        [mShaderPopup addItemWithTitle:[shaderNode name]];
    }
    [mShaderPopup selectItemAtIndex:[[mCurrentMaterial getShaderResource] ID]];
    [mListController setCurrentMaterial:mCurrentMaterial];
    [mUniformTable reloadData];
}

- (IBAction)shaderPopupChanged:(id)sender
{
    if ([mShaderRoot numberOfChildren] > 0)
    {
        NSUInteger shaderID = [(NSPopUpButton*)sender indexOfSelectedItem];
        CResourceNode *shader = (CResourceNode*)[mShaderRoot getChildAtIndex:shaderID];
        
        if (mCurrentShader)
            [mCurrentShader release];
        mCurrentShader = [shader retain];
        [mCurrentMaterial buildUniformListFromShaderResource:mCurrentShader];
        [mUniformTable reloadData];
    }
}

@end
