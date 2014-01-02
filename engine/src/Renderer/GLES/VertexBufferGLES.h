//
//  VertexBufferGLES.h
//  mythreal
//
//  Created by Ian Ewell on 3/25/13.
//
//

#ifndef __mythreal__VertexBufferGLES__
#define __mythreal__VertexBufferGLES__

#include <iostream>
#include "VertexBuffer.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class CGRVertexBufferGLES : public CGRVertexBuffer
{
protected:
    GLuint mVBO;
    GLuint mVAO;
    
    void _allocate(U32 size);
    void _bind();
    void _unbind();
    
public:
    CGRVertexBufferGLES(){};
    ~CGRVertexBufferGLES();
};

#endif /* defined(__mythreal__VertexBufferGLES__) */
