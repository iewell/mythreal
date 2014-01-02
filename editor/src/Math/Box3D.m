//
//  Box3D.m
//  Resource Edit
//
//  Created by Ian Ewell on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Box3D.h"

@implementation CBox3D

- (id)init
{
    if (self = [super init])
    {
        mMin = [[CVector3 alloc] initX:0.0f Y:0.0f Z:0.0f];
        mMax = [[CVector3 alloc] initX:0.0f Y:0.0f Z:0.0f];
    }
    return self;
}

- (id)initWithMin:(CVector3*)min max:(CVector3*)max
{
    if (self = [super init])
    {
        mMin = [min copy];
        mMax = [max copy];
    }
    return self;
}

- (void)dealloc
{
    if (mMin)
        [mMin release];
    if (mMax)
        [mMax release];
    
    [super dealloc];
}

- (void)setMin:(CVector3*)min
{
    if (mMin)
        [mMin release];
    mMin = [min copy];
}

- (CVector3*)getMin
{
    return mMin;
}

- (void)setMax:(CVector3*)max
{
    if (mMax)
        [mMax release];
    mMax = [max copy];
}

- (CVector3*)getMax
{
    return mMax;
}


@end
