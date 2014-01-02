//
//  IndexBuffer.cpp
//  mythreal
//
//  Created by Ian Ewell on 4/9/13.
//
//

#include "IndexBuffer.h"

CGRIndexBuffer::CGRIndexBuffer(CIndexArray array, EVBOType type)
{
    mElementCount = array.getSize();
    mVBOType = type;
    _allocate();
    _fill(&array[0]);
}