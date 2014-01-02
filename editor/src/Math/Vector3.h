//
//  Vector3.h
//  Resource Edit
//
//  Created by Ian Ewell on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FileStream.h"

@interface CVector3 : NSObject <NSCopying>
{
    float mX;
    float mY;
    float mZ;
}

- (id)init;
- (id)initX:(float)x Y:(float)y Z:(float)z;

- (void)setX:(float)x;
- (float)getX;
- (void)setY:(float)y;
- (float)getY;
- (void)setZ:(float)z;
- (float)getZ;

- (void)add:(CVector3*)vec;
- (float)dot:(CVector3*)vec;
- (void)multiplyScalar:(float)value;
- (void)negate;

- (void)normalize;

- (void)writeToStream:(CFileStream*)stream;

-(id)copyWithZone:(NSZone *)zone;

+ (CVector3*)vectorFromSum:(CVector3*)A B:(CVector3*)B;
+ (CVector3*)vectorFromStream:(CFileInputStream*)stream;

@end
