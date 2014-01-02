//
//  3DObject.h
//  Resource Edit
//
//  Created by Ian Ewell on 3/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "3DModelResource.h"

// Class that defines a 3D object that includes a base model, collision data, and event data
@interface C3DObject : NSObject
{
    CModelRoot *mModelRoot;
}

@end
