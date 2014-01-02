/*
 *
 * FILE: 3DModelView.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Views
 *
 * PURPOSE: A view for rendering a preview of a 3D model resource.
 *
 * CREATED ON: 07/28/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import "3DModelResource.h"


@interface CModelView : NSOpenGLView 
{
    CModelRoot *mRenderModel;
    NSMutableArray *mMaterialGroups;
    BOOL mWireframeMode;
    float mRotX;
    float mRotY;
    NSPoint mMouseLoc;
    float mZoom;
    
    int mFrame;
    
    IBOutlet NSOutlineView *mOutlineView;
}

- (void)renderBone:(CModelSkeletonNode*)bone;

- (void)setRenderModel:(CModelRoot*)model;
- (void)setWireframeMode:(BOOL)mode;

- (void)setFrameRel:(float)frame;

- (void)rightMouseDown:(NSEvent *)theEvent;
- (void)rightMouseDragged:(NSEvent *)theEvent;
- (void)scrollWheel:(NSEvent *)theEvent;

@end
