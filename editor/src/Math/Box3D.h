//
//  Box3D.h
//  Resource Edit
//
//  Created by Ian Ewell on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Vector3.h"

@interface CBox3D : NSObject
{
    CVector3 *mMin;
    CVector3 *mMax;
}

- (id)init;
- (id)initWithMin:(CVector3*)min max:(CVector3*)max;

- (void)setMin:(CVector3*)min;
- (CVector3*)getMin;
- (void)setMax:(CVector3*)max;
- (CVector3*)getMax;

@end
