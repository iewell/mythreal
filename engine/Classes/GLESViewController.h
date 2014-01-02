//
//  GLESViewController.h
//  mythreal
//
//  Created by Ian Ewell on 6/4/13.
//
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface GLESViewController : GLKViewController
{
}

@property (strong, nonatomic) EAGLContext *context;

@end
