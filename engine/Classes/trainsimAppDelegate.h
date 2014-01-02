//
//  trainsimAppDelegate.h
//  trainsim
//
//  Created by Ian Ewell on 5/10/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "GLESViewController.h"

@class EAGLView;

@interface trainsimAppDelegate : UIResponder <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) GLESViewController *viewController;

@end

