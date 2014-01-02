//
//  LevelLayoutResource.cpp
//  mythreal
//
//  Created by Ian Ewell on 8/3/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "LevelLayoutResource.h"

void CRSceneObject::readFromStream(CCFileStream *stream)
{
    mModelName = stream->readString();
    mPosition.setX(stream->readFloat());
    mPosition.setY(stream->readFloat());
    mPosition.setZ(stream->readFloat());
    readChildrenFromStream(stream);
}

CR2DPhysicsLine::CR2DPhysicsLine()
{
    mType = "2DPhysicsLine";
}

CR2DPhysicsLine::~CR2DPhysicsLine()
{
    
}

void CR2DPhysicsLine::readFromStream(CCFileStream *stream)
{
    mLine[0].setX(stream->readDouble());
    mLine[0].setY(stream->readDouble());
    mLine[1].setX(stream->readDouble());
    mLine[1].setY(stream->readDouble());
    
    mLineType = (ELineType)stream->readUInt();
    
    readChildrenFromStream(stream);
}

void CRArea::readFromStream(CCFileStream *stream)
{
    mMin.setX(stream->readFloat());
    mMin.setY(stream->readFloat());
    mMax.setX(stream->readFloat());
    mMax.setY(stream->readFloat());
    
    readChildrenFromStream(stream);
}

void CRLight::readFromStream(CCFileStream *stream)
{
    mPosition.setX(stream->readFloat());
    mPosition.setY(stream->readFloat());
    
    mIntensity = stream->readFloat();
    
    mColor.setX((float)stream->readUChar()/255.0f);
    mColor.setY((float)stream->readUChar()/255.0f);
    mColor.setZ((float)stream->readUChar()/255.0f);
    
    readChildrenFromStream(stream);
}

CRLevelLayoutResource::CRLevelLayoutResource()
{
    mSceneRoot = NULL;
}

CRLevelLayoutResource::~CRLevelLayoutResource()
{
    if (mSceneRoot)
        delete mSceneRoot;
}

void CRLevelLayoutResource::readFromStream(CCFileStream *stream)
{
    std::string name = stream->readString();
    stream->readString();
    
    mSceneRoot = new CCHeirarchyNode;
    mSceneRoot->setName(name);
    mSceneRoot->readFromStream(stream);
}