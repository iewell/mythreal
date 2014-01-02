/*
 *
 * FILE: Resource.cpp
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

//Includes:
#include "Resource.h"
#include "TextureResource.h"
#include "ShaderResource.h"
#include "MaterialResource.h"
#include "3DModelResource.h"
#include "LevelLayoutResource.h"

CRResourceNode::CRResourceNode()
{
    mType = "Resource";
    mResource = NULL;
    mRefCount = 0;
}

CRResourceNode::~CRResourceNode()
{
    
}

void CRResourceNode::loadResource()
{
    if (mRefCount == 0)
    {
        if (mResource)
            mResource->loadResource();
    }
    mRefCount++;
}

void CRResourceNode::releaseResource()
{
    mRefCount--;
    if (mRefCount == 0)
    {
        if (mResource)
        {
            mResource->releaseResource();
            mResource = NULL;
        }
        delete this;
    }
}

void CRResourceNode::readFromStream(CCFileStream *stream)
{
    std::string resourceType = stream->readString();
    
    if (resourceType.compare("3DModel") == 0)
        mResource = new CR3DModelResource;
    else if (resourceType.compare("Texture") == 0)
        mResource = new CRTextureResource;
    else if (resourceType.compare("Shader") == 0)
        mResource = new CRShaderResource;
    else if (resourceType.compare("Material") == 0)
        mResource = new CRMaterialResource;
    else if (resourceType.compare("3DSceneLayout") == 0)
        mResource = new CRLevelLayoutResource;
    else
        mResource = new CRResource;
    
    mResource->readFromStream(stream);
    
    readChildrenFromStream(stream);
}

////////////////////////////////////////////////////////////////////////

CRResource::CRResource()
{
    mType = "NULLResource";
    mRefCount = 0;
}

CRResource::~CRResource()
{
    
}

void CRResource::loadResource()
{
    mRefCount++;
}

void CRResource::releaseResource()
{
    mRefCount--;
    if (mRefCount == 0)
        delete this;
}

void CRResource::readFromStream(CCFileStream *stream)
{
    
}