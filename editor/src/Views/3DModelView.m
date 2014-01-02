/*
 *
 * FILE: 3DModelView.m
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

#import "3DModelView.h"


@implementation CModelView

- (id)initWithFrame:(NSRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
    {
        mMaterialGroups = nil;
        mRotX = 0;
        mRotY = 0;
        mMouseLoc.x = 0;
        mMouseLoc.y = 0;
        mZoom = -40;
    }
    
    return self;
}

- (void)prepareOpenGL
{
    GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glEnable(GL_DEPTH_TEST);
}

- (void)renderBone:(CModelSkeletonNode*)bone
{
    glPushMatrix();
    GLfloat matrix[16];
    //[bone getInitialMatrix:matrix];
    [[bone frameAtIndex:mFrame] getTransformMatrix:matrix];
    glMultMatrixf(matrix);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    
    for (CModelSkeletonNode* child in [bone getChildren])
        [self renderBone:child];
    
    glPopMatrix();
}

- (void)setRenderModel:(CModelRoot*)model
{
    if (mRenderModel)
        [mRenderModel release];
    mRenderModel = [model retain];
    mMaterialGroups = [[NSMutableArray alloc] init];
    for (CHeirarchyNode *node in [mRenderModel getChildren])
    {
        for (CModelMaterialGroup *group in [node getChildren])
        {
            if ([[group getNodeType] compare:@"MaterialGroup"] == NSOrderedSame)
                [mMaterialGroups addObject:group];
        }
    }
}

- (void)setWireframeMode:(BOOL)mode
{
    mWireframeMode = mode;
}

- (void)setFrameRel:(float)frame
{
    mFrame = frame*[mRenderModel getFrameCount];
}

- (void)dealloc 
{
    // Clean-up code here.
    [mRenderModel release];
    [mMaterialGroups release];
    [super dealloc];
}

- (void)prepare
{
    NSOpenGLContext *context = [self openGLContext];
    [context makeCurrentContext];
}

- (void)reshape
{
    NSRect base = [self convertRectToBase:[self bounds]];
    glViewport(0, 0, base.size.width, base.size.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, base.size.width/base.size.height, 0.1, 1000);
}

- (void)drawRect:(NSRect)dirtyRect 
{
    if ([mRenderModel isSkinned])
        [mRenderModel skin:mFrame];
    
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (mWireframeMode == TRUE)
    {
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_LINE);
    }
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);
    glLoadIdentity();
    
    glTranslatef(0, 0, -40 + mZoom);
    glRotatef(-mRotY/3, 1, 0, 0);
    glRotatef(mRotX/3, 0, 1, 0);

    glScalef(1, 1, 1);
    
    if (mRenderModel)
    {
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat spec[] = { 1.0f, 1.0f, 1.00f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 128);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        if ([mRenderModel isSkinned])
        {
            glVertexPointer(3, GL_FLOAT, 0, [mRenderModel getSkinnedVertices]);
            glNormalPointer(GL_FLOAT, 0, [mRenderModel getSkinnedNormals]);
            glTexCoordPointer(2, GL_FLOAT, 0, [mRenderModel getUVs]);
        }
        else
        {
            glVertexPointer(3, GL_FLOAT, 0, [mRenderModel getVertices]);
            glNormalPointer(GL_FLOAT, 0, [mRenderModel getNormals]);
            glTexCoordPointer(2, GL_FLOAT, 0, [mRenderModel getUVs]);
        }
        for (CModelMaterialGroup *group in mMaterialGroups)
            glDrawElements(GL_TRIANGLES, [group getTriangleCount]*3, GL_UNSIGNED_SHORT, (GLvoid*)[group getVertIndices]);
        CModelMaterialGroup *selectedGroup = [mOutlineView itemAtRow:[mOutlineView selectedRow]];
        if (selectedGroup)
        {
            if ([[selectedGroup getNodeType] isEqualToString:@"MaterialGroup"])
            {
                glClear(GL_DEPTH_BUFFER_BIT);
                glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
                glDrawElements(GL_TRIANGLES, [selectedGroup getTriangleCount]*3, GL_UNSIGNED_SHORT, (GLvoid*)[selectedGroup getVertIndices]);
                
                float *vertexList = (float*)[mRenderModel getVertices];
                unsigned short *indices = (unsigned short*)[selectedGroup getVertIndices];
                
                // Calculate the center
                float minX = vertexList[indices[0]*3];
                float minY = vertexList[indices[0]*3+1];
                float maxX = vertexList[indices[0]*3];
                float maxY = vertexList[indices[0]*3+1];
                for (int i = 0; i < [selectedGroup getTriangleCount]*3; i++)
                {
                    if (vertexList[indices[i]*3] < minX)
                        minX = vertexList[indices[i]*3];
                    if (vertexList[indices[i]*3] > maxX)
                        maxX = vertexList[indices[i]*3];
                    if (vertexList[indices[i]*3+1] < minY)
                        minY = vertexList[indices[i]*3+1];
                    if (vertexList[indices[i]*3+1] > maxY)
                        maxY = vertexList[indices[i]*3+1];
                }
                
                float centerX = ((minX+maxX)/2);
                float centerY = ((minY+maxY)/2);
                
                glDisable(GL_DEPTH_TEST);
                glPointSize(5.0f);
                glBegin(GL_POINTS);
                glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
                glVertex3f(centerX, centerY, 0.0f);
                glEnd();
            }
        }
        
        // Render bones
        CHeirarchyNode *skeleton = [mRenderModel findChildWithName:@"Skeleton" isRecursive:FALSE];
        if (skeleton)
        {
            glDisable(GL_DEPTH_TEST);
            for (CModelSkeletonNode *bone in [skeleton getChildren])
                [self renderBone:bone];
        }
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    
    glFlush();
    [self setNeedsDisplay:TRUE];
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
    mMouseLoc = [NSEvent mouseLocation];
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
    mRotX += [NSEvent mouseLocation].x - mMouseLoc.x;
    mRotY += [NSEvent mouseLocation].y - mMouseLoc.y;
    mMouseLoc = [NSEvent mouseLocation];
    [self setNeedsDisplay:TRUE];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
    mZoom += [theEvent deltaY];
    [self setNeedsDisplay:TRUE];
}

@end
