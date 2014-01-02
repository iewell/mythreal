//
//  ShaderResource.cpp
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "ShaderResource.h"

CRShaderResource::CRShaderResource()
{
    mRefCount = 0;
    mProgram = 0;
    mType = "Shader";
}

CRShaderResource::~CRShaderResource()
{
    
}

void CRShaderResource::loadResource()
{
    if (mRefCount == 0)
    {
        // Set up shaders
        GLuint vert, frag;
        GLint status;
        mProgram = glCreateProgram();
        
        // Now compile our shaders
        const GLchar *vShaderPtr;
        vShaderPtr = mVertexShader.c_str();
        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vShaderPtr, NULL);
        glCompileShader(vert);
        GLint logLength;
        glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(vert, logLength, &logLength, log);
            printf("Vertex Shader compile log:\n%s", log);
            free(log);
        }
        
        glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
        if (status == 0)
        {
            glDeleteShader(vert);
        }
        
        const GLchar *fShaderPtr;
        fShaderPtr = mFragmentShader.c_str();
        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fShaderPtr, NULL);
        glCompileShader(frag);
        glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(frag, logLength, &logLength, log);
            printf("Fragment Shader compile log:\n%s", log);
            free(log);
        }
        
        glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
        if (status == 0)
        {
            glDeleteShader(vert);
            glDeleteShader(frag);
        }
        
        // Attach to our program
        glAttachShader(mProgram, vert);
        glAttachShader(mProgram, frag);
        
        // Bind the vertex attributes to our program
        glBindAttribLocation(mProgram, ATTRIB_VERTEX, "position");
        glBindAttribLocation(mProgram, ATTRIB_UV, "texCoord");
        glBindAttribLocation(mProgram, ATTRIB_NORMAL, "normal");
        glBindAttribLocation(mProgram, ATTRIB_WEIGHTS, "weights");
        glBindAttribLocation(mProgram, ATTRIB_INFLUENCES, "influences");
        
        // Now link the shader
        glLinkProgram(mProgram);
        glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
        
        // Clean up
        glDeleteShader(vert);
        glDeleteShader(frag);
    }
    mRefCount++;
}

void CRShaderResource::releaseResource()
{
    mRefCount--;
    if (mRefCount == 0)
    {
        glDeleteProgram(mProgram);
        delete this;
    }
}

void CRShaderResource::readFromStream(CCFileStream *stream)
{
    // Read the shaders
    mVertexShader = stream->readString();
    mFragmentShader = stream->readString();
    
    U32 uniformCount = stream->readUInt();
    
    // Skip the uniform data. It is only relevent in the editor
    for (int i = 0; i < uniformCount; i++)
    {
        stream->readString();
        stream->readUInt();
    }
}