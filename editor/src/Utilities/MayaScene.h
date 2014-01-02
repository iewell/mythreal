//
//  MayaScene.h
//  Resource Edit
//
//  Created by Ian Ewell on 5/21/11.
//  Copyright 2013 In Ewell. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "3DModelResource.h"

@interface MayaScene : NSObject {
@private
    
}

- (CModelRoot*)importModel:(NSString*)path;

@end
