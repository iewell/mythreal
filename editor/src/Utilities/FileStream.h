//
//  FileStream.h
//  Resource Edit
//
//  Created by Ian Ewell on 7/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CFileStream : NSObject
{
    NSOutputStream *mStream;
}

- (void)openFile:(NSString*)file;

- (void)writeUByte:(unsigned char)byte;
- (void)writeByte:(char) byte;

- (void)writeUShort:(unsigned short)s;
- (void)writeShort:(short)s;

- (void)writeUInt:(unsigned int)i;
- (void)writeInt:(unsigned int)i;

- (void)writeFloat:(float)f;

- (void)writeString:(NSString*)string;

- (void)writeData:(void*)data withLength:(int)length;

- (void)close;

@end

@interface CFileInputStream : NSObject
{
    NSInputStream *mStream;
    unsigned short mVersionMinor;
    unsigned short mVersionMajor;
}

- (void)openFile:(NSString*)file;

- (unsigned char)readUByte;
- (char)readByte;

- (unsigned short)readUShort;
- (short)readShort;

- (unsigned int)readUInt;
- (int)readInt;

- (float)readFloat;

- (NSString*)readString;

- (void*)readDataWithLength:(int)length;
- (void)readDataIntoBuffer:(void*)buffer withLenght:(int)length;

- (void)close;

- (void)setVersion:(unsigned short)minor Major:(unsigned short)major;
- (unsigned short)getVersionMajor;
- (unsigned short)getVersionMinor;

@end