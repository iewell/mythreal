//
//  Material.cpp
//  mythreal
//
//  Created by Ian Ewell on 7/1/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

/*#include <iostream>
#include "Material.h"
#include "ResourceManager.h"
#include "FileStream.h"
#include "SceneManager.h"

CGMaterialUniform::CGMaterialUniform()
{
    mUniformLocation = 0;
}

void CGMaterialUniform::setUniformName(const std::string &name)
{
    mUniformName = name;
}

std::string CGMaterialUniform::getUniformName() const
{
    return mUniformName;
}

EUniformType CGMaterialUniform::getUniformType() const
{
    return mUniformType;
}

void CGMaterialUniform::setUniformValueType(U32 type)
{
    mUniformValueType = type;
}

U32 CGMaterialUniform::getUniformValueType() const
{
    return mUniformValueType;
}

void CGMaterialUniform::setupUniform(GLint shader)
{
    mUniformLocation = glGetUniformLocation(shader, mUniformName.c_str());
}

////////////////////////////////////////////////////////

CGMaterialFloatUniform::CGMaterialFloatUniform()
{
    mUniformType = UT_FLOAT;
}

void CGMaterialFloatUniform::applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc)
{
    glUniform1f(mUniformLocation, mFloatValue);
}

void CGMaterialFloatUniform::loadUniformFromStream(CCFileStream *stream, GLint shaderProg)
{
    mUniformName = stream->readString();
    mUniformValueType = stream->readUInt();
    mFloatValue = stream->readFloat();
}

////////////////////////////////////////////////////////

CGMaterialVector2Uniform::CGMaterialVector2Uniform()
{
    mUniformType = UT_VECTOR2;
}

void CGMaterialVector2Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc)
{
    glUniform2f(mUniformLocation, mVector.getX(), mVector.getY());
}

void CGMaterialVector2Uniform::loadUniformFromStream(CCFileStream *stream, GLint shaderProg)
{
    mUniformName = stream->readString();
    mUniformValueType = stream->readUInt();
    mVector = CMVector2(stream->readFloat(), stream->readFloat());
}

////////////////////////////////////////////////////////

CGMaterialVector3Uniform::CGMaterialVector3Uniform()
{
    mUniformType = UT_VECTOR3;
}

void CGMaterialVector3Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc)
{
    glUniform3f(mUniformLocation, mVector.getX(), mVector.getY(), mVector.getZ());
}

void CGMaterialVector3Uniform::loadUniformFromStream(CCFileStream *stream, GLint shaderProg)
{
    mUniformName = stream->readString();
    mUniformValueType = stream->readUInt();
    mVector = CMVector3(stream->readFloat(), stream->readFloat(), stream->readFloat());
}

////////////////////////////////////////////////////////

CGMaterialVector4Uniform::CGMaterialVector4Uniform()
{
    mUniformType = UT_VECTOR4;
}

void CGMaterialVector4Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc)
{
    glUniform4f(mUniformLocation, mVector.getX(), mVector.getY(), mVector.getZ(), mVector.getW());
}

void CGMaterialVector4Uniform::loadUniformFromStream(CCFileStream *stream, GLint shaderProg)
{
    mUniformName = stream->readString();
    mUniformValueType = stream->readUInt();
    mVector = CMVector4(stream->readFloat(), stream->readFloat(), stream->readFloat(), stream->readFloat());
}

////////////////////////////////////////////////////////

CGMaterialMatrix4Uniform::CGMaterialMatrix4Uniform()
{
    mUniformType = UT_MATRIX4;
}

void CGMaterialMatrix4Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc)
{
    CMMatrix4x4 modelView = gGlobalSceneManager->getViewMatrix();
    CMMatrix4x4 view = gGlobalSceneManager->getViewMatrix();
    modelView.multiply(modelMatrix);

    switch (mUniformValueType)
    {
        case VTM4_MODEL_VIEW:
            // Assemble our modelView matrix
            glUniformMatrix4fv(mUniformLocation, 1, GL_FALSE, modelView.transpose().getArray());
            break;
        case VTM4_MODEL:
            glUniformMatrix4fv(mUniformLocation, 1, GL_FALSE, modelMatrix.transpose().getArray());
            break;
        case VTM4_VIEW:
            glUniformMatrix4fv(mUniformLocation, 1, GL_FALSE, gGlobalSceneManager->getViewMatrix().transpose().getArray());
            break;
        case VTM4_PROJECTION:
            glUniformMatrix4fv(mUniformLocation, 1, GL_FALSE, gGlobalSceneManager->getProjectionMatrix().transpose().getArray());
            break;
    }
}

void CGMaterialMatrix4Uniform::loadUniformFromStream(CCFileStream *stream, GLint shaderProg)
{
    mUniformName = stream->readString();
    mUniformValueType = stream->readUInt();
    F32 matrix[16];
    stream->read(16*4, &matrix[0]);
}

////////////////////////////////////////////////////////

CGMaterialSampler2DUniform::CGMaterialSampler2DUniform()
{
    mUniformType = UT_SAMPLER2D;
}

void CGMaterialSampler2DUniform::applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc)
{
    glActiveTexture(GL_TEXTURE0 + *textureInc);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glUniform1i(mUniformLocation, *textureInc);
    textureInc[0]++;
}

void CGMaterialSampler2DUniform::loadUniformFromStream(CCFileStream *stream, GLint shaderProg)
{
    mUniformName = stream->readString();
    mUniformValueType = stream->readUInt();
    mTextureWidth = stream->readUInt();
    mTextureHeight = stream->readUInt();
    mTextureData = malloc(mTextureWidth*mTextureHeight*4);
    stream->read(mTextureWidth*mTextureHeight*4, mTextureData);
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTextureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    free(mTextureData);
}

////////////////////////////////////////////////////////

CGMaterial::CGMaterial()
{
    
}

CGMaterial::~CGMaterial()
{
    
}

bool CGMaterial::loadShader(const std::string &vShader, const std::string &fShader)
{
    GLuint vert, frag;
    GLint status;
    
    mShaderProgram = glCreateProgram();
    
    // Load up our shaders
    CCFileStream *vertShaderStream = static_cast<CCFileStream*>(CRResourceManager::Get()->requestStream(vShader, RLID_APP, RPID_READONLY));
    CCFileStream *fragShaderStream = static_cast<CCFileStream*>(CRResourceManager::Get()->requestStream(fShader, RLID_APP, RPID_READONLY));
    if (!vertShaderStream || !fragShaderStream)
    {
        if (vertShaderStream)
            delete vertShaderStream;
        if (fragShaderStream)
            delete fragShaderStream;
        return false;
    }
    
    GLchar *vertShaderSource = (GLchar*)malloc(vertShaderStream->getSize());
    GLchar *fragShaderSource = (GLchar*)malloc(fragShaderStream->getSize());
    if (!vertShaderSource || !fragShaderSource)
    {
        if (vertShaderStream)
            delete vertShaderStream;
        if (fragShaderStream)
            delete fragShaderStream;
        if (vertShaderSource)
            free(vertShaderSource);
        if (fragShaderSource)
            free(fragShaderSource);
        return false;
    }
    
    vertShaderStream->read(vertShaderStream->getSize(), vertShaderSource);
    fragShaderStream->read(fragShaderStream->getSize(), fragShaderSource);
    
    GLint vertShaderSize = vertShaderStream->getSize();
    GLint fragShaderSize = fragShaderStream->getSize();
    
    // We loaded them, free our streams
    delete vertShaderStream;
    delete fragShaderStream;
    
    // Now compile our shaders
    const GLchar *vShaderPtr;
    vShaderPtr = vertShaderSource;
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vShaderPtr, &vertShaderSize);
    glCompileShader(vert);
    GLint logLength;
    glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(vert, logLength, &logLength, log);
        printf("Shader compile log:\n%s", log);
        free(log);
    }
    
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(vert);
        if (vertShaderSource)
            free(vertShaderSource);
        if (fragShaderSource)
            free(fragShaderSource);
        return false;
    }
    
    const GLchar *fShaderPtr;
    fShaderPtr = fragShaderSource;
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShaderPtr, &fragShaderSize);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(frag, logLength, &logLength, log);
        printf("Shader compile log:\n%s", log);
        free(log);
    }
    
    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(vert);
        glDeleteShader(frag);
        if (vertShaderSource)
            free(vertShaderSource);
        if (fragShaderSource)
            free(fragShaderSource);
        return false;
    }
    
    // Attach to our program
    glAttachShader(mShaderProgram, vert);
    glAttachShader(mShaderProgram, frag);
    
    // Bind the vertex attributes to our program
    glBindAttribLocation(mShaderProgram, ATTRIB_VERTEX, "position");
    glBindAttribLocation(mShaderProgram, ATTRIB_UV, "texCoord");
    glBindAttribLocation(mShaderProgram, ATTRIB_NORMAL, "normal");
    
    // Now link the shader
    glLinkProgram(mShaderProgram);
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
    
    // Get our uniforms
    mModelViewMatrixUniformLoc = glGetUniformLocation(mShaderProgram, "modelViewMatrix");
    mProjectionMatrixUniformLoc = glGetUniformLocation(mShaderProgram, "projMatrix");
    mModelMatrixUniformLoc = glGetUniformLocation(mShaderProgram, "modelMatrix");
    mEyePosUniformLoc = glGetUniformLocation(mShaderProgram, "eyePos");
    
    if (status == 0)
    {
        glDeleteShader(vert);
        glDeleteShader(frag);
        if (vertShaderSource)
            free(vertShaderSource);
        if (fragShaderSource)
            free(fragShaderSource);
        return false;
    }
    
    // Clean up and return
    glDeleteShader(vert);
    glDeleteShader(frag);
    if (vertShaderSource)
        free(vertShaderSource);
    if (fragShaderSource)
        free(fragShaderSource);
    
    return true;
}

void CGMaterial::addTextureUniform(const char *uniformName, eTextureType textureType, GLuint textureID)
{
    sTextureUniform texuniform;
    texuniform.textureID = textureID;
    texuniform.uniformID = glGetUniformLocation(mShaderProgram, uniformName);
    texuniform.textureType = textureType;
    mTextureUniformList.push_back(texuniform);
}

void CGMaterial::loadMaterial(CCFileStream *stream)
{
    U32 uniformCount = stream->readUInt();
    for (int i = 0; i < uniformCount; i++)
    {
        U32 uniformType = stream->readUInt();
        CGMaterialUniform *uniform;
        switch (uniformType)
        {
            case UT_FLOAT:
                uniform = new CGMaterialFloatUniform();
                break;
            case UT_VECTOR2:
                uniform = new CGMaterialVector2Uniform();
                break;
            case UT_VECTOR3:
                uniform = new CGMaterialVector3Uniform();
                break;
            case UT_VECTOR4:
                uniform = new CGMaterialVector4Uniform();
                break;
            case UT_MATRIX4:
                uniform = new CGMaterialMatrix4Uniform();
                break;
            case UT_SAMPLER2D:
                uniform = new CGMaterialSampler2DUniform();
                break;
            default:
                break;
        }
        uniform->loadUniformFromStream(stream, 0);
        mMaterialUniformList.push_back(uniform);
    }
    
    // Read the shaders
    std::string vertShader = stream->readString();
    std::string fragShader = stream->readString();
    
    // Set up shaders
    GLuint vert, frag;
    GLint status;
    mShaderProgram = glCreateProgram();
    
    // Now compile our shaders
    const GLchar *vShaderPtr;
    vShaderPtr = vertShader.c_str();
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
    fShaderPtr = fragShader.c_str();
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
    glAttachShader(mShaderProgram, vert);
    glAttachShader(mShaderProgram, frag);
    
    // Bind the vertex attributes to our program
    glBindAttribLocation(mShaderProgram, ATTRIB_VERTEX, "position");
    glBindAttribLocation(mShaderProgram, ATTRIB_UV, "texCoord");
    glBindAttribLocation(mShaderProgram, ATTRIB_NORMAL, "normal");
    
    // Now link the shader
    glLinkProgram(mShaderProgram);
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
    
    // Clean up
    glDeleteShader(vert);
    glDeleteShader(frag);
    
    // Set up the uniforms
    for (std::list<CGMaterialUniform*>::iterator it = mMaterialUniformList.begin(); it != mMaterialUniformList.end(); it++)
        (*it)->setupUniform(mShaderProgram);
}

void CGMaterial::applyMaterial(CMMatrix4x4 &modelMatrix)
{
    glUseProgram(mShaderProgram);
    /*glUniformMatrix4fv(mProjectionMatrixUniformLoc, 1, GL_FALSE, gGlobalSceneManager->getProjectionMatrix().transpose().getArray());
    
    // Assemble our modelView matrix
    CMMatrix4x4 modelView = gGlobalSceneManager->getViewMatrix();
    CMMatrix4x4 view = gGlobalSceneManager->getViewMatrix();
    CMVector4 eyePos = view.getColumn(3);
    modelView.multiply(modelMatrix);
    
    glUniformMatrix4fv(mModelViewMatrixUniformLoc, 1, GL_FALSE, modelView.transpose().getArray());
    glUniformMatrix4fv(mModelMatrixUniformLoc, 1, GL_FALSE, modelMatrix.transpose().getArray());
    glUniform3f(mEyePosUniformLoc, -eyePos.getX(), -eyePos.getY(), -eyePos.getZ());
    
    int i = 0;
    for (std::list<sTextureUniform>::iterator it = mTextureUniformList.begin(); it != mTextureUniformList.end(); it++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        switch ((*it).textureType)
        {
            case TT_2D:
                glBindTexture(GL_TEXTURE_2D, (*it).textureID);
                break;
                
            case TT_CUBE:
                glBindTexture(GL_TEXTURE_CUBE_MAP, (*it).textureID);
                break;
        }
        glUniform1i((*it).uniformID, i);
        i++;
    }*
    
    int *increment = new int;
    increment[0] = 0;
    
    for (std::list<CGMaterialUniform*>::iterator it = mMaterialUniformList.begin(); it != mMaterialUniformList.end(); it++)
        (*it)->applyUniform(modelMatrix, increment);
}*/