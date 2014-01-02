/*
 *
 * FILE: FileStream.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Core
 *
 * PURSPOSE: Creates a file stream for reading and writing files locally.
 *
 * CREATED ON: 06/27/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef CORE_FILE_STREAM_H
#define CORE_FILE_STREAM_H

//Includes:
#include <iostream>
#include <string>
#include <stdio.h>
#include "Stream.h"

class CCFileStream : public CCStream
{
private:
	std::string mFileName;
	int mPosition;
	FILE *mFile;
	
public:
	CCFileStream();
	~CCFileStream();
	
	// File Stream Functions:
	bool openFile(const std::string &file, const char *options);
	void closeFile();
	
	// Read funtions:
	virtual float readFloat();
    virtual double readDouble();
	virtual int   readInt();
	virtual short readShort();
	virtual char  readChar();
    virtual unsigned int   readUInt();
	virtual unsigned short readUShort();
	virtual unsigned char  readUChar();
    virtual std::string readString();
	virtual bool  read(int length, void *buffer);
	
	// Write functions:
	virtual void  writeFloat(float value);
    virtual void  writeDouble(double value);
	virtual void  writeInt(int value);
	virtual void  writeShort(short value);
	virtual void  writeChar(char value);
    virtual void  writeUInt(unsigned int value);
	virtual void  writeUShort(unsigned short value);
	virtual void  writeUChar(unsigned char value);
	virtual void  write(int length, void *buffer);
	
	//Stream funtions:
	virtual void  rewind();
	virtual void  seekTo(long pos);
	virtual long  getPos();
	virtual long  getSize();
};

#endif