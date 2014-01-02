/*
 *
 * FILE: AccelerationStream.mm
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

#include "AccelerationStream.h"

CIAccelerationStream* CIAccelerationStream::mInstance;

@interface accelerationDelegate : NSObject<UIAccelerometerDelegate>
{
    
}

-(void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration;

@end

@implementation accelerationDelegate

-(void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
    CIAccelerationStream::getSingleton()->setX([acceleration x]);
    CIAccelerationStream::getSingleton()->setY([acceleration y]);
    CIAccelerationStream::getSingleton()->setZ([acceleration z]);
}

@end

CIAccelerationStream::CIAccelerationStream()
{
    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:1/32];
    [[UIAccelerometer sharedAccelerometer] setDelegate:[[accelerationDelegate alloc] init]];
    mX = 0;
    mY = 0;
    mZ = 0;
}

CIAccelerationStream* CIAccelerationStream::getSingleton()
{
    if (!mInstance)
        mInstance = new CIAccelerationStream();
    return mInstance;
}

void CIAccelerationStream::setX(double x)
{
    mX = x;
}

void CIAccelerationStream::setY(double y)
{
    mY = y;
    //printf("%f\n", y);
}

void CIAccelerationStream::setZ(double z)
{
    mZ = z;
}

double CIAccelerationStream::getX()
{
    return mX;
}

double CIAccelerationStream::getY()
{
    return mY;
}

double CIAccelerationStream::getZ()
{
    return mZ;
}