//
//  3DSceneLayoutResource.m
//  Resource Edit
//
//  Created by Ian Ewell on 7/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "3DSceneLayoutResource.h"

@implementation C3DSceneLayoutResource

- (id)init
{
    self = [super init];
    if (self) 
    {
        mSceneRoot = [[CHeirarchyNode alloc] init];
        [mSceneRoot setName:@"SceneRoot"];
        
        [mType release];
        mType = @"3DSceneLayout";
        
        // Some containers
        CHeirarchyNode *collisionContainer = [[CHeirarchyNode alloc] init];
        [collisionContainer setName:@"2DCollisionData"];
        [mSceneRoot insertChild:collisionContainer];
        [collisionContainer release];
        
        CHeirarchyNode *areaContainer = [[CHeirarchyNode alloc] init];
        [areaContainer setName:@"2DAreaData"];
        [mSceneRoot insertChild:areaContainer];
        [areaContainer release];
    }
    
    return self;
}

- (CHeirarchyNode*)getRoot
{
    return mSceneRoot;
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeString:mType];
    [mSceneRoot writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    // Kill the old scene root and create a new one
    if (mSceneRoot)
        [mSceneRoot release];
    mSceneRoot = [[CHeirarchyNode alloc] init];
    [mSceneRoot setName:@"SceneRoot"];
    
    NSString *name = [stream readString];
    [stream readString];
    [mSceneRoot setName:name];
    [mSceneRoot readFromStream:stream];
}

@end
