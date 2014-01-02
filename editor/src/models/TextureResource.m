//
//  TextureResource.m
//  Resource Edit
//
//  Created by Ian Ewell on 7/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "TextureResource.h"

@implementation CTextureResource

- (id)init
{
    self = [super init];
    if (self) 
    {
        [mType release];
        mType = @"Texture";
    }
    
    return self;
}

- (void)setWidth:(unsigned int)width
{
    mWidth = width;
}

- (unsigned int)getWidth
{
    return mWidth;
}

- (void)setHeight:(unsigned int)height
{
    mHeight = height;
}

- (unsigned int)getHeight
{
    return mHeight;
}

- (void)setTextureData:(void*)data
{
    if (mTextureData)
        free(mTextureData);
    mTextureData = data;
}

- (void*)getTextureData
{
    return mTextureData;
}

- (void)writeToStream:(CFileStream*)stream
{
    [stream writeString:mType];
    [stream writeUInt:mWidth];
    [stream writeUInt:mHeight];
    [stream writeData:mTextureData withLength:mWidth*mHeight*4];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    mWidth = [stream readUInt];
    mHeight = [stream readUInt];
    mTextureData = [stream readDataWithLength:mWidth*mHeight*4];
}

@end
