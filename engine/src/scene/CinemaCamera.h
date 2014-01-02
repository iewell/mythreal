//
//  CinemaCamera.h
//  mythreal
//
//  Created by Ian Ewell on 3/30/13.
//
//

#ifndef __mythreal__CinemaCamera__
#define __mythreal__CinemaCamera__

#include <iostream>
#include "SceneObject.h"

class CSCinemaCamera : public CSSceneObject
{
protected:
    bool mActive;
    CMVector3 mStartPos;
    CMVector3 mStartAim;
    CMVector3 mCurrentPos;
    CMVector3 mCurrentAim;
    CMVector3 mTargetPos;
    CMVector3 mTargetAim;
    F32 mDuration;
    F32 mTimer;
    
    bool mStarted;
    
public:
    CSCinemaCamera();
    ~CSCinemaCamera();
    
    void setStart(CMVector3 pos, CMVector3 aim);
    void setEnd(CMVector3 pos, CMVector3 aim);
    
    CMVector3 getCurrentPosition();
    
    void setDuration(F32 duration);
    void start();
    void setActive(bool active) {mActive = active;}
    void tick(F32 dt);
    void getRay(U32 x, U32 y, CMVector3 &pos, CMVector3 &dir);
    void render();
};

#endif /* defined(__mythreal__CinemaCamera__) */
