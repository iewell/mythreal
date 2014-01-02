/*
 *
 * FILE: Resource.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Models
 *
 * PURPOSE: Generic resource classes.
 *
 * CREATED ON: 07/20/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import "HeirarchyNode.h"

@class CResource;

@interface CResourceNode : CHeirarchyNode
{
	CResource *mResource;
	NSPanel *mEditor;
}

- (id)init;
- (void)dealloc;

- (void)setResource:(CResource*)resource;
- (CResource*)getResource;
- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc;
- (void)writeToStream:(CFileStream*)stream;

@end


@interface CResource : NSObject 
{
	NSString *mType;
}

- (id)init;
- (void)serializeXML:(NSXMLElement*)parent bundleLocation:(NSString*) loc name:(NSString*)name;
- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;

@end
