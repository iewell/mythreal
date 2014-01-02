/*
 *
 * FILE: ResourceType.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Resource
 *
 * PURSPOSE: The resource type allows for coders to add new resource handlers and add them to the resource manager.
 *
 * CREATED ON: 06/28/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef RESOURCE_RESOURCETYPE_H
#define RESOURCE_RESOURCETYPE_H

//Includes:
#include <Block.h>
#include "Stream.h"

//Classes:

// CRResourceType:
// Use this to create a new loadable resource for the recource manager.
// You create a resource type by instantiating this class. You then set
// the read and write block with the equivlent code needed to parse or write
// the resource type. If you are reading, you get a stream as an argument and
// you do the parsing and return a pointer to an instance of an abritary type
// representing the data.
class CRResourceType
{
private:
	friend class CRResourceManager;
	friend class CRResource;
	
	void*(^mReadBlock)(CCStream *stream);
	void (^mWriteBlock)(CCStream *stream, void *write);
	
	// Only used by the resource manager.
	void* executeRead(CCStream *stream);
	void executeWrite(CCStream *stream, void *write);
	
public:
	CRResourceType();
	~CRResourceType();
	
	void setReadBlock(void*(^block)(CCStream *stream));
	void setWriteBlock(void(^block)(CCStream *stream, void *write));
};

#endif