//
//  GLESViewController.m
//  mythreal
//
//  Created by Ian Ewell on 6/4/13.
//
//

#import "GLESViewController.h"
#include "MainLoop.h"
#include "SceneManager.h"
#include "Matrix.h"
extern CCMainLoop *gMainLoop;

@implementation GLESViewController

float gScreenWidth;
float gScreenHeight;

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [_context release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    self.preferredFramesPerSecond = 30;
    
    [EAGLContext setCurrentContext:self.context];
    
    glEnable(GL_CULL_FACE);
    
    // Create our scene manager
    gGlobalSceneManager = new CSSceneManager();
    gGlobalSceneManager->initSceneManager();
    
    float aspect = fabsf(self.view.bounds.size.width / self.view.bounds.size.height);
    CMMatrix4x4 proj;
    proj.makePerspectiveMatrix(60.0f, aspect, 0.2f, 500.0f);
    gGlobalSceneManager->setProjectionMatrix(proj);
    
    // Set our global screen width and height
    gScreenWidth = self.view.bounds.size.width;
    gScreenHeight = self.view.bounds.size.height;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    
    // Dispose of any resources that can be recreated.
}

- (void)update
{
    gMainLoop->Frame();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
    glDepthMask(GL_TRUE);
    glClearColor(1.0f, 0.23f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gGlobalSceneManager->prePassRenderScene();
    
    glViewport(0.0f, 0.0f, gScreenWidth, gScreenHeight);
    glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
	glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gGlobalSceneManager->renderScene();

}

@end
