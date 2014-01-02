//
//  ShaderResource.h
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_ShaderResource_h
#define mythreal_ShaderResource_h

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "Resource.h"
#include "VertexBuffer.h"

class CRShaderResource : public CRResource
{
private:
    std::string mVertexShader;
    std::string mFragmentShader;
    GLint mProgram;
    
public:
    CRShaderResource();
    virtual ~CRShaderResource();
    
    void loadResource();
    void releaseResource();
    
    GLint getProgram()
    {
        return mProgram;
    }
    
    void readFromStream(CCFileStream *stream);
};

#endif
