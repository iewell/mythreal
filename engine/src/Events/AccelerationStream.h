/*
 *
 * FILE: AccelerationStream.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Input
 *
 * PURSPOSE: Implement a stream that will return the iphone's accelerometer values.
 *
 * CREATED ON: 10/14/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef INPUT_ACCELERATION_H
#define INPUT_ACCELERATION_H

#ifdef __OBJC__
#include <UIKit/UIKit.h>
#endif

class CIAccelerationStream
{
private:
    CIAccelerationStream();
    
    static CIAccelerationStream *mInstance;
    
    double mX;
    double mY;
    double mZ;
    
public:
    
    static CIAccelerationStream* getSingleton();
    
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    
    double getX();
    double getY();
    double getZ();
};
 
#endif