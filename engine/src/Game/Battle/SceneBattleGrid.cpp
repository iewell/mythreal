//
//  SceneBattleGrid.cpp
//  mythreal
//
//  Created by Ian Ewell on 3/29/13.
//
//

#include "SceneBattleGrid.h"
#include <OpenGLES/ES2/gl.h>

CGBSceneBattleGrid::CGBSceneBattleGrid(CCPointerStrong<CGBBattleGrid> ref)
{
    mGridScale = 1.0f;
    mGrid = ref;
}

CGBSceneBattleGrid::~CGBSceneBattleGrid()
{
    
}

void CGBSceneBattleGrid::setGridScale(F32 scale)
{
    mGridScale = scale;
}

F32 CGBSceneBattleGrid::getScale() const
{
    return mGridScale;
}

void CGBSceneBattleGrid::setPlane(const CMPlane &plane)
{
    mPlane = plane;
}

CMPlane CGBSceneBattleGrid::getPlane() const
{
    return mPlane;
}

void CGBSceneBattleGrid::setPlaneMat(CRMaterialResource *mat)
{
    mPlaneMat = mat;
    mGridMat->loadResource();   
}

void CGBSceneBattleGrid::setGridMat(CRMaterialResource *mat)
{
    mGridMat = mat;
    mGridMat->loadResource();
}

void CGBSceneBattleGrid::setArea(CRArea *area)
{
    mArea = area;
}

void CGBSceneBattleGrid::tick(F32 dt)
{
    
}

void CGBSceneBattleGrid::render()
{
    // Start at (0, 0) and scale
    U32 sizeX = mGrid->getWidth();
    U32 sizeY = mGrid->getHeight();
    
    // Build vertex buffer
    F32 vertices[] = {0.0f, 0.0f, 0.0f,
        sizeX*mGridScale, 0.0f, 0.0f,
        sizeX*mGridScale, 0.0f, sizeY*mGridScale,
        0.0f, 0.0f, sizeY*mGridScale};
    // Normal buffer
    F32 normals[] = {0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};
    // Uvs, 1 tile per grid cell
    F32 uvs[] = {0.0f, 0.0f,
        (F32)sizeX, 0.0f,
        (F32)sizeX, (F32)sizeY,
        0.0f, (F32)sizeY};
    // Indices
    U16 indices[] = {0, 2, 1, 2, 0, 3};
    
    CCSafeArray<CMMatrix4x4> bonemat;
    
    // Render
    glEnable(GL_DEPTH_TEST);
    mPlaneMat->applyMaterial(mObject, mArea, bonemat);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, 0, 0, uvs);
    glEnableVertexAttribArray(ATTRIB_UV);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, 0, 0, normals);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glDrawElements(GL_TRIANGLES, 2*3, GL_UNSIGNED_SHORT, indices);
    glDisableVertexAttribArray(ATTRIB_VERTEX);
    glDisableVertexAttribArray(ATTRIB_UV);
    glDisableVertexAttribArray(ATTRIB_NORMAL);
    
    // Build vertex buffer for grid render
    F32 gridVertices[(sizeX+1+sizeY+1)*3*2];
    
    // Horizontal lines
    for (U32 i = 0; i <= sizeX; i++)
    {
        gridVertices[i*6] = i*mGridScale;
        gridVertices[i*6+1] = 0.01f;
        gridVertices[i*6+2] = 0.0f;
        gridVertices[i*6+3] = i*mGridScale;
        gridVertices[i*6+4] = 0.01f;
        gridVertices[i*6+5] = sizeY*mGridScale;
    }
    
    // Vertical lines
    U32 offset = (sizeX+1)*6;
    for (U32 i = 0; i <= sizeY; i++)
    {
        gridVertices[offset+i*6] = 0.0f;
        gridVertices[offset+i*6+1] = 0.01f;
        gridVertices[offset+i*6+2] = i*mGridScale;
        gridVertices[offset+i*6+3] = sizeX*mGridScale;
        gridVertices[offset+i*6+4] = 0.01f;
        gridVertices[offset+i*6+5] = i*mGridScale;
    }
    
    // Render
    glLineWidth(2.0f);
    mGridMat->applyMaterial(mObject, mArea, bonemat);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, gridVertices);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glDrawArrays(GL_LINES, 0, (sizeX+1+sizeY+1)*2);
    glDisableVertexAttribArray(ATTRIB_VERTEX);
}