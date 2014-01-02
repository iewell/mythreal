//
//  TextureResource.cpp
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "TextureResource.h"

CRTextureResource::CRTextureResource()
{
    mType = "Texture";
    mTextureID = 0;
    mData = NULL;
    mRefCount = 0;
}

CRTextureResource::~CRTextureResource()
{
    if (mData)
        free(mData);
}

void CRTextureResource::loadResource()
{
    if (mRefCount == 0)
    {
        glGenTextures(1, &mTextureID);
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);
        free(mData);
        mData = NULL;
    }
    mRefCount++;
}

void CRTextureResource::releaseResource()
{
    mRefCount--;
    if (mRefCount == 0)
    {
        glDeleteTextures(1, &mTextureID);
        delete this;
    }
}

void CRTextureResource::readFromStream(CCFileStream *stream)
{
    mWidth = stream->readUInt();
    mHeight = stream->readUInt();
    mData = malloc(mWidth*mHeight*4);
    stream->read(mWidth*mHeight*4, mData);
}