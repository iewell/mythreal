//
//  SceneBattleGrid.h
//  mythreal
//
//  Created by Ian Ewell on 3/29/13.
//
//

#ifndef __mythreal__SceneBattleGrid__
#define __mythreal__SceneBattleGrid__

#include <iostream>
#include "SceneObject.h"
#include "BattleGrid.h"
#include "MaterialResource.h"
#include "Plane.h"

class CGBSceneBattleGrid : public CSSceneObject
{
protected:
    F32 mGridScale;
    CGBBattleGridRef mGrid;
    CMPlane mPlane;
    
    CRMaterialResource *mPlaneMat;
    CRMaterialResource *mGridMat;
    
    CRArea *mArea;
    
public:
    CGBSceneBattleGrid(CGBBattleGridRef ref);
    virtual ~CGBSceneBattleGrid();
    
    void setGridScale(F32 scale);
    F32 getScale() const;
    
    void setPlane(const CMPlane &plane);
    CMPlane getPlane() const;
    
    void setPlaneMat(CRMaterialResource *mat);
    void setGridMat(CRMaterialResource *mat);
    
    // TODO: Get rid of this god-awful area system for a real lighting manager
    void setArea(CRArea *area);
    
    void tick(F32 dt);
    void render();
};

#endif /* defined(__mythreal__SceneBattleGrid__) */
