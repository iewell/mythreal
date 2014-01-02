//
//  trainsimAppDelegate.mm
//  trainsim
//
//  Created by Ian Ewell on 5/10/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "trainsimAppDelegate.h"
#import "EAGLView.h"

#include <pthread.h>
#include "Globals.h"
#include "MainLoop.h"
#include "TestPyramid.h"
#include "TestPyramidRender.h"

CCMainLoop *gMainLoop;

@implementation trainsimAppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch.
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.viewController = [[[GLESViewController alloc] initWithNibName:@"MainWindow" bundle:nil] autorelease];
    } else {
        self.viewController = [[[GLESViewController alloc] initWithNibName:@"MainWindow_ipad" bundle:nil] autorelease];
    }
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    gMainLoop = new CCMainLoop();
    gMainLoop->Initialize();
    gMainLoop->populateScene();
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
}


@end
