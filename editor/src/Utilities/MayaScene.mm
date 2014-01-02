//
//  MayaScene.m
//  Resource Edit
//
//  Created by Ian Ewell on 5/21/11.
//  Copyright 2013 Ian Ewell. All rights reserved.
//

#import "MayaScene.h"
#import "ResourceListModel.h"
#import "MaterialResource.h"

#include <maya/MIOStream.h>
#include <maya/MLibrary.h>
#include <maya/MFileIO.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MDagpath.h>
#include <maya/MDagPathArray.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnSet.h>
#include <maya/MFnTransform.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItGeometry.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MAnimControl.h>
#include <maya/MTime.h>

#include <vector>
#include <list>

// Structure to hold skin data as we loop through the skins. These are then merged together into one master skin.
typedef struct SKIN
{
    NSString *name;
    unsigned int vertexCount;
    SBoneWeight *weights;
    SBoneInfluence *influences;
    NSMutableArray *bones;
} SSkin;

extern CResourceListModel *gModel;

void importBones(CModelRoot *root, MObject &bone);

// This function will recursively load all the bones from a root bone and place them in the model root. It will also load the animation data for these bones
void importBones(CModelRoot *root, MObject &bone)
{
    MFnTransform trans(bone);
    CHeirarchyNode *skeletonRoot = [[root findChildWithName:@"Skeleton" isRecursive:false] retain];
    if (!skeletonRoot)
        return;
    
    CModelSkeletonNode *node = (CModelSkeletonNode*)[[[skeletonRoot findChildWithName:[NSString stringWithUTF8String:trans.name().asUTF8()] isRecursive:true] retain] autorelease];
    if (!node)
    {
        bool hasParent = false;
        for (int i = 0; i < trans.parentCount(); i++)
        {
            if (trans.parent(i).hasFn(MFn::kTransform))
                hasParent = true;
        }
        
        if (!hasParent)
        {
            CModelSkeletonNode *skeletonNode = [[CModelSkeletonNode alloc] init];
            [skeletonNode setName:[NSString stringWithUTF8String:trans.name().asUTF8()]];
            [skeletonRoot insertChild:skeletonNode];
            MMatrix boneMat = trans.transformation().asMatrix();
            float boneTrans[4][4];
            boneMat.get(boneTrans);
            [skeletonNode setInitialMatrix:&boneTrans[0][0]];
            
            // Extract animation data if applicable
            int StartTime = static_cast<int>(MAnimControl::minTime().as(MTime::kPALFrame));
            int EndTime   = static_cast<int>(MAnimControl::maxTime().as(MTime::kPALFrame));
            [skeletonNode setFrameCount:EndTime-StartTime+1];
            MTime currTime = MAnimControl::currentTime();;
            for (int i = StartTime; i <= EndTime; i++)
            {
                MAnimControl::setCurrentTime(MTime(i, MTime::kPALFrame));
                MMatrix anim = trans.transformation().asMatrix();
                float animTrans[4][4];
                anim.get(animTrans);
                CModelSkeletonFrame *frame = [[CModelSkeletonFrame alloc] init];
                [frame setTransformMatrix:&animTrans[0][0]];
                [skeletonNode insertFrame:frame index:i-StartTime];
            }
            MAnimControl::setCurrentTime(currTime);
            
            [skeletonNode release];
        }
        else
        {
            CModelSkeletonNode *parent = (CModelSkeletonNode*)[skeletonRoot findChildWithName:[NSString stringWithUTF8String:MFnTransform(trans.parent(0)).name().asUTF8()] isRecursive:true];
            if (!parent)
            {
                MObject parentObject;
                for (int i = 0; i < trans.parentCount(); i++)
                {
                    if (trans.parent(i).hasFn(MFn::kTransform))
                        parentObject = trans.parent(i);
                    importBones(root, parentObject);
                }
                parent = (CModelSkeletonNode*)[skeletonRoot findChildWithName:[NSString stringWithUTF8String:MFnTransform(trans.parent(0)).name().asUTF8()] isRecursive:true];
                if (!parent)
                    return;
            }
            CModelSkeletonNode *skeletonNode = [[CModelSkeletonNode alloc] init];
            [skeletonNode setName:[NSString stringWithUTF8String:trans.name().asUTF8()]];
            [parent insertChild:skeletonNode];
            MMatrix boneMat = trans.transformation().asMatrix();
            float boneTrans[4][4];
            boneMat.get(boneTrans);
            [skeletonNode setInitialMatrix:&boneTrans[0][0]];
            
            // Extract animation data if applicable
            int StartTime = static_cast<int>(MAnimControl::minTime().as(MTime::kPALFrame));
            int EndTime   = static_cast<int>(MAnimControl::maxTime().as(MTime::kPALFrame));
            [skeletonNode setFrameCount:EndTime-StartTime+1];
            MTime currTime = MAnimControl::currentTime();;
            for (int i = StartTime; i <= EndTime; i++)
            {
                MAnimControl::setCurrentTime(MTime(i, MTime::kPALFrame));
                MMatrix anim = trans.transformation().asMatrix();
                float animTrans[4][4];
                anim.get(animTrans);
                CModelSkeletonFrame *frame = [[CModelSkeletonFrame alloc] init];
                [frame setTransformMatrix:&animTrans[0][0]];
                [skeletonNode insertFrame:frame index:i-StartTime];
            }
            MAnimControl::setCurrentTime(currTime);
            
            [skeletonNode release];
        }
        for (int i = 0; i < trans.childCount(); i++)
        {
            MObject child = trans.child(i);
            if (child.hasFn(MFn::kTransform))
            {
                importBones(root, child);
            }
        }
    }
}

@implementation MayaScene

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

// Big long function that somehow wonders Maya's complicated and erratic APIs and returns a model in our simple and elegant (if not poorly organized) format
- (CModelRoot*)importModel:(NSString*)path
{
    // Init maya
    MStatus status = MLibrary::initialize("ResourceEdit");
    NSLog([NSString stringWithUTF8String: MGlobal::mayaVersion().asUTF8()]);
    const char* filename = [path UTF8String];
    MFileIO::newFile(true);
    status = MFileIO::open(filename);
    
    if (!status)
        return nil;
    
    status = MGlobal::executeCommand( "delete -ch" );
    if ( !stat )
        return nil;
    
    // Set up our root node for our model
    CModelRoot *root = [[CModelRoot alloc] init];
    [root setName:@"RootNode"];
    
    int StartTime = static_cast<int>(MAnimControl::minTime().as(MTime::kPALFrame));
    int EndTime   = static_cast<int>(MAnimControl::maxTime().as(MTime::kPALFrame));
    [root setFrameCount:EndTime-StartTime+1];
    
    // Allocate the theoretical limit of vertices, UVs, and normals
    SVertex *vertices = (SVertex*)malloc(sizeof(SVertex)*65536);
    SVertex *normals = (SVertex*)malloc(sizeof(SVertex)*65536);
    SUV *uvs = (SUV*)malloc(sizeof(SVertex)*65536);
    
    // Allocate for weights and influences
    SBoneWeight *boneWeights = (SBoneWeight*)malloc(sizeof(SBoneWeight)*65536);
    SBoneInfluence *boneInfluences = (SBoneInfluence*)malloc(sizeof(SBoneInfluence)*65536);
    
    // Grand list of all the bones in the scene that the are indexed and referenced per-vertex
    NSMutableArray *boneList = [[NSMutableArray alloc] init];
    
    // Set up counter
    int numElements = 0;
    
    // Set to true if we already alerted the user of non-triangular geometry
    bool triangleAlert = false;
    
    // Skin list
    std::list<SSkin> skins;
    
    // Iterate through to find skin clusters and bones
    MItDependencyNodes itDep(MFn::kInvalid);
    while (!itDep.isDone())
    {
        MObject item = itDep.item();
        if (item.apiType() == MFn::kSkinClusterFilter)
        {
            // Create skeleton root
            CHeirarchyNode *skltn = [root findChildWithName:@"Skeleton" isRecursive:FALSE];
            if (skltn == nil)
            {
                skltn = [[CHeirarchyNode alloc] init];
                [skltn setName:@"Skeleton"];
                [root insertChild:skltn];
            }
            
            MStatus s;
            MFnSkinCluster skin(item);
            
            MDagPathArray joints;
            skin.influenceObjects( joints, &s );
            for (int i = 0; i < joints.length(); i++)
            {
                if(joints[i].node().hasFn(MFn::kTransform))
                {
                    MObject child = joints[i].node();
                    importBones(root, child);
                }
            }
            
            // Index the bones into an array to be indexed as influence objects
            NSMutableArray *bones = [[NSMutableArray alloc] init];
            for (int i = 0; i < joints.length(); i++)
            {
                if(joints[i].node().hasFn(MFn::kTransform))
                {
                    CModelSkeletonNode *bone = (CModelSkeletonNode*)[skltn findChildWithName:[NSString stringWithUTF8String:MFnTransform(joints[i].node()).name().asUTF8()] isRecursive:TRUE];
                    if ([bone getBoneID] == -1)
                        [bone setBoneID:i];
                    [bones addObject:bone];
                }
                else
                {
                    CModelSkeletonNode *dummy = [[CModelSkeletonNode alloc] init];
                    [dummy setName:@"DUMMY_BONE"];
                    [dummy setBoneID:i];
                    [bones addObject:dummy];
                }
            }
            
            // Actually try and get the skin data
            unsigned int geomCount = skin.numOutputConnections();
            for (unsigned int i = 0; i < geomCount; i++)
            {
                SSkin skindat;
                skindat.bones = bones;
                MDagPath skinPath;
                skin.getPathAtIndex(i, skinPath);
                skindat.name = [NSString stringWithUTF8String:skinPath.partialPathName().asUTF8()];
                
                MItGeometry it(skinPath);
                unsigned int numVerts = it.count();
                SBoneWeight *rawWeights = new SBoneWeight[numVerts];
                memset(rawWeights, 0, sizeof(SBoneWeight)*numVerts);
                SBoneInfluence *rawInfluences = new SBoneInfluence[numVerts];
                memset(rawInfluences, 0, sizeof(SBoneInfluence)*numVerts);
                unsigned short *rawInfluenceCounts = new unsigned short[numVerts];
                for (int i = 0; i < numVerts; i++)
                {
                    MObject comp = it.component();
                    MDoubleArray weight;
                    MIntArray influence;
                    unsigned int numInfluences;
                    skin.getWeights(skinPath, comp, weight, numInfluences);
                    if (numInfluences != 0 && it.index() == i && !it.isDone())
                    {
                        SBoneWeight bWeight;
                        SBoneInfluence bInfluence;
                        float w0 = 1.0f;
                        int j = 0;
                        int influences = 0;
                        while (j < numInfluences && influences < BONES_PER_VERTEX)
                        {
                            if (weight[j] >= 0.0001f)
                            {
                                //bWeight.weights[influences] = weight[j];
                                w0 -= weight[j];
                                rawInfluenceCounts[i] = 0;
                                influences++;
                            }
                            j++;
                        }
                        //bWeight.weights[0] = w0;
                        //rawWeights[i] = bWeight;
                        //rawInfluences[i] = bInfluence;
                        it.next();
                    }
                }
                
                // Loop through all the points affected by all the influences to genertate influences per vertex
                for (int i = 0; i < joints.length(); i++)
                {
                    MDoubleArray weights;
                    MSelectionList points;
                    skin.getPointsAffectedByInfluence(joints[i], points, weights);
                    
                    MObject component;
                    MDagPath dpath;
                    for (int idx = 0; idx < points.length(); idx++)
                    {
                        points.getDagPath(idx, dpath, component);
                        if (dpath.fullPathName() == skinPath.fullPathName())
                            break;
                    }
                    if (dpath.fullPathName() != skinPath.fullPathName())
                        continue;
                    MItMeshVertex vert(dpath, component);
                    int element = 0;
                    for (; !vert.isDone(); vert.next())
                    {
                        unsigned int index = vert.index();
                        if (rawInfluenceCounts[index] < BONES_PER_VERTEX)
                        {
                            rawInfluences[index].index[rawInfluenceCounts[index]] = [[bones objectAtIndex:i] getBoneID];
                            rawWeights[index].weights[rawInfluenceCounts[index]] = weights[element];
                            rawInfluenceCounts[index]++;
                            element++;
                        }
                    }
                }
                
                // Build weights list to match up face based vertex lists
                MFnMesh mesh(skinPath);
                unsigned int instanceNumber = skinPath.instanceNumber();
                MObjectArray sets;
                MObjectArray comps;
                mesh.getConnectedSetsAndMembers(instanceNumber, sets, comps, true);
                mesh.getConnectedSetsAndMembers(instanceNumber, sets, comps, true);
                skindat.influences = new SBoneInfluence[65536];
                skindat.weights = new SBoneWeight[65536];
                skindat.vertexCount = 0;
                for (int s = 0; s < sets.length(); s++)
                {
                    MObject comp = comps[s];
                    MItMeshPolygon poly(skinPath, comp);
                    int currentPoly = skindat.vertexCount/3;
                    skindat.vertexCount += poly.count()*3;
                    for (; !poly.isDone(); poly.next())
                    {
                        if (poly.polygonVertexCount() != 3)
                        {
                           
                            NSBeep();
                            NSRunAlertPanel(@"Non-triangular Geometry", @"The model importer has encountered non-triangular geometry. Please triangulate the model and try again", @"OK", nil, nil);
                            return NULL;
                          
                        }
                        for (int idx = 0; idx < 3; idx++)
                        {
                            unsigned int index = poly.vertexIndex(idx);
                            skindat.influences[currentPoly*3+idx] = rawInfluences[index];
                            skindat.weights[currentPoly*3+idx] = rawWeights[index];
                        }
                        currentPoly++;
                    }

                }
                
                skins.push_back(skindat);
                delete [] rawWeights;
                delete [] rawInfluences;
            }
        }
        itDep.next();
    }
    
    // iterate through all the nodes in the DAG, and print out their names
    MItDag dagIter( MItDag::kBreadthFirst, MFn::kInvalid, &status );
    for ( ; !dagIter.isDone(); dagIter.next())
    {
        MDagPath dagPath;
        status = dagIter.getPath( dagPath );
        
        cerr << "Found DAG Node: "
        << dagPath.fullPathName().asChar()
        << endl;
        
        if ( status )
        {
            MFnDagNode dagNode( dagPath, &status );
            
            // If we have some bones, load them up
            if ( dagPath.hasFn(MFn::kSkinClusterFilter))
            {
                                
            }
            
            // this object cannot be intermediate, and it must be a mesh
            // and it can't be a transform.
            // Intermediate objects are special meshes
            // that are not drawn used for mesh morphs or something.
            if ( dagNode.isIntermediateObject()) continue;
            if ( !dagPath.hasFn( MFn::kMesh )) continue;
            if ( dagPath.hasFn( MFn::kTransform )) continue;
            
            MFnMesh fnMesh( dagPath );
            
            // Create a new node for our mesh
            CHeirarchyNode *group = [[CHeirarchyNode alloc] init];
            [group setName:[NSString stringWithCString:dagNode.partialPathName().asUTF8() encoding:NSUTF8StringEncoding]];
            [root insertChild:group];
            
            // See if this mesh has a skin attached
            bool hasSkin = false;
            SSkin skin;
            for (std::list<SSkin>::iterator it = skins.begin(); it != skins.end(); it++)
            {
                if ([(*it).name compare:[NSString stringWithUTF8String:dagNode.partialPathName().asUTF8()]] == NSOrderedSame)
                {
                    hasSkin = true;
                    skin = (*it);
                    break;
                }
            }
            
            // If we have a skin, we need to map the weights to the vertices of this mesh and reindex the influence objects
            if (hasSkin)
            {
                // Copy all the weights into the current offset in the weights and influences buffers
                memcpy(&boneWeights[numElements], skin.weights, skin.vertexCount*sizeof(SBoneWeight));
                memcpy(&boneInfluences[numElements], skin.influences, skin.vertexCount*sizeof(SBoneInfluence));
                
                // Now we need to merge all the bones into grand bone list and update the per-vertex influence references
                for (CModelSkeletonNode *bone in skin.bones)
                {
                    unsigned int oldBoneID = [bone getBoneID];
                    int boneIndex = [boneList indexOfObject:bone];
                    if (boneIndex == -1)
                    {
                        boneIndex = [boneList count];
                        [boneList addObject:bone];
                    }
                    if (boneIndex != oldBoneID)
                    {
                        [bone setBoneID:boneIndex];
                        for (int i = numElements; i < numElements + skin.vertexCount; i++)
                        {
                            for (int j = 0; j < BONES_PER_VERTEX; j++)
                            {
                                if (boneInfluences[i].index[j] == oldBoneID)
                                    boneInfluences[i].index[j] = boneIndex;
                            }
                        }
                    }
                }
            }
            
            unsigned int instanceNumber = dagPath.instanceNumber();
            
            MObjectArray	sets;
            MObjectArray	comps;		
            
            fnMesh.getConnectedSetsAndMembers(instanceNumber, sets, comps, true);
            for (int s = 0; s < sets.length(); s++)
            {
                MObject	set = sets[s];
                MObject comp = comps[s];
                
                MFnSet				fnSet(set);
                MFnDependencyNode	dnSet(set);
                
                MObject				ssAttr = dnSet.attribute(MString("surfaceShader"));
                MPlug				ssPlug(set, ssAttr);
                
                MPlugArray			srcPlugArray;
                ssPlug.connectedTo(srcPlugArray, true, false);
                
                if (srcPlugArray.length() == 0)
                    continue;
                
                MObject	shaderObject = srcPlugArray[0].node();
                MFnDependencyNode shader(shaderObject);
                
                // Set up our indices table
                SIndex *indices = (SIndex*)malloc(sizeof(SIndex)*65536);
                
                // And our counter
                int indexCounter = 0;
                
                // Set up our material group
                CModelMaterialGroup *mat = [[CModelMaterialGroup alloc] init];
                [mat setName:[NSString stringWithCString:shader.name().asUTF8() encoding:NSUTF8StringEncoding]];
                [group insertChild:mat];
                
                // Create a material resource for this
                [gModel addResourceOfType:RT_MATERIAL withName:[NSString stringWithCString:shader.name().asUTF8() encoding:NSUTF8StringEncoding]];
                
                // Find that material resource
                CHeirarchyNode *materials = [gModel getResourceTypeAtIndex:RT_MATERIAL];
                CResourceNode *matResource = (CResourceNode*)[materials findChildWithName:[NSString stringWithCString:shader.name().asUTF8() encoding:NSUTF8StringEncoding] isRecursive:FALSE];
                //CMaterialResource *materialResource = (CMaterialResource*)[matResource getResource];
                
                // Assign it to our material group
                [mat setMaterialData:matResource];
            
                // Iterate through our polygons
                MItMeshPolygon poly(dagPath, comp);
                for (; !poly.isDone(); poly.next())
                {
                    if (poly.polygonVertexCount() != 3)
                    {
                        if (!triangleAlert)
                        {
                            NSBeep();
                            NSRunAlertPanel(@"Non-triangular Geometry", @"The model importer has encountered non-triangular geometry. The resulting model may have undesireable results.", @"OK", nil, nil);
                            triangleAlert = true;
                        }
                        continue;
                    }
                    
                    // Extract the stuff
                    for (int i = 0; i < 3; i++)
                    {
                        // Extract Vertex
                        MPoint vert = poly.point(i, MSpace::kWorld);
                        float temp[4];
                        vert.get(temp);
                        vertices[numElements].x = temp[0]*1;
                        vertices[numElements].y = temp[1]*1;
                        vertices[numElements].z = temp[2]*1;
                        
                        // Extract Normal
                        MVector normal;
                        poly.getNormal(i, normal, MSpace::kWorld);
                        double temp2[3];
                        normal.get(temp2);
                        normals[numElements].x = (float)temp2[0];
                        normals[numElements].y = (float)temp2[1];
                        normals[numElements].z = (float)temp2[2];
                        
                        // Extract UV
                        float uv[2];
                        poly.getUV(i, uv);
                        
                        uvs[numElements].u = uv[0];
                        uvs[numElements].v = uv[1];
                        
                        switch (i)
                        {
                            case 0:
                                indices[indexCounter].i1 = numElements;
                                break;
                            case 1:
                                indices[indexCounter].i2 = numElements;
                                break;
                            case 2:
                                indices[indexCounter].i3 = numElements;
                                break;
                        }
                        
                        if (!hasSkin)
                        {
                            memset(&boneWeights[numElements], 0, sizeof(SBoneWeight));
                            memset(&boneWeights[numElements], 0, sizeof(SBoneInfluence));
                        }
                        
                        // Increment our element count
                        numElements++;
                    }
                    
                    indexCounter++;
                }
                
                // We have everything extracted from this mesh. We need to reallocate it and copy it into new structures for our material groups
                SIndex *vertIndices = (SIndex*)malloc(sizeof(SIndex)*indexCounter);
                SIndex *normalIndices = (SIndex*)malloc(sizeof(SIndex)*indexCounter);
                SIndex *uvIndices = (SIndex*)malloc(sizeof(SIndex)*indexCounter);
                
                // Copy our data
                memcpy(vertIndices, indices, indexCounter*sizeof(SIndex));
                memcpy(normalIndices, indices, indexCounter*sizeof(SIndex));
                memcpy(uvIndices, indices, indexCounter*sizeof(SIndex));
                
                // Release the original
                free(indices);
                
                // And hand them off to our material group
                [mat setTriangleCount:indexCounter];
                [mat setVertIndices:vertIndices];
                [mat setNormalIndices:normalIndices];
                [mat setUVIndices:uvIndices];
            }
        }
    }
    
    // No need to shrink the vertex tables. They will be shrunk automatically when we reindex them
    [root setVertices:vertices];
    [root setNormals:normals];
    [root setUVs:uvs];
    
    [root setBones:boneList];
    [root setWeights:boneWeights];
    [root setInfluences:boneInfluences];
    
    if ([boneList count] > 0)
        [root setIsSkinnes:true];
    else
        [root setIsSkinnes:false];
    
    [root reindex];
    
    // now shut down Maya, or if you want to process another file,
    // just make another call to MFileIO::newFile(), and MFileIO::open()
    //MLibrary::cleanup();
    return root;
}

@end
