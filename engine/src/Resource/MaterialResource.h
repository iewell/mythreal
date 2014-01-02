//
//  MaterialResource.h
//  mythreal
//
//  Created by Ian Ewell on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_MaterialResource_h
#define mythreal_MaterialResource_h

#include "Resource.h"
#include "Matrix.h"
#include "TextureResource.h"
#include "ShaderResource.h"
#include "LevelLayoutResource.h"
#include "SafeArray.h"

#include <list>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

// The uniform types the engine supports
typedef enum
{
    UT_FLOAT,
    UT_VECTOR2,
    UT_VECTOR3,
    UT_VECTOR4,
    UT_MATRIX4,
    UT_SAMPLER2D,
    UT_SAMPLER_CUBE,
} EUniformType;

// The different kinds of values put in each uniform
typedef enum
{
    VTF_CUSTOM = 0,
    VTF_DT,
    VTF_LIGHT_COUNT,
    VTF_LIGHT_INTENSITY,
} EFloatValueType;

typedef enum
{
    VTV2_CUSTOM = 0,
} EVector2ValueType;

typedef enum
{
    VTV3_CUSTOM = 0,
    VTV3_EYE_POS,
    VTV3_LIGHTPOS,
    VTV3_LIGHT_COLOR,
} EVector3ValueType;

typedef enum
{
    VTV4_CUSTOM = 0,
} EVector4ValueType;

typedef enum
{
    VTM4_CUSTOM = 0,
    VTM4_MODEL_VIEW,
    VTM4_MODEL,
    VTM4_VIEW,
    VTM4_PROJECTION,
    VTM4_BONES,
} EMatrix4ValueType;

enum eTextureType
{
    TT_2D,
    TT_CUBE,
};

class CGMaterialUniform
{
protected:
    std::string mUniformName;
    EUniformType mUniformType;
    U32 mUniformValueType;
    GLint mUniformLocation;
    
public:
    CGMaterialUniform();
    
    void setUniformName(const std::string &name);
    std::string getUniformName() const;
    
    EUniformType getUniformType() const;
    
    void setUniformValueType(U32 type);
    U32 getUniformValueType() const;
    
    void setupUniform(GLint shader);
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc = NULL) = 0;
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg) = 0;
};

class CGMaterialFloatUniform : public CGMaterialUniform
{
private:
    F32 mFloatValue;
    
public:
    CGMaterialFloatUniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialVector2Uniform : public CGMaterialUniform
{
private:
    CMVector2 mVector;
    
public:
    CGMaterialVector2Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialVector3Uniform : public CGMaterialUniform
{
private:
    CMVector3 mVector;
    
public:
    CGMaterialVector3Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialVector4Uniform : public CGMaterialUniform
{
private:
    CMVector4 mVector;
    
public:
    CGMaterialVector4Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialMatrix4Uniform : public CGMaterialUniform
{
private:
    CMMatrix4x4 mMatrix;
    
public:
    CGMaterialMatrix4Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialSampler2DUniform : public CGMaterialUniform
{
private:
    std::string mTextureRef;
    CRResourceNode *mTexture;
public:
    CGMaterialSampler2DUniform();
    virtual ~CGMaterialSampler2DUniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CRMaterialResource : public CRResource
{
protected:
    std::list<CGMaterialUniform*> mMaterialUniformList;
    CRResourceNode *mShader;
    std::string mShaderRef;
    
public:
    CRMaterialResource();
    virtual ~CRMaterialResource();
    
    void loadResource();
    void releaseResource();
    
    void applyMaterial(CMMatrix4x4 &modelMatrix, CRArea *area, CCSafeArray<CMMatrix4x4> bones);
    
    void readFromStream(CCFileStream *stream);
};

#endif
