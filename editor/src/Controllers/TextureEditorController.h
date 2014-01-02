//
//  TextureEditor.h
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "TextureResource.h"

@interface CTextureEditorController : NSWindowController
{
    IBOutlet NSImageView *mImageDisplay;
    
    CTextureResource *mCurrentTexture;
}

- (IBAction)importImagePressed:(id)sender;

- (void)setTextureResource:(CTextureResource*)resource;

@end
