/*
 *
 * FILE: 3DModelResource.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Models
 *
 * PURPOSE: Stores information about an instance of a scene object
 *
 * CREATED ON: 11/25/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import "3DModelResource.h"


@interface C3DSceneObjectInstance : CHeirarchyNode 
{
@private
    CResourceNode *m3DModel;
    NSString *m3DModelName;
    CModelRoot *m3DModelRoot;
    NSMutableArray *mMaterialGroups;
    
    float mPosition[3];
}

- (void)set3DModel:(CResourceNode*)model;
- (void)render;

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;


@end
