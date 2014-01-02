//
//  TextureResource.h
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Resource.h"
#import "FileStream.h"

@interface CTextureResource : CResource
{
    // Width and height of the texture
    unsigned int mWidth;
    unsigned int mHeight;
    
    // The raw texture data
    void *mTextureData;
}

- (void)setWidth:(unsigned int)width;
- (unsigned int)getWidth;

- (void)setHeight:(unsigned int)height;
- (unsigned int)getHeight;

- (void)setTextureData:(void*)data;
- (void*)getTextureData;

- (void)writeToStream:(CFileStream*)stream;

@end
