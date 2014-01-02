/*
 *
 * FILE: UIRenderView.mm
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine
 *
 * MODULE: UI
 *
 * PURSPOSE: View fo where all UI elements are rendered on
 *
 * CREATED ON: 05/13/11
 *
 * COPYRIGHT: (C) Copyright 2011 Mythreal Studios. All rights reserved.
 *
 */

#import "UIRenderView.h"
#import "CineText.h"

UIView *gUIView;

@implementation UIRenderView

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        gUIView = self;
    }
    return self;
}


@end
