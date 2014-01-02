//
//  Material.h
//  mythreal
//
//  Created by Ian Ewell on 7/1/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

/*#ifndef mythreal_Material_h
#define mythreal_Material_h
#include <OpenGLES/ES2/gl.h>
#include "Matrix.h"
#include "FileStream.h"
#include <list>

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
} EFloatValueType;

typedef enum
{
    VTV2_CUSTOM = 0,
} EVector2ValueType;

typedef enum
{
    VTV3_CUSTOM = 0,
    VTV3_EYE_POS,
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
} EMatrix4ValueType;

// attribute index
enum {
    ATTRIB_VERTEX,
    ATTRIB_UV,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

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
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc = NULL) = 0;
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg) = 0;
};

class CGMaterialFloatUniform : public CGMaterialUniform
{
private:
    F32 mFloatValue;
    
public:
    CGMaterialFloatUniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialVector2Uniform : public CGMaterialUniform
{
private:
    CMVector2 mVector;
    
public:
    CGMaterialVector2Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialVector3Uniform : public CGMaterialUniform
{
private:
    CMVector3 mVector;
    
public:
    CGMaterialVector3Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialVector4Uniform : public CGMaterialUniform
{
private:
    CMVector4 mVector;
    
public:
    CGMaterialVector4Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialMatrix4Uniform : public CGMaterialUniform
{
private:
    CMMatrix4x4 mMatrix;
    
public:
    CGMaterialMatrix4Uniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

class CGMaterialSampler2DUniform : public CGMaterialUniform
{
private:
    void *mTextureData;
    U32 mTextureWidth;
    U32 mTextureHeight;
    GLuint mTextureID;
    
public:
    CGMaterialSampler2DUniform();
    
    virtual void applyUniform(const CMMatrix4x4 &modelMatrix, int *textureInc = NULL);
    virtual void loadUniformFromStream(CCFileStream *stream, GLint shaderProg);
};

typedef struct
{
    GLuint uniformID;
    GLuint textureID;
    eTextureType textureType;
} sTextureUniform;

class CGMaterial
{
private:
    GLuint mShaderProgram;
    GLint mModelMatrixUniformLoc;
    GLint mModelViewMatrixUniformLoc;
    GLint mProjectionMatrixUniformLoc;
    GLint mEyePosUniformLoc;
    
    std::list<sTextureUniform> mTextureUniformList;
    
    std::list<CGMaterialUniform*> mMaterialUniformList;
    
public:
    CGMaterial();
    ~CGMaterial();
    
    bool loadShader(const std::string &vShader, const std::string &fShader);
    
    void addTextureUniform(const char *uniformName, eTextureType textureType, GLuint textureID);
    
    void loadMaterial(CCFileStream *stream);
    
    void applyMaterial(CMMatrix4x4 &modelMatrix);
};

#endif*/
