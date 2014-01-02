//
//  LevelLayoutResource.h
//  mythreal
//
//  Created by Ian Ewell on 8/3/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_LevelLayoutResource_h
#define mythreal_LevelLayoutResource_h

#include "Resource.h"
#include "Vector.h"

class CRSceneObject : public CCHeirarchyNode
{
    std::string mModelName;
    CMVector3 mPosition;
    
public:
    std::string getModelName()
    {
        return mModelName;
    }
    
    CMVector3 getPosition()
    {
        return mPosition;
    }
    
    void readFromStream(CCFileStream *stream);
};

typedef enum
{
    LT_COLLISION,
    LT_KILL,
} ELineType;

class CR2DPhysicsLine : public CCHeirarchyNode
{
protected:
    CMVector2 mLine[2];
    ELineType mLineType;
    
public:
    CR2DPhysicsLine();
    virtual ~CR2DPhysicsLine();
    
    CMVector2 getLinePoint(U32 point)
    {
        return mLine[point];
    }
    
    ELineType getLineType()
    {
        return mLineType;
    }
    
    void readFromStream(CCFileStream *stream);
};

class CRArea : public CCHeirarchyNode
{
protected:
    CMVector2 mMin;
    CMVector2 mMax;
    
    // Whether or not to render objects in this area
    bool mVisible;
    
public:
    CMVector2 getMin()
    {
        return mMin;
    }
    
    CMVector2 getMax()
    {
        return mMax;
    }
    
    void setMin(CMVector2 min)
    {
        mMin = min;
    }
    
    void setMax(CMVector2 max)
    {
        mMax = max;
    }
    
    void setVisible(bool visible)
    {
        mVisible = visible;
    }
    
    bool getVisible()
    {
        return mVisible;
    }
    
    void readFromStream(CCFileStream *stream);
};

class CRLight : public CCHeirarchyNode
{
protected:
    CMVector2 mPosition;
    F32 mIntensity;
    CMVector3 mColor;
    
public:
    CMVector2 getPosition() const
    {
        return mPosition;
    }
    
    void setPosition(CMVector2 pos)
    {
        mPosition = pos;
    }
    
    F32 getIntensity() const
    {
        return mIntensity;
    }
    
    void setIntensity(F32 intensity)
    {
        mIntensity = intensity;
    }
    
    CMVector3 getColor() const
    {
        return mColor;
    }
    
    void setColor(CMVector3 color)
    {
        mColor = color;
    }
    
    void readFromStream(CCFileStream *stream);
};

class CRLevelLayoutResource : public CRResource
{
protected:
    CCHeirarchyNode *mSceneRoot;
    
public:
    CRLevelLayoutResource();
    virtual ~CRLevelLayoutResource();
    
    CCHeirarchyNode* getSceneRoot()
    {
        return mSceneRoot;
    }
    
    //void loadResource();
    //void releaseResource();
    
    void readFromStream(CCFileStream *stream);

};

#endif
