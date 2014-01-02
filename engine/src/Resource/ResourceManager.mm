/*
 *
 * FILE: ResourceManager.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Resource
 *
 * PURSPOSE: The resource manager is responsible for managing resources both locally and on a remote server.
 *
 * CREATED ON: 06/28/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

//Includes:
#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#include "ResourceManager.h"
#include "ResourceType.h"
#include "FileStream.h"

//CRResourceManager implementation begin:

// Static variable declarations.
CRResourceManager* CRResourceManager::mInstance;
std::string CRResourceManager::mBundleLocation;


CRResourceManager::CRResourceManager()
{
	mInstance = NULL;
}

CRResourceManager::~CRResourceManager()
{
	delete mInstance;
}

void CRResourceManager::Init()
{
	mInstance = new CRResourceManager();
	
    const char *bundleURL = [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];
    
	mBundleLocation = bundleURL;
	//delete bundleURL;
	std::cout << "Bundle URL: " << mBundleLocation << "\n";
}

CRResourceManager* CRResourceManager::Get()
{
	if (mInstance)
		return mInstance;
	
	return NULL;
}

void CRResourceManager::loadResourceFile(const std::string &name)
{
    // Get a stream
    CCFileStream *stream = static_cast<CCFileStream*>(requestStream(name + ".mrrc", RLID_APP, RPID_READONLY));
    char magic[4];
    stream->read(4, magic);
    
    U16 versionMajor = stream->readUShort();
    U16 versionMinor = stream->readUShort();
    
    if (versionMajor != RESOURCE_FORMAT_VERSION_MAJOR || versionMinor != RESOURCE_FORMAT_VERSION_MINOR)
    {
        std::cout << "ERROR: Resource file " << name << " is of an incompatible version\n";
        return;
    }
    
    SResourceFile resource;
    resource.name = name;
    resource.root = new CCHeirarchyNode;
    std::string rootName = stream->readString();
    stream->readString();
    resource.root->setName(rootName);
    resource.root->readFromStream(stream);
    mResourceFiles.push_back(resource);
    delete stream;
}

void CRResourceManager::releaseResourceFile(const std::string &name)
{
    // TODO: Implement when resource management becomes an issue :P
}

CRResourceNode* CRResourceManager::findResourceOfType(const std::string &name, EResourceType type)
{
    for (std::vector<SResourceFile>::iterator it = mResourceFiles.begin(); it < mResourceFiles.end(); it++)
    {
        // Find it
        CRResourceNode *node = (CRResourceNode*)((*it).root->getChildAtIndex(type)->findChildWithName(name));
        if (node)
            return node;
        
    }
    return NULL;
}

CCStream* CRResourceManager::requestStream(const std::string &file, 
										   EResourceLocationID locationID, 
										   EResourcePermissionID permissionID,
										   const std::string netAddress)
{
	// TODO: Use the location ID once the paths for app and storage are stored in the manager.
	// TODO: Implement network access and downloading assets from remote server for testing.
	CCFileStream *stream = new CCFileStream();
	
	//Parse the enum and convert them to options used by fopen().
	std::string options;
	switch (permissionID)
	{
		case RPID_READONLY:
			options = "r";
			break;
		case RPID_WRITEONLY:
			options = "w";
			break;
		case RPID_APPEND:
			options = "a";
			break;
		case RPID_READWRITE:
			options = "r+";
			break;
		case RPID_WRITEREAD:
			options = "w+";
			break;
		default:
			options = "r";
			break;
	}
	
	//Open the file using our options.
	std::string path = mBundleLocation + "/" + file;
	if (stream->openFile(path, (char*)options.c_str()))
		return stream;
	
	delete stream;
	std::cout << "ResourceManager.cpp::requestStream():ERROR: path " << path << " does not exist!\n";
	return NULL;
}