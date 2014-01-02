/*
 *
 * FILE: Resource.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Resource
 *
 * PURSPOSE: A resource is for a single file. It sets up a stream and handles reading and writing a file.
 *
 * CREATED ON: 06/29/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef RESOURCE_RESOURCE_H
#define RESOURCE_RESOURCE_H

//Includes:
#include "ResourceManager.h"
#include "ResourceType.h"
#include "HeirarchyNode.h"

//Classes:
class CRResource;

class CRResourceNode : public CCHeirarchyNode
{
protected:
    CRResource *mResource;
    U32 mRefCount;
    
public:
    CRResourceNode();
    virtual ~CRResourceNode();
    
    // This is a wrapper for the resource contained inside. This should
    // be called rather than the one inside a resource
    void loadResource();
    void releaseResource();
    CRResource* getResource()
    {
        return mResource;
    }
    
    void readFromStream(CCFileStream *stream);
};

class CRResource
{
protected:
    EResourceType mResourceType;
    std::string mType;
    
    // This keeps track of how many things are using the resource
    U32 mRefCount;
    
public:
    CRResource();
    virtual ~CRResource();
    
    // This function will setup the resource and load all the components
    // The resource must have been read befre this is called
    virtual void loadResource();
    
    // This is called when something is finished with the resource. If nothing
    // is using it, the resource will be deallocated
    virtual void releaseResource();
    
    virtual void readFromStream(CCFileStream *stream);
    
    EResourceType getResourceType()
    {
        return mResourceType;
    }
};

#endif