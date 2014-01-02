//
//  Plane.h
//  Resource Edit
//
//  Created by Ian Ewell on 3/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Vector3.h"

@interface CPlane : CVector3
{
    float mD;
}

- (id)init;
- (id)initWithNormal:(CVector3*)normal point:(CVector3*)point;

- (void)setNormal:(CVector3*)normal point:(CVector3*)point;
- (float)distance:(CVector3*)point;

- (void)negate;

- (bool)intersectRayAtPos:(CVector3*)pos direction:(CVector3*)dir intersection:(CVector3*)inter;

@end
