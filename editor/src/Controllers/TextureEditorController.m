//
//  TextureEditor.m
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "TextureEditorController.h"

@implementation CTextureEditorController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) 
    {
        mCurrentTexture = nil;
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
}

- (IBAction)importImagePressed:(id)sender
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setAllowsMultipleSelection:NO];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowedFileTypes:[NSImage imageTypes]];
    [openPanel setTitle:@"Select an image"];
    NSUInteger result = [openPanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSImage *image = [[NSImage alloc] initWithContentsOfURL:[openPanel URL]];
    [mImageDisplay setImage:image];
    
    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    CGImageSourceRef myImageSourceRef = CGImageSourceCreateWithURL((CFURLRef)[openPanel URL], NULL);
    CGImageRef myImageRef = CGImageSourceCreateImageAtIndex (myImageSourceRef, 0, NULL);
    size_t width = CGImageGetWidth(myImageRef);
    size_t height = CGImageGetHeight(myImageRef);
    CGRect rect = {{0, 0}, {width, height}};
    void *imageData = malloc( height * width * 4 );
    CGContextRef myBitmapContext = CGBitmapContextCreate (imageData,
                                                          width, height, 8,
                                                          width*4, space,
                                                          kCGBitmapByteOrder32Big |
                                                          kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease( space );
    CGContextClearRect( myBitmapContext, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( myBitmapContext, 0, height - height );
    CGContextSetBlendMode(myBitmapContext, kCGBlendModeCopy);
    CGContextDrawImage(myBitmapContext, rect, myImageRef);
    CGContextRelease(myBitmapContext);
    
    [mCurrentTexture setWidth:width];
    [mCurrentTexture setHeight:height];
    [mCurrentTexture setTextureData:imageData];
    
    [image release];
}

- (void)setTextureResource:(CTextureResource*)resource
{
    mCurrentTexture = resource;
}

@end
