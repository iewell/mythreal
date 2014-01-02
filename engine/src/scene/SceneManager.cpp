//
//  SceneManager.cpp
//  mythreal
//
//  Created by Ian Ewell on 6/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "SceneManager.h"
#include "testm3d.h"
#include "LevelLayoutResource.h"
#include "ResourceManager.h"

CSSceneManager *gGlobalSceneManager;

CSSceneManager::CSSceneManager()
{
    
}

void CSSceneManager::initSceneManager()
{
    mPhysicsWorld = new CSPhysicsWorld2D(CMVector2(0.0f, -9.8f));
}

void CSSceneManager::loadSceneFromLevelLayout(const std::string &name)
{
    CRResourceNode *node = CRResourceManager::Get()->findResourceOfType(name, RT_3D_LEVEL_LAYOUT);
    CRLevelLayoutResource *resource = static_cast<CRLevelLayoutResource*>(node->getResource());
    std::vector<CSTestM3D*> m3DList;
    std::vector<CCHeirarchyNode*> sceneList = resource->getSceneRoot()->getChildren();
    std::vector<CCHeirarchyNode*>::iterator sceneListIterator;
    for (sceneListIterator = sceneList.begin(); sceneListIterator < sceneList.end(); sceneListIterator++)
    {
        // Do we need to process collision lines?
        if ((*sceneListIterator)->getName().compare("2DCollisionData") == 0)
        {
            std::vector<CCHeirarchyNode*> colList = (*sceneListIterator)->getChildren();
            std::vector<CCHeirarchyNode*>::iterator colListIterator;
            for (colListIterator = colList.begin(); colListIterator < colList.end(); colListIterator++)
            {
                CR2DPhysicsLine *line = static_cast<CR2DPhysicsLine*>((*colListIterator));
                mPhysicsWorld->addCollisionLine(line->getLinePoint(0), line->getLinePoint(1), line->getLineType());
            }
        }
        else if ((*sceneListIterator)->getName().compare("2DAreaData") == 0)
        {
            std::vector<CCHeirarchyNode*> areaList = (*sceneListIterator)->getChildren();
            std::vector<CCHeirarchyNode*>::iterator areaListIterator;
            for (areaListIterator = areaList.begin(); areaListIterator < areaList.end(); areaListIterator++)
            {
                CRArea *area = static_cast<CRArea*>((*areaListIterator));
                mAreas.push_back(area);
            }
        }
        else
        {
            CRSceneObject *object = static_cast<CRSceneObject*>((*sceneListIterator));
            CSTestM3D *m3d = new CSTestM3D;
            CMVector3 pos = object->getPosition();
            m3d->setPosition(pos);
            m3d->loadFromResource(object->getModelName());
            m3DList.push_back(m3d);
            insertObject(m3d);
        }
    }
    
    std::vector<CSTestM3D*>::iterator m3DIt;
    for (m3DIt = m3DList.begin(); m3DIt < m3DList.end(); m3DIt++)
    {
        (*m3DIt)->getRenderer()->assignAreas(resource->getSceneRoot()->findChildWithName("2DAreaData", false));
    }
}

void CSSceneManager::insertObject(CSSceneObject *object)
{
    mObjects.push_back(object);
}

bool intersectPointRect(CMVector2 point, CMVector2 min, CMVector2 max)
{
    if (point.getX() >= min.getX() && point.getX() <= max.getX() &&
        point.getY() >= min.getY() && point.getY() <= max.getY())
    {
        return true;
    }
    return false;
}

void CSSceneManager::tick(F32 dt)
{
    // Update our physics
    mPhysicsWorld->tick(dt);
    
    // Update all of our objects
    for (std::list<CSSceneObject*>::iterator it = mObjects.begin(); it != mObjects.end(); it++)
    {
        (*it)->tick(dt);
    }
    
    // Update visibility information on all areas
    CMVector4 precamPos = CMVector4();
    precamPos = mViewMatrix.multiplyVec(precamPos);
    CMVector4 camPos = CMVector4(-precamPos.getY(), precamPos.getX(), 0.0f, 1.0f);
    for (std::list<CRArea*>::iterator areaIt = mAreas.begin(); areaIt != mAreas.end(); areaIt++)
    {
        CMVector2 areaMin = (*areaIt)->getMin();
        CMVector2 areaMax = (*areaIt)->getMax();
        if (intersectPointRect(CMVector2(camPos.getX()-50, camPos.getY()-50), areaMin, areaMax) ||
            intersectPointRect(CMVector2(camPos.getX()+50, camPos.getY()-50), areaMin, areaMax) ||
            intersectPointRect(CMVector2(camPos.getX()-50, camPos.getY()+50), areaMin, areaMax) ||
            intersectPointRect(CMVector2(camPos.getX()+50, camPos.getY()+50), areaMin, areaMax))
        {
            (*areaIt)->setVisible(true);
        }
        else
        {
            (*areaIt)->setVisible(false);
        }
    }
}

void CSSceneManager::prePassRenderScene()
{
    for (std::list<CSSceneObject*>::iterator it = mObjects.begin(); it != mObjects.end(); it++)
    {
        (*it)->prePassRender();
    }
}

void CSSceneManager::renderScene()
{
    for (std::list<CSSceneObject*>::iterator it = mObjects.begin(); it != mObjects.end(); it++)
    {
        (*it)->render();
    }
}

void CSSceneManager::setViewMatrix(const CMMatrix4x4 &view)
{
    mViewMatrix = view;
}

CMMatrix4x4 CSSceneManager::getViewMatrix() const
{
    return mViewMatrix;
}

void CSSceneManager::setProjectionMatrix(const CMMatrix4x4 &proj)
{
    mProjectionMatrix = proj;
}

CMMatrix4x4 CSSceneManager::getProjectionMatrix() const
{
    return mProjectionMatrix;
}

CSPhysicsWorld2D* CSSceneManager::getPhysicsWorld() const
{
    return mPhysicsWorld;
}

void CSSceneManager::insertArea(CRArea *area)
{
    mAreas.push_back(area);
}

std::list<CRArea*> CSSceneManager::getAreas() const
{
    return mAreas;
}