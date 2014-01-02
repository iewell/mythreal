/*
 *
 * FILE: 3DModelResource.m
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

#import <OpenGL/gl.h>
#import "3DSceneObjectInstance.h"
#import "ResourceListModel.h"

extern CResourceListModel *gModel;

@implementation C3DSceneObjectInstance

- (id)init 
{
    mNodeType = @"3DSceneObjectInstance";
    if ((self = [super init])) 
    {
        // Initialization code here.
        mMaterialGroups = [[NSMutableArray alloc] init];
        m3DModelName = nil;
        m3DModel = nil;
        
        mPosition[0] = 0.0f;
        mPosition[1] = 0.0f;
        mPosition[2] = 0.0f;
    }
    
    return self;
}

- (void)dealloc 
{
    if (m3DModel)
    {
        [m3DModelRoot release];
        [m3DModel release];
    }
    
    [super dealloc];
}

- (void)set3DModel:(CResourceNode*)model
{
    m3DModel = [model retain];
    m3DModelRoot = [[(CModelResource*)[model getResource] getRoot] retain];
    
    for (CHeirarchyNode *node in [m3DModelRoot getChildren])
    {
        for (CModelMaterialGroup *group in [node getChildren])
        {
            if ([[group getNodeType] compare:@"MaterialGroup"] == NSOrderedSame)
                [mMaterialGroups addObject:group];
        }
    }
}

- (void)render
{
    if (!m3DModel && m3DModelName)
    {
        CResourceNode *node = (CResourceNode*)[[gModel getResourceTypeAtIndex:RT_3D_MODEL] findChildWithName:m3DModelName isRecursive:FALSE];
        [self set3DModel:node];
    }
                                
    if (m3DModelRoot)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
        glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
        glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, [m3DModelRoot getVertices]);
        glNormalPointer(GL_FLOAT, 0, [m3DModelRoot getNormals]);
        glTexCoordPointer(2, GL_FLOAT, 0, [m3DModelRoot getUVs]);
        for (CModelMaterialGroup *group in mMaterialGroups)
            glDrawElements(GL_TRIANGLES, [group getTriangleCount]*3, GL_UNSIGNED_SHORT, (GLvoid*)[group getVertIndices]);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, [m3DModelRoot getVertices]);
        glNormalPointer(GL_FLOAT, 0, [m3DModelRoot getNormals]);
        glTexCoordPointer(2, GL_FLOAT, 0, [m3DModelRoot getUVs]);
        for (CModelMaterialGroup *group in mMaterialGroups)
            glDrawElements(GL_TRIANGLES, [group getTriangleCount]*3, GL_UNSIGNED_SHORT, (GLvoid*)[group getVertIndices]);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    // Write the reference
    [stream writeString:[m3DModel name]];
    
    // Write out the model's position
    [stream writeData:&mPosition[0] withLength:sizeof(float)*3];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    // Read the reference
    m3DModelName = [[stream readString] retain];
    
    // Read the position
    if ([stream getVersionMinor] >= 2)
    {
        [stream readDataIntoBuffer:mPosition withLenght:sizeof(float)*3];
    }
    else
    {
        mPosition[0] = 0.0f;
        mPosition[1] = 0.0f;
        mPosition[2] = 0.0f;
    }
    
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    return 3;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    if ([(NSString*)[aTableColumn identifier] compare:@"Property"] == NSOrderedSame)
    {
        switch (rowIndex)
        {
            case 0:
                return @"PositionX";
                break;
            case 1:
                return @"PositionY";
                break;
            case 2:
                return @"PositionZ";
                break;
            default:
                return @"";
                break;
        };
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"Value"] == NSOrderedSame)
    {
        switch (rowIndex)
        {
            case 0:
                return [NSString stringWithFormat:@"%f", mPosition[0]];
                break;
            case 1:
                return [NSString stringWithFormat:@"%f", mPosition[1]];
                break;
            case 2:
                return [NSString stringWithFormat:@"%f", mPosition[2]];
                break;
            default:
                return @"";
                break;
        };
    }
    return nil;
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    
    if ([(NSString*)[aTableColumn identifier] compare:@"Value"] == NSOrderedSame)
    {
        switch (rowIndex)
        {
            case 0:
                mPosition[0] = [anObject floatValue];
                break;
            case 1:
                mPosition[1] = [anObject floatValue];
                break;
            case 2:
                mPosition[2] = [anObject floatValue];
                break;
            default:
                break;
        };
        
    }
    
}

@end
