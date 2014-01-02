/*
 *
 * FILE: ResourceType.cpp
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

//Includes:
#include "ResourceType.h"

// CRResourceType Implementation begin:
CRResourceType::CRResourceType()
{
	
}

CRResourceType::~CRResourceType()
{
	if (mReadBlock)
		Block_release(mReadBlock);
	if (mWriteBlock)
		Block_release(mWriteBlock);
}

void* CRResourceType::executeRead(CCStream *stream)
{
	return mReadBlock(stream);
}

void CRResourceType::executeWrite(CCStream *stream, void *write)
{
	mWriteBlock(stream, write);
}

void CRResourceType::setReadBlock(void*(^block)(CCStream *stream))
{
	mReadBlock = Block_copy(block);
}

void CRResourceType::setWriteBlock(void(^block)(CCStream *stream, void *write))
{
	mWriteBlock = Block_copy(block);
}