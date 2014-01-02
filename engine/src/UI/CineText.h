/*
 *
 * FILE: CineText.h
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

#ifndef UI_CINETEXT_H
#define UI_CINETEXT_H

#include <String>
#include <CoreText/CoreText.h>
#include "Types.h"

class CUICineText
{
protected:
    
    bool mVisible;
    bool mNeedsRefresh;
    U32 mCurrentCharacter;
    F32 mUpdateTimer;
    std::string mCurrentText;
    
public:
    
    CUICineText();
    void render(CGContextRef context);
    void setText(std::string str);
    void tick(float dt);
    bool getNeedsRefresh();
    bool getFinishedScrolling();
};

#endif