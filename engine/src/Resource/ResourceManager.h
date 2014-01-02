/*
 *
 * FILE: ResourceManager.h
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

#ifndef RESOURCE_RESOURCEMANAGER_H
#define RESOURCE_RESOURCEMANAGER_H

//Includes:
#include <map>
#include <string>
#include <Block.h>
#include "Stream.h"
#include "ResourceType.h"
#include "HeirarchyNode.h"

#define RESOURCE_FORMAT_VERSION_MAJOR 0
#define RESOURCE_FORMAT_VERSION_MINOR 3

//Enums:

// EResourceLocationID:
// This enum is used to specify where a CRResource instance should look to find the
// requested file to open and parse.
enum EResourceLocationID
{
	RLID_APP = 0, // Get the file from the app bundle.
	RLID_STORAGE, // Get the file from the app's storage.
	RLID_NETWORK, // Get the file from a remote network server.
};

// EResourcePermissionID:
// What you plan to do with the file.
enum EResourcePermissionID
{
	RPID_READONLY = 0,
	RPID_WRITEONLY,
	RPID_APPEND,
	RPID_READWRITE,
	RPID_WRITEREAD,
};

enum EResourceType
{
    RT_3D_MODEL = 0,
	RT_TEXTURE,
    RT_SHADER,
    RT_MATERIAL,
    RT_3D_LEVEL_LAYOUT,
	RT_NUMBER_RESOURCE_TYPES,
};

typedef struct
{
    std::string name;
    CCHeirarchyNode *root;
} SResourceFile;

class CRResourceNode;

//Classes:
class CRResourceManager
{
private:
	// The singleton instance.
	static CRResourceManager *mInstance;
	
	CRResourceManager();
	~CRResourceManager();
	
	// The directory of the bundle
	static std::string mBundleLocation;
    
    // A list of all the resource files we have loaded
    std::vector<SResourceFile> mResourceFiles;
	
public:
	static void Init();
	static CRResourceManager* Get();
    
    // Loads a resource file into the engine
    void loadResourceFile(const std::string &name);
    
    // Releases all the resources loaded from a resource file
    void releaseResourceFile(const std::string &name);
    
    // Finds a resource with a name and a type
    CRResourceNode* findResourceOfType(const std::string &name, EResourceType type);
	
    //private:
	friend class CRResource;
	
	// requestStream:
	// Only CRResource has access to this method. It takes in the file name,
	// the location (see EResourceLocationID), the file permissions (see EResourcePermissionID),
	// and a network address for the server the file will be retrieved from if the location id
	// equals RLID_NETWORK.
	CCStream* requestStream(const std::string &file, 
							EResourceLocationID locationID, 
							EResourcePermissionID permissionID,
							const std::string netAddress = "");
};
#endif