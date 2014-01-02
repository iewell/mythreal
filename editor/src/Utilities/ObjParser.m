/*
 *
 * FILE: ObjParser.m
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

#import <stdio.h>
#import "ObjParser.h"

// Warning: This code is obsolete, sloppily written, and a general disgrace against mankind, and yet it somehow
// once worked. If you value your sanity, it may be best to not attempt to read this code.

@implementation CObjParser

- (id)init 
{
    if ((self = [super init])) 
    {
        mVertexCount = 0;
        mUVCount = 0;
        mNormalCount = 0;
    }
    
    return self;
}

- (void)dealloc 
{
    // Clean-up code here
    
    [super dealloc];
}

- (CModelRoot*)parse:(NSString*)path
{
    // Reset everything
    mVertexCount = 0;
    mUVCount = 0;
    mNormalCount = 0;
    
    // Number of faces in current material group
    unsigned short numFaces = 0;

    FILE *fp = fopen([path UTF8String], "r");
    if (!fp)
    {
        NSRunAlertPanel(@"Could not open Obj!", @"The file does not exist!", @"OK", nil, nil);
        return nil;
    }
    
    // First, go through the file and count the vertices, normals, etc. so we can allocate the right size
    char token = 0;
    int lineNumber = 1;
    
    // Current material and group
    CModelMaterialGroup *currentMaterial = nil;
    CHeirarchyNode *currentGroup = nil;
    
    // Create the root model node
    CModelRoot* modelRoot = [[CModelRoot alloc] init];
    [modelRoot setName:@"RootNode"];
    
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    while (ftell(fp) < fileSize)
    {
        fread(&token, 1, 1, fp);
        switch (token)
        {
            case '#':
                while (token != '\n')
                    fread(&token, 1, 1, fp);
                lineNumber++;
                break;
                
            case 'v':
                fread(&token, 1, 1, fp);
                switch (token)
                {
                    case ' ':
                    case 20:
                        mVertexCount += 1;
                        break;
                        
                    case 't':
                        mUVCount += 1;
                        break;
                        
                    case 'n':
                        mNormalCount += 1;
                        break;
                        
                    default:
                        NSRunAlertPanel(@"Error parsing Obj.", @"An error occured while parsing the Obj file. See console for details", @"OK", nil, nil);
                        printf("Obj Parsing Error on line %d: Unknown command v%c.\n", lineNumber, token);
                        
                        if (currentGroup)
                            [currentGroup release];
                        if (currentMaterial)
                            [currentMaterial release];
                        
                        [modelRoot release];
                        return nil;
                        break;
                }
                
                while (token != '\n')
                    fread(&token, 1, 1, fp);
                lineNumber++;
                break;
            
            case 'g':
                fread(&token, 1, 1, fp);
                fread(&token, 1, 1, fp);
                
                int length = 0;
                while ((token != ' ') && (token != '\n'))
                {
                    fread(&token, 1, 1, fp);
                    length++;
                }
                
                char *groupName = (char*)malloc(length+1);
                fseek(fp, -length - 1, SEEK_CUR);
                fscanf(fp, "%s", groupName);
                
                CHeirarchyNode *groupNode = [[CHeirarchyNode alloc] init];
                [groupNode setName:[[[NSString alloc] initWithUTF8String:groupName] autorelease]];
                [modelRoot insertChild:groupNode];
                
                if (currentGroup)
                    [currentGroup release];
                currentGroup = groupNode;
                if (currentMaterial)
                    [currentMaterial release];
                currentMaterial = nil;
                free(groupName);
                break;
                
            case 'u':
                // The compiler for some reson gives an error if we declare a variable first
                token = token;
                char *buffer = (char*)malloc(6);
                fread(buffer, 5, 1, fp);
                
                // zero-terminate the end
                buffer[5] = 0;
                
                if (strcmp(buffer, "semtl"))
                {
                    fread(&token, 1, 1, fp);
                    fread(&token, 1, 1, fp);
                    
                    int length = 0;
                    while ((token != ' ') && (token != '\n'))
                    {
                        fread(&token, 1, 1, fp);
                        length++;
                    }
                    
                    char *mtlName = (char*)malloc(length+1);
                    fseek(fp, -length - 1, SEEK_CUR);
                    fscanf(fp, "%s", mtlName);
                    
                    if (!currentGroup)
                    {
                        currentGroup = [[modelRoot findChildWithName:@"DefaultGroup" isRecursive:FALSE] retain];
                        if (!currentGroup)
                        {
                            currentGroup = [[CHeirarchyNode alloc] init];
                            [currentGroup setName:@"DefaultGroup"];
                            [modelRoot insertChild:currentGroup];
                        }
                    }
                    
                    if (currentMaterial)
                    {
                        // Give the current material its total face count
                        [currentMaterial setTriangleCount:numFaces];
                        [currentMaterial release];
                    }
                    
                    currentMaterial = (CModelMaterialGroup*)[[currentGroup findChildWithName:[[[NSString alloc] initWithUTF8String:mtlName] autorelease] isRecursive:FALSE] retain];
                    
                    if (!currentMaterial)
                    {
                        currentMaterial = [[CModelMaterialGroup alloc] init];
                        [currentMaterial setName:[[[NSString alloc] initWithUTF8String:mtlName] autorelease]];
                        [currentGroup insertChild:currentMaterial];
                    }
                    numFaces = [currentMaterial getTriangleCount];
                    buffer = buffer;
                }
                
                free(buffer);
                break;
                
            case 'f':
                if (!currentMaterial)
                {
                    if (!currentGroup)
                    {
                        currentGroup = [[modelRoot findChildWithName:@"DefaultGroup" isRecursive:FALSE] retain];
                        if (!currentGroup)
                        {
                            currentGroup = [[CHeirarchyNode alloc] init];
                            [currentGroup setName:@"DefaultGroup"];
                            [modelRoot insertChild:currentGroup];
                        }
                    }
                    currentMaterial = [[CModelMaterialGroup alloc] init];
                    [currentMaterial setName:@"DefaultMaterial"];
                    [currentGroup insertChild:currentMaterial];
                    numFaces = 0;
                }
                numFaces++;
                
                while (token != '\n')
                    fread(&token, 1, 1, fp);

                break;
                
            case ' ':
            case 20:
                break;
            case '\n':
                lineNumber++;
                break;
                
            default:
                break;
        }
    }
    
    // Set the last material's triangle count
    if (currentMaterial)
        [currentMaterial setTriangleCount:numFaces];
    
    printf("Obj Parse results:\n");
    printf("Number of lines: %d\n", lineNumber);
    printf("Vertex count:    %d\n", mVertexCount);
    printf("UV count:        %d\n", mUVCount);
    printf("Normal count     %d\n", mNormalCount);
    printf("Group Count      %lu\n", [modelRoot numberOfChildren]);
    
    [modelRoot setVertexCount:mVertexCount];
    [modelRoot setUVCount:mUVCount];
    [modelRoot setNormalCount:mNormalCount];
    
    // Allocate room for our stuff
    SVertex *verts = (SVertex*)malloc(mVertexCount*sizeof(SVertex));
    SUV *uvs = (SUV*)malloc(mUVCount*sizeof(SUV));
    SVertex *normals = (SVertex*)malloc(mNormalCount*sizeof(SVertex));
    
    // Reset everything again
    mVertexCount = 0;
    mUVCount = 0;
    mNormalCount = 0;
    numFaces = 0;
    
    if (currentGroup)
        [currentGroup release];
    currentGroup = nil;
    
    if (currentMaterial)
        [currentMaterial release];
    currentMaterial = nil;
    
    // Create pointers to our current face data
    SIndex *currentVertices = NULL;
    SIndex *currentUVs = NULL;
    SIndex *currentNormals = NULL;
    
    // Now let's actually read the data
    fseek(fp, 0, SEEK_SET);
    while (ftell(fp) < fileSize)
    {
        fread(&token, 1, 1, fp);
        switch (token)
        {
            case '#':
                while (token != '\n')
                    fread(&token, 1, 1, fp);
                lineNumber++;
                break;
                
            case 'v':
                fread(&token, 1, 1, fp);
                switch (token)
                {
                    case ' ':
                    case 20:
                        fscanf(fp, "%f %f %f", &verts[mVertexCount].x,
                                               &verts[mVertexCount].y,
                                               &verts[mVertexCount].z);
                        mVertexCount++;
                        break;
                    
                    case 't':
                        fscanf(fp, " %f %f", &uvs[mUVCount].u,
                                            &uvs[mUVCount].v);
                        mUVCount++;
                        break;
                    
                    case 'n':
                        fscanf(fp, " %f %f %f", &normals[mNormalCount].x,
                                               &normals[mNormalCount].y,
                                               &normals[mNormalCount].z);
                        mNormalCount++;
                        break;
                    
                    default:
                        NSRunAlertPanel(@"Error parsing Obj.", @"An error occured while parsing the Obj file. See console for details", @"OK", nil, nil);
                        printf("Obj Parsing Error on line %d: Unknown command v%c.\n", lineNumber, token);
                        if (currentGroup)
                            [currentGroup release];
                        if (currentMaterial)
                            [currentMaterial release];
                        
                        [modelRoot release];
                        return nil;
                        break;
                }
                
                while (token != '\n')
                    fread(&token, 1, 1, fp);
                lineNumber++;
                break;
            
            case 'g':
                fread(&token, 1, 1, fp);
                fread(&token, 1, 1, fp);
                
                int length = 0;
                while ((token != ' ') && (token != '\n'))
                {
                    fread(&token, 1, 1, fp);
                    length++;
                }
                
                char *groupName = (char*)malloc(length+1);
                fseek(fp, -length - 1, SEEK_CUR);
                fscanf(fp, "%s", groupName);
                
                if (currentGroup)
                    [currentGroup release];
                currentGroup = [[modelRoot findChildWithName:[[[NSString alloc] initWithUTF8String:groupName] autorelease] isRecursive:FALSE] retain];
                break;
                
            case 'u':
                // The compiler for some reason gives an error if we declare a variable first
                token = token;
                char *buffer = (char*)malloc(6);
                fread(buffer, 5, 1, fp);
                
                // zero-terminate the end
                buffer[5] = 0;
                
                if (strcmp(buffer, "semtl"))
                {
                    fread(&token, 1, 1, fp);
                    fread(&token, 1, 1, fp);
                    
                    int length = 0;
                    while ((token != ' ') && (token != '\n'))
                    {
                        fread(&token, 1, 1, fp);
                        length++;
                    }
                    
                    char *mtlName = (char*)malloc(length+1);
                    fseek(fp, -length - 1, SEEK_CUR);
                    fscanf(fp, "%s", mtlName);
                    
                    if (!currentGroup)
                        currentGroup = [[modelRoot findChildWithName:@"DefaultGroup" isRecursive:FALSE] retain];
                    
                    if (currentMaterial)
                        [currentMaterial release];
                
                    
                    currentMaterial = (CModelMaterialGroup*)[[currentGroup findChildWithName:[[[NSString alloc] initWithUTF8String:mtlName] autorelease] isRecursive:FALSE] retain];
                    
                    if ([currentMaterial getVertIndices])
                    {
                        currentVertices = [currentMaterial getVertIndices];
                        currentUVs = [currentMaterial getUVIndices];
                        currentNormals = [currentMaterial getNormalIndices];
                    }
                    else
                    {
                        currentVertices = (SIndex*)malloc([currentMaterial getTriangleCount]*sizeof(SIndex));
                        [currentMaterial setVertIndices:currentVertices];
                        
                        currentUVs = (SIndex*)malloc([currentMaterial getTriangleCount]*sizeof(SIndex));
                        [currentMaterial setUVIndices:currentUVs];
                        
                        currentNormals = (SIndex*)malloc([currentMaterial getTriangleCount]*sizeof(SIndex));
                        [currentMaterial setNormalIndices:currentNormals];
                     }

                    numFaces = 0;
                }
                
                free(buffer);
                break;
            
            case 'f':
                if (!currentMaterial)
                {
                    if (!currentGroup)
                        currentGroup = [[modelRoot findChildWithName:@"DefaultGroup" isRecursive:FALSE] retain];
    
                    currentMaterial = (CModelMaterialGroup*)[[currentGroup findChildWithName:@"DefaultMaterial" isRecursive:FALSE] retain];
                    
                    if ([currentMaterial getVertIndices])
                    {
                        currentVertices = [currentMaterial getVertIndices];
                        currentUVs = [currentMaterial getUVIndices];
                        currentNormals = [currentMaterial getNormalIndices];
                    }
                    else
                    {
                        currentVertices = (SIndex*)malloc([currentMaterial getTriangleCount]*sizeof(SIndex));
                        [currentMaterial setVertIndices:currentVertices];
                        
                        currentUVs = (SIndex*)malloc([currentMaterial getTriangleCount]*sizeof(SIndex));
                        [currentMaterial setUVIndices:currentUVs];
                        
                        currentNormals = (SIndex*)malloc([currentMaterial getTriangleCount]*sizeof(SIndex));
                        [currentMaterial setNormalIndices:currentNormals];
                    }
                    numFaces = 0;

                }
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentVertices[numFaces].i1);
                currentVertices[numFaces].i1 -= 1;
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentUVs[numFaces].i1);
                currentUVs[numFaces].i1 -= 1;
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentNormals[numFaces].i1);
                currentNormals[numFaces].i1 -= 1;
                
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentVertices[numFaces].i2);
                currentVertices[numFaces].i2 -= 1;
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentUVs[numFaces].i2);
                currentUVs[numFaces].i2 -= 1;
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentNormals[numFaces].i2);
                currentNormals[numFaces].i2 -= 1;
                
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentVertices[numFaces].i3);
                currentVertices[numFaces].i3 -= 1;
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentUVs[numFaces].i3);
                currentUVs[numFaces].i3 -= 1;
                fread(&token, 1, 1, fp);
                fscanf(fp, "%hu", &currentNormals[numFaces].i3);
                currentNormals[numFaces].i3 -= 1;
                
                numFaces++;
                //printf("Face %d parsed\n", numFaces);
                break;
                
            case ' ':
            case 20:
                break;
            case '\n':
                lineNumber++;
                break;
                
            default:
                break;
        }
    }
    
    if (currentGroup)
        [currentGroup release];
    if (currentMaterial)
        [currentMaterial release];
    
    // Put the data in
    [modelRoot setVertices:verts];
    [modelRoot setUVs:uvs];
    [modelRoot setNormals:normals];
    
    fclose(fp);
    
    // Reindex the model root so it can be rendered
    [modelRoot reindex];
    
    [modelRoot autorelease];
    return modelRoot;
}

@end
