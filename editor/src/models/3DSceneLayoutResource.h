//
//  3DSceneLayoutResource.h
//  Resource Edit
//
//  Created by Ian Ewell on 7/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Resource.h"

@interface C3DSceneLayoutResource : CResource
{
    CHeirarchyNode *mSceneRoot;
}

- (CHeirarchyNode*)getRoot;

@end
