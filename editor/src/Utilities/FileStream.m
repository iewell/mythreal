//
//  FileStream.m
//  Resource Edit
//
//  Created by Ian Ewell on 7/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "FileStream.h"

@implementation CFileStream

- (id)init
{
    self = [super init];
    if (self) 
    {
        // Initialization code here.
    }
    
    return self;
}

- (void)openFile:(NSString*)file
{
    if (mStream)
        [mStream release];
    mStream = [NSOutputStream outputStreamToFileAtPath:file append:FALSE];
    [mStream open];
}

- (void)writeUByte:(unsigned char)byte
{
    [mStream write:(&byte) maxLength:sizeof(char)];
}

- (void)writeByte:(char) byte
{
    [mStream write:((unsigned char*)&byte) maxLength:sizeof(char)];
}

- (void)writeUShort:(unsigned short)s
{
    [mStream write:((unsigned char*)&s) maxLength:sizeof(short)];
}

- (void)writeShort:(short)s
{
    [mStream write:((unsigned char*)&s) maxLength:sizeof(short)];
}

- (void)writeUInt:(unsigned int)i
{
    [mStream write:((unsigned char*)&i) maxLength:sizeof(int)];
}

- (void)writeInt:(unsigned int)i
{
    [mStream write:((unsigned char*)&i) maxLength:sizeof(int)];
}

- (void)writeFloat:(float)f
{
    [mStream write:((unsigned char*)&f) maxLength:sizeof(float)];
}

- (void)writeString:(NSString*)string
{
    unsigned int length = [string length];
    const char *buffer = [string UTF8String];
    [self writeUInt:length];
    [mStream write:(unsigned char*)buffer maxLength:length];
}

- (void)writeData:(void*)data withLength:(int)length
{
    [mStream write:data maxLength:length];
}

- (void)close
{
    [mStream close];
}

@end

@implementation CFileInputStream

- (void)openFile:(NSString*)file
{
    if (mStream)
        [mStream release];
    mStream = [NSInputStream inputStreamWithFileAtPath:file];
    [mStream open];
}

- (unsigned char)readUByte
{
    unsigned char buffer;
    [mStream read:&buffer maxLength:sizeof(char)];
    return buffer;
}

- (char)readByte
{
    unsigned char buffer;
    [mStream read:&buffer maxLength:sizeof(char)];
    return (char)buffer;
}

- (unsigned short)readUShort
{
    unsigned short buffer;
    [mStream read:(unsigned char*)&buffer maxLength:sizeof(short)];
    return buffer;
}

- (short)readShort
{
    short buffer;
    [mStream read:(unsigned char*)&buffer maxLength:sizeof(short)];
    return buffer;
}

- (unsigned int)readUInt
{
    unsigned int buffer;
    [mStream read:(unsigned char*)&buffer maxLength:sizeof(int)];
    return buffer;
}

- (int)readInt
{
    int buffer;
    [mStream read:(unsigned char*)&buffer maxLength:sizeof(int)];
    return buffer;
}

- (float)readFloat
{
    float buffer;
    [mStream read:(unsigned char*)&buffer maxLength:sizeof(float)];
    return buffer;
}

- (NSString*)readString
{
    unsigned int length = [self readUInt];
    unsigned char buffer[length+1];
    [mStream read:buffer maxLength:length];
    buffer[length] = 0;
    return [NSString stringWithUTF8String:(char*)&buffer[0]];
}

- (void*)readDataWithLength:(int)length
{
    unsigned char *buffer = malloc(length);
    [mStream read:buffer maxLength:length];
    return buffer;
}

- (void)readDataIntoBuffer:(void*)buffer withLenght:(int)length
{
    [mStream read:buffer maxLength:length];
}

- (void)close
{
    [mStream close];
}

- (void)setVersion:(unsigned short)minor Major:(unsigned short)major
{
    mVersionMajor = major;
    mVersionMinor = minor;
}

- (unsigned short)getVersionMajor
{
    return mVersionMajor;
}

- (unsigned short)getVersionMinor
{
    return mVersionMinor;
}

@end