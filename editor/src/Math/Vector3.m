//
//  Vector3.m
//  Resource Edit
//
//  Created by Ian Ewell on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Vector3.h"
#include <math.h>

@implementation CVector3

- (id)init
{
    if (self = [super init])
    {
        mX = 0.0f;
        mY = 0.0f;
        mZ = 0.0f;
    }
    return self;
}

- (id)initX:(float)x Y:(float)y Z:(float)z
{
    if (self = [super init])
    {
        mX = x;
        mY = y;
        mZ = z;
    }
    return self;
}

- (void)setX:(float)x
{
    mX = x;
}

- (float)getX
{
    return mX;
}

- (void)setY:(float)y
{
    mY = y;
}

- (float)getY
{
    return mY;
}

- (void)setZ:(float)z
{
    mZ = z;
}

- (float)getZ
{
    return mZ;
}

- (void)add:(CVector3*)vec
{
    mX += [vec getX];
    mY += [vec getY];
    mZ += [vec getZ];
}

- (float)dot:(CVector3*)vec
{
    return mX*[vec getX] + mY*[vec getY] + mZ*[vec getZ];
}

- (void)multiplyScalar:(float)value
{
    mX *= value;
    mY *= value;
    mZ *= value;
}

- (void)negate
{
    mX = -mX;
    mY = -mY;
    mZ = -mZ;
}

- (void)normalize
{
    float magnitude = sqrtf(mX*mX + mY*mY + mZ*mZ);
    mX /= magnitude;
    mY /= magnitude;
    mZ /= magnitude;
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeFloat:mX];
    [stream writeFloat:mY];
    [stream writeFloat:mZ];
}

-(id)copyWithZone:(NSZone *)zone
{
    CVector3 *copy = [[CVector3 alloc]initX:mX Y:mY Z:mZ];
    return copy;
}

+ (CVector3*)vectorFromSum:(CVector3*)A B:(CVector3*)B
{
    CVector3* result = [A copy];
    [result add:B];
    return result;
}

+ (CVector3*)vectorFromStream:(CFileInputStream*)stream
{
    CVector3* result = [[CVector3 alloc] init];
    [result setX:[stream readFloat]];
    [result setY:[stream readFloat]];
    [result setZ:[stream readFloat]];
    return result;
}

@end
