//
//  MaterialResource.cpp
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "MaterialResource.h"
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

void CGMaterialFloatUniform::applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc)
{
    switch (mUniformValueType)
    {
        case VTF_CUSTOM:
            glUniform1f(mUniformLocation, mFloatValue);
            break;
        case VTF_LIGHT_COUNT:
            if (area)
                glUniform1f(mUniformLocation, area->getNumberOfChildren());
            break;
        case VTF_LIGHT_INTENSITY:
            if (area)
            {
                int i = 0;
                F32 list[3];
                std::vector<CCHeirarchyNode*> lights = area->getChildren();
                std::vector<CCHeirarchyNode*>::iterator lightIt;
                for (lightIt = lights.begin(); lightIt < lights.end(); lightIt++)
                {
                    list[i] = static_cast<CRLight*>((*lightIt))->getIntensity();
                    i++;
                }
                glUniform1fv(mUniformLocation, 3, list);
            }
            break;
    }
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

void CGMaterialVector2Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc)
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

void CGMaterialVector3Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc)
{
    switch (mUniformValueType)
    {
        case VTV3_CUSTOM:
            glUniform3f(mUniformLocation, mVector.getX(), mVector.getY(), mVector.getZ());
            break;
        case VTV3_LIGHTPOS:
            if (area)
            {
                int i = 0;
                F32 list[9];
                std::vector<CCHeirarchyNode*> lights = area->getChildren();
                std::vector<CCHeirarchyNode*>::iterator lightIt;
                for (lightIt = lights.begin(); lightIt < lights.end(); lightIt++)
                {
                    list[i] = static_cast<CRLight*>((*lightIt))->getPosition().getX();
                    i++;
                    list[i] = static_cast<CRLight*>((*lightIt))->getPosition().getY();
                    i++;
                    list[i] = 0.0f;
                    i++;
                }
                glUniform3fv(mUniformLocation, 3, list);
            }
            break;
        case VTV3_LIGHT_COLOR:
            if (area)
            {
                int i = 0;
                F32 list[9];
                std::vector<CCHeirarchyNode*> lights = area->getChildren();
                std::vector<CCHeirarchyNode*>::iterator lightIt;
                for (lightIt = lights.begin(); lightIt < lights.end(); lightIt++)
                {
                    list[i] = static_cast<CRLight*>((*lightIt))->getColor().getX();
                    i++;
                    list[i] = static_cast<CRLight*>((*lightIt))->getColor().getY();
                    i++;
                    list[i] = static_cast<CRLight*>((*lightIt))->getColor().getZ();
                    i++;
                }
                glUniform3fv(mUniformLocation, 3, list);
            }
            break;
    }
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

void CGMaterialVector4Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc)
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

void CGMaterialMatrix4Uniform::applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc)
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
        case VTM4_BONES:
            glUniformMatrix4fv(mUniformLocation, bones->getSize(), GL_FALSE, (GLfloat*)&bones[0]);
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
    mTexture = NULL;
}

CGMaterialSampler2DUniform::~CGMaterialSampler2DUniform()
{
    if (mTexture)
        mTexture->releaseResource();
}

void CGMaterialSampler2DUniform::applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc)
{
    if (!mTexture)
    {
        mTexture = (CRResourceNode*)CRResourceManager::Get()->findResourceOfType(mTextureRef, RT_TEXTURE);
        if (!mTexture)
        {
            std::cout << "ERROR: Unable to find referenced texture " << mTextureRef << ".\n";
            return;
        }
        mTexture->loadResource();
    }
    glActiveTexture(GL_TEXTURE0 + *textureInc);
    glBindTexture(GL_TEXTURE_2D, static_cast<CRTextureResource*>(mTexture->getResource())->getTextureID());
    glUniform1i(mUniformLocation, *textureInc);
    textureInc[0]++;
}

void CGMaterialSampler2DUniform::loadUniformFromStream(CCFileStream *stream, GLint shaderProg)
{
    mUniformName = stream->readString();
    mUniformValueType = stream->readUInt();
    mTextureRef = stream->readString();
}

////////////////////////////////////////////////////////

CRMaterialResource::CRMaterialResource()
{
    mRefCount = 0;
    mType = "Material";
    mShaderRef = "";
}

CRMaterialResource::~CRMaterialResource()
{
    
}

void CRMaterialResource::loadResource()
{
    if (mRefCount == 0)
    {
        mShader = (CRResourceNode*)CRResourceManager::Get()->findResourceOfType(mShaderRef, RT_SHADER);
        if (!mShader)
        {
            std::cout << "ERROR: Could not find shader " << mShaderRef <<"\n";
            return;
        }
        mShader->loadResource();
        
        for (std::list<CGMaterialUniform*>::iterator it = mMaterialUniformList.begin(); it != mMaterialUniformList.end(); it++)
            (*it)->setupUniform(static_cast<CRShaderResource*>(mShader->getResource())->getProgram());
    }
    mRefCount++;
}

void CRMaterialResource::releaseResource()
{
    mRefCount--;
    if (mRefCount == 0)
    {
        mShader->releaseResource();
        delete this;
    }
}

void CRMaterialResource::applyMaterial(CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones)
{
    glUseProgram(static_cast<CRShaderResource*>(mShader->getResource())->getProgram());
    
    int *increment = new int;
    increment[0] = 0;
    
    for (std::list<CGMaterialUniform*>::iterator it = mMaterialUniformList.begin(); it != mMaterialUniformList.end(); it++)
        (*it)->applyUniform(modelMatrix, area, bones, increment);
    
    glValidateProgram(static_cast<CRShaderResource*>(mShader->getResource())->getProgram());
    GLint status;
    glGetProgramiv(static_cast<CRShaderResource*>(mShader->getResource())->getProgram(), GL_VALIDATE_STATUS, &status);
    
    delete increment;
}

void CRMaterialResource::readFromStream(CCFileStream *stream)
{
    U32 uniformCount = stream->readUInt();
    for (int i = 0; i < uniformCount; i++)
    {
        U32 uniformType = stream->readUInt();
        CGMaterialUniform *uniform = NULL;
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
        if (uniform)
        {
            uniform->loadUniformFromStream(stream, 0);
            mMaterialUniformList.push_back(uniform);
        }
    }
    mShaderRef = stream->readString();
}