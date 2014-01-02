/*
 *
 * FILE: testCamera.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Scene
 *
 * PURSPOSE: Implement a camera prototype that tracks the player prototype.
 *
 * CREATED ON: 10/11/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#include <OpenGLES/ES1/gl.h>
#include <math.h>
#include "testCamera.h"
#include "SceneManager.h"
#include "AccelerationStream.h"

CSTestCamera::CSTestCamera()
{
    mTarget = NULL;
    mOrbitSpeed = 0;
    mOrbitAngle = 0;
}

void CSTestCamera::setOffset(CMVector3 offset)
{
    mOffset = offset;
}

void CSTestCamera::setTargetPlayer(CSSceneObject *player)
{
    mTarget = player;
}

void CSTestCamera::setOrbitSpeed(F32 orbitSpeed)
{
    mOrbitSpeed = orbitSpeed;
}

void CSTestCamera::setOrbitAngle(F32 orbitAngle)
{
    mOrbitAngle = orbitAngle;
}

void CSTestCamera::tick(F32 dt)
{
    if (!mTarget)
        return;
    
    mOrbitAngle += mOrbitSpeed*dt;
    mOldPosition = mNewPosition;
    mNewPosition = mTarget->getPosition();
    mNewPosition.add(mOffset);
    CMVector3 gravity(-CIAccelerationStream::getSingleton()->getY(), CIAccelerationStream::getSingleton()->getX(), 0.0f);
    gravity.normalize();
    gravity.multiplyScalar(15.0f);
    mNewPosition.add(gravity);
    CMMatrix4x4 screenTrans;
    screenTrans.rotate(D_TO_R(90), 0, 0, 1);
    mWorld.setIdentity();
    
    /*case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
     viewMatrix.lookAtMatrix(position, CMVector3(position.getX()+1, position.getY(), position.getZ()), CMVector3(0.0f, 1.0f, 0.0f));
     break;
     
     case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
     viewMatrix.lookAtMatrix(position, CMVector3(position.getX()-1, position.getY(), position.getZ()), CMVector3(0.0f, 1.0f, 0.0f));
     break;
     
     case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
     viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY()+1, position.getZ()), CMVector3(0.0f, 0.0f, -1.0f));
     break;
     
     case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
     viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY()-1, position.getZ()), CMVector3(0.0f, 0.0f, 1.0f));
     break;
     
     case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
     viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY(), position.getZ()+1), CMVector3(0.0f, 1.0f, 0.0f));
     break;
     
     case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
     viewMatrix.lookAtMatrix(position, CMVector3(position.getX(), position.getY(), position.getZ()-1), CMVector3(0.0f, 1.0f, 0.0f));
     break;*/

    
    mWorld.lookAtMatrix(mNewPosition, mTarget->getPosition());
    CMVector3 position = mTarget->getPosition();
    //mWorld.lookAtMatrix(position, CMVector3(position.getX(), position.getY()-1, position.getZ()));
    //printf("%f\n", position.getY());
    //CMMatrix4x4 trans;
    // trans.setColumn(3, CMVector4(mNewPosition));
    //mWorld.rotate(D_TO_R(-20.0f), 1.0f, 0.0f, 0.0f);
    //mWorld.setColumn(3, CMVector4(mNewPosition));
    //mWorld.multiply(trans);
    mView = mWorld;
    //mView.inverse();
    //mView.rotate(D_TO_R(-mOrbitAngle), 0.0f, 1.0f, 0.0f);
    screenTrans.multiply(mView);
    mView = screenTrans;
    gGlobalSceneManager->setViewMatrix(mView);
    mNewPosition.add(mOffset);
}

void CSTestCamera::debugRender()
{
    GLfloat vertices[6];
    vertices[0] = point1.getX();
    vertices[1] = point1.getY();
    vertices[2] = point1.getZ();
    vertices[3] = point2.getX();
    vertices[4] = point2.getY();
    vertices[5] = point2.getZ();
    
    glColor4f(1, 1, 1, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void CSTestCamera::transformScene()
{
    //glTranslatef(-mNewPosition.getX(), /*-mNewPosition.getY()*/0, -50);
    //glRotatef(45, 1.0f, 0.0f, 0.0f);
    //mView.multMatrixGL();
    //glRotatef(mOrbitAngle, 0.0f, 1.0f, 0.0f);
}

void CSTestCamera::getRay(U32 x, U32 y, CMVector3 &pos, CMVector3 &dir)
{
    F32 dx, dy;
    CMMatrix4x4 world;
    
    dx = tanf(D_TO_R(60.0f*0.5f))*(((768.0-(F32)x)/(768.0/2.0) - 1.0f))/0.75;
    dy = tanf(D_TO_R(60.0f*0.5f))*(1.0f-(1024.0-(F32)y)/(1024.0/2.0));
    world = mView;
    world.inverse();
    CMVector3 p1(dx*0.1f, dy*0.1f, 0.1f);
    CMVector3 p2(dx*250.0f, dy*250.0f, 250.0f);
    
    CMVector4 p1t = world.multiplyVec(CMVector4(p1));
    CMVector4 p2t = world.multiplyVec(CMVector4(p2));
    point1 = p1t;
    point2 = p2t;
    CMVector4 vec = p2t;
    vec.subtract(p1t);
    
    dir.setX(vec.getX());
    dir.setY(vec.getY());
    dir.setZ(vec.getZ());
    dir.normalize();
    
    pos.setX(p1t.getX());
    pos.setY(p1t.getY());
    pos.setZ(p1t.getZ());
}

void CSTestCamera::render()
{
    
}