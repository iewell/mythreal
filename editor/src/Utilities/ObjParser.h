/*
 *
 * FILE: ObjParser.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Utilities
 *
 * PURPOSE: Parses and loads a model in the Obj format.
 *
 * CREATED ON: 07/30/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import "3DModelResource.h"


@interface CObjParser : NSObject 
{
    int mVertexCount;
    int mUVCount;
    int mNormalCount;
}

- (CModelRoot*)parse:(NSString*)path;

@end
