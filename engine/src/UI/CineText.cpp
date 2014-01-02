/*
 *
 * FILE: CineText.cpp
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: UI
 *
 * PURSPOSE: A simple interface for displaying text in a cinematic fasion.
 *
 * CREATED ON: 05/13/11
 *
 * COPYRIGHT: (C) Copyright 2011 Mythreal Studios. All rights reserved.
 *
 */

#include "CineText.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

CUICineText::CUICineText()
{
    mVisible = false;
    mCurrentCharacter = 0;
    mUpdateTimer = 0;
}

void CUICineText::render(CGContextRef context)
{
    if (mVisible)
    {
        CGContextSetTextMatrix(context, CGAffineTransformIdentity);
        CGAffineTransformMakeRotation(3.141592654/2);
        CGContextTranslateCTM(context, 768, 1024);
        CGContextScaleCTM(context, -1, 1);
        CGContextRotateCTM(context, -M_PI/2);
    
        // Draw the black letterbox
        CGContextBeginPath(context);
        CGFloat color[] = {0, 0, 0, 1};
        CGContextSetStrokeColor(context, color);
        CGRect letterbox = CGRectMake(0, 0, 1024, 100);
        CGContextAddRect(context, letterbox);
        CGContextFillRect(context, letterbox);
        //CGContextClosePath(context);
    
    
        // Initialize a rectangular path.
    
        CGMutablePathRef path = CGPathCreateMutable();
    
        CGRect bounds = CGRectMake(0, 0, 1024.0, 100);
    
        CGPathAddRect(path, NULL, bounds);
    
    
    
        // Initialize an attributed string.
        
        std::string cpy = mCurrentText;
        std::string::iterator it = cpy.begin()+mCurrentCharacter;
        if (mCurrentText.length())
            cpy.erase(mCurrentCharacter);
        
        CFStringRef string = CFStringCreateWithCString(NULL, cpy.c_str(), kCFStringEncodingUTF8);
        
        CFMutableAttributedStringRef attrString =
    
        CFAttributedStringCreateMutable(kCFAllocatorDefault, 0);
    
        CFAttributedStringReplaceString (attrString,
                                     
                                     CFRangeMake(0, 0), string);
    
    
    
        CTFontDescriptorRef des = CTFontDescriptorCreateWithNameAndSize(CFSTR("Ariel"), 30);
        CTFontRef font = CTFontCreateWithFontDescriptor(des, 30, NULL);
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength(string)), kCTFontAttributeName, font);
    
        // Create a color and add it as an attribute to the string.
    
        CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
    
        CGFloat components[] = { 1.0, 1.0, 1.0, 1.0 };
        
        CGColorRef red = CGColorCreate(rgbColorSpace, components);
    
        CGColorSpaceRelease(rgbColorSpace);
    
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength(string)),
                                   
                                   kCTForegroundColorAttributeName, red);
    
    
    
        // Create the framesetter with the attributed string.
    
        CTFramesetterRef framesetter =
    
        CTFramesetterCreateWithAttributedString(attrString);
    
        CFRelease(attrString);
    
    
    
        // Create the frame and draw it into the graphics context
    
        CTFrameRef frame = CTFramesetterCreateFrame(framesetter,
                                                
                                                CFRangeMake(0, 0), path, NULL);
    
        CFRelease(framesetter);
    
        CTFrameDraw(frame, context);
    
        CFRelease(frame);
        
        mNeedsRefresh = false;
    }
}

void CUICineText::setText(std::string str)
{
    mCurrentText = str;
    mCurrentCharacter = 0;
}

void CUICineText::tick(float dt)
{
    mUpdateTimer += dt;
    if (mUpdateTimer >= 0.1)
    {
        mCurrentCharacter+=2;
        if (mCurrentCharacter > mCurrentText.length())
            mCurrentCharacter = mCurrentText.length();
        else
            mNeedsRefresh = true;
        mUpdateTimer -= 0.1;
    }
}

bool CUICineText::getNeedsRefresh()
{
    return mNeedsRefresh;
}

bool CUICineText::getFinishedScrolling()
{
    if (mCurrentCharacter == mCurrentText.length())
        return true;
    return false;
}