//
//  TextureResource.h
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_TextureResource_h
#define mythreal_TextureResource_h

#include "Resource.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class CRTextureResource : public CRResource
{
protected:
    U32 mWidth;
    U32 mHeight;
    
    void *mData;
    
    GLuint mTextureID;
    
public:
    CRTextureResource();
    virtual ~CRTextureResource();
    
    void loadResource();
    void releaseResource();
    
    GLuint getTextureID()
    {
        return mTextureID;
    }
    
    void readFromStream(CCFileStream *stream);
};

#endif
