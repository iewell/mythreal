//
//  SceneManager.h
//  mythreal
//
//  Created by Ian Ewell on 6/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef mythreal_SceneManager_h
#define mythreal_SceneManager_h

#include <list>
#include "SceneObject.h"
#include "PhysicsWorld2D.h"
#include "Matrix.h"
#include "LevelLayoutResource.h"

// Real simple scene manager right now, but the code will be way more organized than it was
class CSSceneManager
{
private:
    // Real simple implementation: have all the objects in one giant list for now. 
    // We will have to come up with a method for frustum culling later
    std::list<CSSceneObject*> mObjects;
    
    std::list<CRArea*> mAreas;
    
    // The 2D physics world simulator
    CSPhysicsWorld2D *mPhysicsWorld;
    
    // The current view transformation
    CMMatrix4x4 mViewMatrix;
    
    // The current projection matrix
    CMMatrix4x4 mProjectionMatrix;
    
public:
    CSSceneManager();
    ~CSSceneManager();
    
    // This will initialize the scene manager. This must be callled before anything else.
    void initSceneManager();
    
    // Load a scene from a level Layout resource
    void loadSceneFromLevelLayout(const std::string &name);
    
    // Add an object into the scene manager.
    void insertObject(CSSceneObject *object);
    
    // This is called to update the entire scene every "tick" of the game's clock
    void tick(F32 dt);
    
    // This is called befor the scene renders for the scene to do some render passes
    // in external framebuffers before the render to the screen
    void prePassRenderScene();
    
    // Renders the scene
    void renderScene();
    
    // Set and get the current view matrix
    void setViewMatrix(const CMMatrix4x4 &view);
    CMMatrix4x4 getViewMatrix() const;
    
    // Set and get the current projection matrix
    void setProjectionMatrix(const CMMatrix4x4 &proj);
    CMMatrix4x4 getProjectionMatrix() const;
    
    // Access the physics world
    CSPhysicsWorld2D* getPhysicsWorld() const;
    
    // Access the areas
    void insertArea(CRArea *area);
    std::list<CRArea*> getAreas() const;
};

// The global scene manager
extern CSSceneManager *gGlobalSceneManager;

#endif
