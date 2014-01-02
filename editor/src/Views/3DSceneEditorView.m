/*
 *
 * FILE: 3DSceneEditorView.m
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

#import "3DSceneEditorView.h"


@implementation C3DSceneEditorView

- (id)initWithFrame:(NSRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
    {
        m2DZoom = 100;
        m2DTransX = 0;
        m2DTransY = 0;
    }
    
    return self;
}

- (void)dealloc 
{
    // Clean-up code here.
    
    [super dealloc];
}

- (void)prepare
{
    NSOpenGLContext *context = [self openGLContext];
    [context makeCurrentContext];
}

- (void)prepareOpenGL
{
    GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.00f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glEnable(GL_DEPTH_TEST);
}

- (void)reshape
{
    NSRect base = [self convertRectToBase:[self bounds]];
    float aspect = base.size.width/base.size.height;
    glViewport(0, 0, base.size.width, base.size.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (m2DMode)
        glOrtho(-m2DZoom*aspect/2, m2DZoom*aspect/2, -m2DZoom/2, m2DZoom/2, 0.1, 1000);
    else
        gluPerspective(60.0, base.size.width/base.size.height, 0.1, 1000);
}

- (void)drawRect:(NSRect)dirtyRect 
{
    // Drawing code here.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    NSRect base = [self convertRectToBase:[self bounds]];
    float aspect = base.size.width/base.size.height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (m2DMode)
        glOrtho(-m2DZoom*aspect/2, m2DZoom*aspect/2, -m2DZoom/2, m2DZoom/2, 0.1, 1000);
    else
        gluPerspective(60.0, base.size.width/base.size.height, 0.1, 1000);
    
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);
    glLoadIdentity();
    
    glTranslatef(0, 0, -40);
    
    if (m2DMode)
        glTranslatef(m2DTransX, m2DTransY, 0.0f);
    
    glPushMatrix();
    
    for (C3DSceneObjectInstance *instance in [mScene getChildren])
    {
        if ([[instance getNodeType] compare:@"3DSceneObjectInstance"] == NSOrderedSame)
            [instance render];
    }
    
    glPopMatrix();
    
    // Now render the 2D collision Data
    CHeirarchyNode *collisionData = [mScene findChildWithName:@"2DCollisionData" isRecursive:FALSE];
    if (collisionData)
    {
        glDepthFunc(GL_NOTEQUAL);
        glClear(GL_DEPTH_BUFFER_BIT);
        for (C2DPhysicsLine *line in [collisionData getChildren])
            [line renderLine];
    }
    
    // Render the areas
    CHeirarchyNode *areaData = [mScene findChildWithName:@"2DAreaData" isRecursive:FALSE];
    if (areaData)
    {
        for (C2DArea *area in [areaData getChildren])
        {
            [area render];
            
            // Render the lights inside
            for (CSceneLight *light in [area getChildren])
                [light render];
        }
    }
    
    glFlush();
    [self setNeedsDisplay:TRUE];
}

- (void)setScene:(CHeirarchyNode*)scene
{
    if (mScene)
        [mScene release];
    mScene = [scene retain];
}

- (void)set2DMode:(BOOL)mode
{
    m2DMode = mode;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    NSRect base = [self convertRectToBase:[self bounds]];
    float aspect = base.size.width/base.size.height;
    glOrtho(-m2DZoom*aspect/2, m2DZoom*aspect/2, -m2DZoom/2, m2DZoom/2, 0.1, 1000);
}

- (void)set2DLineArray:(NSMutableData *)array
{
    m2DLineArray = [array retain];
}

// NOTE: this function will only work properly if the view is in 2D mode. There needs to be a function that will generate a ray in perspective
- (NSPoint)getWorldSpaceCoordinateFromScreenSpaceCoordinate:(NSPoint)coordinate
{
    NSRect base = [self convertRectToBase:[self bounds]];
    NSPoint ret;
    
    NSPoint newPoint = [self convertPoint:coordinate fromView:nil];
    
    // First, we must center the coordinate
    ret.x = newPoint.x - base.size.width/2;
    ret.y = newPoint.y - base.size.height/2;
    
    // Now scale it
    float scale = m2DZoom/base.size.height;
    ret.x *= scale;
    ret.y *= scale;
    
    // And translate it
    ret.x -= m2DTransX;
    ret.y -= m2DTransY;
    
    return ret;
}

- (NSPoint)getScreenSpaceCoordinateFromWorldSpaceCoordinate:(NSPoint)coordinate
{
    NSRect base = [self convertRectToBase:[self bounds]];
    NSPoint ret;
    
    // Translate the coordinate to the camera
    ret.x = coordinate.x + m2DTransX;
    ret.y = coordinate.y + m2DTransY;
    
    // Scale it
    float scale = base.size.height/m2DZoom;
    ret.x *= scale;
    ret.y *= scale;
    
    // Move it to the bottom left
    ret.x += base.size.width/2;
    ret.y += base.size.height/2;
    
    return ret;
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
    mMouseLoc = [NSEvent mouseLocation];
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
    m2DTransX += ([NSEvent mouseLocation].x - mMouseLoc.x)*0.2;
    m2DTransY += ([NSEvent mouseLocation].y - mMouseLoc.y)*0.2;
    mMouseLoc = [NSEvent mouseLocation];
    [self setNeedsDisplay:TRUE];
}

- (void)mouseDown:(NSEvent *)theEvent
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"LELeftMouseDown" object:self userInfo:[NSDictionary dictionaryWithObject:theEvent forKey:@"Event"]];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"LELeftMouseDragged" object:self userInfo:[NSDictionary dictionaryWithObject:theEvent forKey:@"Event"]];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
    if (m2DZoom - [theEvent deltaY] > 0)
        m2DZoom -= [theEvent deltaY];
    [self setNeedsDisplay:TRUE];
}

@end
