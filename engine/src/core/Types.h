/*
 *
 * FILE: Types.h
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: Core
 *
 * PURSPOSE: Implement typedefs for convinience and style.
 *
 * CREATED ON: 05/10/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <pthread.h>

//Basic variable typedefs:

//8-bit
typedef unsigned char U8;
typedef signed char   S8;

//16-bit
typedef unsigned short U16;
typedef signed short   S16;

//32-bit
typedef unsigned int U32;
typedef signed int   S32;

//float
typedef float F32;

//double
typedef double F64;

//PI and conversions
#define MPI 3.1415926543
#define D_TO_R(degrees) (MPI*degrees)/180

#endif