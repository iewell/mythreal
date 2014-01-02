/*
 *
 * FILE: testm3d.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: Reads and manages a model in the m3d format.
 *
 * CREATED ON: 08/06/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#include "testm3d.h"
#include "FileStream.h"
//#include "Material.h"
#include "ResourceManager.h"

CSTestM3D::CSTestM3D()
{
    mRenderer = new CGM3DTestRenderer();
    mTime = 0.0f;
}

CSTestM3D::~CSTestM3D()
{
    delete mRenderer;
}

/*void CSTestM3D::openM3D(std::string file)
{
    CCFileStream *stream = static_cast<CCFileStream*>(CRResourceManager::Get()->requestStream(file, RLID_APP, RPID_READONLY));
    
    unsigned int vertCount = stream->readUInt();
    stream->readUInt();
    stream->readUInt();
    
    void *vertices = malloc(vertCount*sizeof(float)*3);
    stream->read(vertCount*sizeof(float)*3, vertices);
    
    void *uvs = malloc(vertCount*sizeof(float)*2);
    stream->read(vertCount*sizeof(float)*2, uvs);
    
    void *normals = malloc(vertCount*sizeof(float)*3);
    stream->read(vertCount*sizeof(float)*3, normals);
    
    mRenderer->setVertexList(vertices);
    mRenderer->setUVList(uvs);
    mRenderer->setNormalList(normals);
    mRenderer->setVertexCount(vertCount);
    
    unsigned int faceCount;
    SMaterialGroup group;
	unsigned int tag = 0;
    
    // Create our default material
    //CGMaterial *material = new CGMaterial();
    
    //if (mReflecting)
    //{
    //    material->loadShader("ShaderReflect.vsh", "ShaderReflect.fsh");
    //}
    //else
    //{
    //    material->loadShader("Shader.vsh", "Shader.fsh");
    //}
    
    while (stream->getPos() <= stream->getSize()-4)
    {
        tag = stream->readUInt();
        if (tag == 4476485)
        {
            break;
        }
        CGMaterial *material = new CGMaterial();
        stream->seekTo(stream->getPos() - 4);
        faceCount = stream->readUInt();
        group.triangleCount = faceCount;
        group.indices = malloc(faceCount*3*sizeof(short));
        stream->read(faceCount*3*sizeof(short), group.indices);
        material->loadMaterial(stream);
        group.material = material;
        mRenderer->addMatGroup(group);
    }
    
    stream->closeFile();
    delete stream;
}*/

void CSTestM3D::loadFromResource(const std::string &resource)
{
    mRenderer->set3DModelResource(CRResourceManager::Get()->findResourceOfType(resource, RT_3D_MODEL));
}

//CMVector3 CSTestM3D::getPosition()
//{
//    return mPosition;
//}

void CSTestM3D::setReflecting(bool refl)
{
    mReflecting = refl;
    mRenderer->setReflectionEnabled(refl);
}

bool CSTestM3D::isReflecting() const
{
    return mReflecting;
}

void CSTestM3D::prePassRender()
{
    if (mReflecting)
    {
        mVisible = false;
        mRenderer->setWorldMatrix(mWorld);
        mRenderer->updateReflection();
        mVisible = true;
    }
}

void CSTestM3D::setAnimation(std::string &name)
{
    mTime = 0.0f;
    mRenderer->setCurrentAnimation(name);
}

void CSTestM3D::tick(F32 dt)
{
    mTime += dt;
    mRenderer->setCurrentFrame(mTime*30);
}
