/*
 *
 * FILE: 3DSceneEditorView.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Views
 *
 * PURPOSE: A view for rendering the scene in the 3D scene editor.
 *
 * CREATED ON: 11/22/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

#import "HeirarchyNode.h"
#import "3DSceneObjectInstance.h"
#import "2DSceneObjects.h"

@interface C3DSceneEditorView : NSOpenGLView 
{
@private
    CHeirarchyNode *mScene;
    float m2DZoom;
    BOOL m2DMode;
    NSMutableData* m2DLineArray;
    
    float m2DTransX;
    float m2DTransY;
    NSPoint mMouseLoc;
}
- (void)setScene:(CHeirarchyNode*)scene;
- (void)set2DMode:(BOOL)mode;

- (void)set2DLineArray:(NSMutableData*)array;

- (NSPoint)getWorldSpaceCoordinateFromScreenSpaceCoordinate:(NSPoint)coordinate;
- (NSPoint)getScreenSpaceCoordinateFromWorldSpaceCoordinate:(NSPoint)coordinate;

- (void)rightMouseDown:(NSEvent *)theEvent;
- (void)rightMouseDragged:(NSEvent *)theEvent;
- (void)mouseDown:(NSEvent *)theEvent;
- (void)mouseDragged:(NSEvent *)theEvent;

- (void)scrollWheel:(NSEvent *)theEvent;

@end
