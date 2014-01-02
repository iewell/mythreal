//
//  IndexBuffer.h
//  mythreal
//
//  Created by Ian Ewell on 4/9/13.
//
//

#ifndef __mythreal__IndexBuffer__
#define __mythreal__IndexBuffer__

#include <iostream>
#include "SafeArray.h"
#include "VertexBuffer.h"

typedef struct
{
    U16 i1;
    U16 i2;
    U16 i3;
} SIndex;

typedef CCSafeArray<SIndex> CIndexArray;

// Index buffer supports unsigned 16-bit indices currently for triangles only
class CGRIndexBuffer : public CCReferenceBase
{
protected:
    U32 mElementCount;
    EVBOType mVBOType;
    
    bool mAllocated;
    
    virtual void _allocate() {};
    virtual void _fill(const void *data) {};

    CGRIndexBuffer(){}
    
public:
    CGRIndexBuffer(CIndexArray array, EVBOType type);
    virtual ~CGRIndexBuffer();
};

#endif /* defined(__mythreal__IndexBuffer__) */
