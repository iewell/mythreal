//
//  2DSceneObjects.h
//  Resource Edit
//
//  Created by Ian Ewell on 6/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGL/gl.h>
#import "HeirarchyNode.h"

typedef enum
{
    LT_COLLISION,
    LT_KILL,
} ELineType;

@interface C2DPhysicsLine : CHeirarchyNode
{
    NSPoint mLine[2];
    BOOL mOnePoint;
    
    BOOL mPointOneSelected;
    BOOL mPointTwoSelected;
    
    BOOL mHighlighted;
    
    ELineType mLineType;
}

- (void)setOnePoint:(BOOL)onePoint;
- (void)setLineWithPointA:(NSPoint)a PointB:(NSPoint)b;
- (NSPointArray)getPoints;
- (void)renderLine;

- (void)selectPointOne:(BOOL)select;
- (void)selectPointTwo:(BOOL)select;

- (void)setHighlighted:(BOOL)highlighted;

- (void)setLineType:(ELineType)type;

@end

@interface C2DArea : CHeirarchyNode 
{
@private
    NSRect mBounds;
    BOOL mSelected;
}

- (NSRect)getBounds;
- (void)setBounds:(NSRect)rect;

- (void)setSelected:(BOOL)selected;
- (BOOL)getSelected;

- (void)render;

- (void)writeToStream:(CFileStream*)stream;
- (void)readFromStream:(CFileInputStream*)stream;

@end

@interface CSceneLight : CHeirarchyNode 
{
@private
    float mPositionX;
    float mPositionY;
    
    float mIntensity;
    
    unsigned char mColRed;
    unsigned char mColBlue;
    unsigned char mColGreen;
}

- (void)setPositionX:(float)x Y:(float)y;

- (float)getPositionX;
- (float)getPositionY;

- (void)render;

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;

@end

@interface CScenePath : CHeirarchyNode 
{
@private
    
}

- (void)render;

@end

@interface CScenePathNode : CHeirarchyNode 
{
@private
    float mPositionX;
    float mPositionY;
    
    // Time to reach here from previous node
    float mTime;
    
    // Delay before sending off to the next node
    float mDelay;
}

- (void)setPositionX:(float)x;
- (void)setPositionY:(float)y;
- (void)setTime:(float)time;
- (void)setDelay:(float)delay;

- (float)getPositionX;
- (float)getPositionY;
- (float)getTime;
- (float)getDelay;

@end