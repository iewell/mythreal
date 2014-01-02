//
//  ES1Renderer.m
//  trainsim
//
//  Created by Ian Ewell on 5/10/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ES1Renderer.h"
#include "Globals.h"
#include "TestPyramidRender.h"
#include "testm3d.h"
#include "testplayer.h"
#include "testCamera.h"
#include "ColorPlane.h"
#include "PhysicsWorld2D.h"
#include "PlayerMarble.h"

CGTestPyramidRender *gPyramidRender = new CGTestPyramidRender;

CSTestM3D gTestm3d;
CSTestM3D gYoshiM3d;
CSTestM3D gRocket;
CSTestPlayer gShip;
CSTestM3D gTerrain;
CSTestM3D gColumn;
CSTestM3D gBowser;
CSTestCamera gCamera;
CSColorPlane gColorPlane;
CSPhysicsWorld2D *gPhysicsWorld;
CSPlayerMarble *gPlayerMarble;

@implementation ES1Renderer


void __gluMakeIdentityf(GLfloat m[16])
{
	m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
	m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
	m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
	m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	GLfloat m[4][4];
	GLfloat sine, cotangent, deltaZ;
	GLfloat radians = fovy / 2 * 3.14 / 180;
	
	deltaZ = zFar - zNear;
	sine = sin(radians);
	if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
	{
		return;
	}
	cotangent = cos(radians) / sine;
	
	__gluMakeIdentityf(&m[0][0]);
	m[0][0] = cotangent / aspect;
	m[1][1] = cotangent;
	m[2][2] = -(zFar + zNear) / deltaZ;
	m[2][3] = -1;
	m[3][2] = -2 * zNear * zFar / deltaZ;
	m[3][3] = 0;
	glMultMatrixf(&m[0][0]);
}

// Create an OpenGL ES 1.1 context
- (id)init
{
    if ((self = [super init]))
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

        if (!context || ![EAGLContext setCurrentContext:context])
        {
            return nil;
        }
        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        glGenFramebuffersOES(1, &defaultFramebuffer);
		glGenRenderbuffersOES(1, &colorRenderbuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, 
									 GL_COLOR_ATTACHMENT0_OES, 
									 GL_RENDERBUFFER_OES, 
									 colorRenderbuffer);
		
		// New part, remember to add GLuint depthRenderbuffer to the class
		glGenRenderbuffersOES(1, &depthRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, 
									 GL_DEPTH_ATTACHMENT_OES, 
									 GL_RENDERBUFFER_OES, 
									 depthRenderbuffer);
		
		 glViewport(0, 0, backingWidth, backingHeight);
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 gluPerspective(60.0f,(GLfloat)backingWidth/backingHeight,2.0f,200.0f);
        
        printf("Width: %d\n", (int)backingWidth);
        printf("Height: %d\n", (int)backingHeight);
        
		 glMatrixMode(GL_MODELVIEW);
		 glLoadIdentity();
		
		GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		GLfloat diffuse[] = { 1.0f, 1.0f, 1.00f, 1.0f };
		GLfloat specular[] = { 1.0f, 1.0f, 1.00f, 1.0f };
		
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);		
        
        CFBundleRef bundle = CFBundleGetMainBundle();
        CFURLRef url = CFBundleCopyBundleURL(bundle);
        CFStringRef urlString = CFURLGetString(url);
        unsigned long length = CFStringGetLength(urlString);
        char *bundleURL = new char[length+1];
        CFStringGetCString(urlString, bundleURL, length+1, kCFStringEncodingASCII);
        // strip off the beginning
        // file://localhost = 16
        std::string bundleLocation = bundleURL + 16;
        delete bundleURL;
        
        //gTestm3d.openM3D(bundleLocation + "sword.m3d");
		//gYoshiM3d.openM3D(bundleLocation + "yoshi.m3d");
		//gRocket.openM3D(bundleLocation + "rocket.m3d");
        
        gPhysicsWorld = new CSPhysicsWorld2D(CMVector2(0, -9.8));
        gPhysicsWorld->loadCollisionData("testlevel01.mcol");
        
        //gPlayerMarble = new CSPlayerMarble(*gPhysicsWorld, 0.2f);
		//gPlayerMarble->openM3D(bundleLocation + "gamesphere.m3d");
        CMVector3 pos(0, 0, 0);
        gPlayerMarble->setPosition(pos);
        //gTerrain.openM3D(bundleLocation + "testterrain.m3d");
        pos.setX(0.0f);
        pos.setY(0.0f);
        gColorPlane.setPosition(pos);
        
        //gColumn.openM3D(bundleLocation + "testlevel01.m3d");
        pos.setX(0.0f);
        pos.setZ(0.0f);
        gColumn.setPosition(pos);
        
        //gBowser.openM3D(bundleLocation + "bowser.m3d");
        pos.setX(-70.0f);
        pos.setZ(-22.0f);
        gBowser.setPosition(pos);
        
    }

    return self;
}

GLfloat position[] = { 0.0f, 80.0f, 50.0f, 1.0f };

- (void)render
{
    // This application only creates a single context which is already set current at this point.
    // This call is redundant, but needed if dealing with multiple contexts.
    [EAGLContext setCurrentContext:context];

    // This application only creates a single default framebuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple framebuffers.
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); 
	glDepthMask(GL_TRUE);
    glViewport(0, 0, backingWidth, backingHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)backingWidth/backingHeight,0.1f,250.0f);
    //printf("Width: %d\n", (int)backingWidth);
    //printf("Height: %d\n", (int)backingHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render everything in landscape view
    
    gCamera.transformScene();
    //gCamera.debugRender();
	
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
	//gPyramidRender->render();
    //gTestm3d.render();
	//gYoshiM3d.render();
	//gRocket.render();
    //gColorPlane.render();
	gPlayerMarble->render();
    gColumn.render();
    //gBowser.render();
    //gTerrain.render();
    
    // This application only creates a single color renderbuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple renderbuffers.
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{	
    // Allocate color buffer backing based on the current layer size
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, 
                                    GL_RENDERBUFFER_WIDTH_OES, 
                                    &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, 
                                    GL_RENDERBUFFER_HEIGHT_OES, 
                                    &backingHeight);
    
    // New part
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES, 
                             GL_DEPTH_COMPONENT16_OES, 
                             backingWidth, backingHeight);
	
    return YES;
}

- (void)dealloc
{
    // Tear down GL
    if (defaultFramebuffer)
    {
        glDeleteFramebuffersOES(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }

    if (colorRenderbuffer)
    {
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
	
	if (depthRenderbuffer) 
	{
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}

    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];

    context = nil;

}

@end
