//
//  Plane.m
//  Resource Edit
//
//  Created by Ian Ewell on 3/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Plane.h"

@implementation CPlane

- (id)init
{
    if (self = [super init])
    {
        mD = 0.0f;
    }
    return self;
}

- (id)initWithNormal:(CVector3*)normal point:(CVector3*)point
{
    if (self = [super init])
    {
        mX = [normal getX];
        mY = [normal getY];
        mZ = [normal getZ];
        [self normalize];
        mD = -(mX*[point getX] + mY*[point getY] + mZ*[point getZ]);
    }
    return self;
}

- (void)setNormal:(CVector3*)normal point:(CVector3*)point
{
    mX = [normal getX];
    mY = [normal getY];
    mZ = [normal getZ];
    [self normalize];
    mD = -(mX*[point getX] + mY*[point getY] + mZ*[point getZ]);
}

- (float)distance:(CVector3 *)point
{
    return [self dot:point] + mD;
}

- (void)negate
{
    [super negate];
    mD = -mD;
}

- (bool)intersectRayAtPos:(CVector3 *)pos direction:(CVector3 *)dir intersection:(CVector3 *)inter
{
    float dotr = [self dot:dir];
    if (dotr == 0.0)
        return false;
    if (inter)
        [inter release];
    inter = [pos copy];
    CVector3 *v = [dir copy];
    [v multiplyScalar:[self distance:pos]/dotr];
    [v negate];
    [inter add:v];
    [v autorelease];
    return true;
}

@end
