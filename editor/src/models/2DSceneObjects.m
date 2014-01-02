//
//  2DSceneObjects.m
//  Resource Edit
//
//  Created by Ian Ewell on 6/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "2DSceneObjects.h"

@implementation C2DPhysicsLine

- (id)init
{
    mNodeType = @"2DPhysicsLine";
    self = [super init];
    if (self) 
    {
        mOnePoint = FALSE;
    }
    
    return self;
}

- (void)setOnePoint:(BOOL)onePoint
{
    mOnePoint = onePoint;
}

- (void)setLineWithPointA:(NSPoint)a PointB:(NSPoint)b
{
    mLine[0] = a;
    mLine[1] = b;
}

- (NSPointArray)getPoints
{
    return mLine;
}

- (void)renderLine
{
    glDisable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
    glLineWidth(2.0f);
    glPointSize(10.0f);
    
    if (!mOnePoint)
    {
        glBegin(GL_LINES);
        if (mHighlighted)
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        else
        {
            if (mLineType == LT_COLLISION)
                glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
            else if (mLineType == LT_KILL)
                glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        }
        glVertex3f(mLine[0].x, mLine[0].y, -0.1f);
        glVertex3f(mLine[1].x, mLine[1].y, -0.1f);
        glEnd();
    }
    
    glBegin(GL_POINTS);
    if (mPointOneSelected)
    {
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        glVertex3f(mLine[0].x, mLine[0].y, 0.1f);
    }
    else
    {
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex3f(mLine[0].x, mLine[0].y, 0.0f);
    }
    if (!mOnePoint)
    {
        if (mPointTwoSelected)
        {
            glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
            glVertex3f(mLine[1].x, mLine[1].y, 0.1f);
        }
        else
        {
            glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
            glVertex3f(mLine[1].x, mLine[1].y, 0.0f);
        }
    }
    glEnd();
}

- (void)selectPointOne:(BOOL)select
{
    mPointOneSelected = select;
}

- (void)selectPointTwo:(BOOL)select
{
    mPointTwoSelected = select;
}

- (void)setHighlighted:(BOOL)highlighted
{
    mHighlighted = highlighted;
}

- (void)setLineType:(ELineType)type
{
    mLineType = type;
}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    // Write the line
    [stream writeData:&mLine[0] withLength:sizeof(NSPoint)*2];
    
    //Write the line type
    [stream writeUInt:mLineType];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
}

- (void)readFromStream:(CFileInputStream*)stream
{
    // Read the line
    [stream readDataIntoBuffer:&mLine[0] withLenght:sizeof(NSPoint)*2];
    
    // Read the type
    mLineType = [stream readUInt];
    
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

@end

@implementation C2DArea

- (id)init
{
    mNodeType = @"2DArea";
    self = [super init];
        
    return self;
}

- (NSRect)getBounds
{
    return mBounds;
}

- (void)setBounds:(NSRect)rect
{
    mBounds = rect;
}

- (void)setSelected:(BOOL)selected
{
    mSelected = selected;
}

- (BOOL)getSelected
{
    return mSelected;
}

- (void)render
{
    glLineWidth(1.0f);
    glPointSize(4.0f);
    
    glDisable(GL_LIGHTING);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Draw the rect
    glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(NSMinX(mBounds), NSMinY(mBounds), 2.0f);
        glVertex3f(NSMaxX(mBounds), NSMinY(mBounds), 2.0f);
        glVertex3f(NSMinX(mBounds), NSMaxY(mBounds), 2.0f);
        glVertex3f(NSMaxX(mBounds), NSMaxY(mBounds), 2.0f);
    glEnd();
    
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(NSMinX(mBounds), NSMinY(mBounds), 2.0f);
    glVertex3f(NSMaxX(mBounds), NSMinY(mBounds), 2.0f);
    glVertex3f(NSMaxX(mBounds), NSMaxY(mBounds), 2.0f);
    glVertex3f(NSMinX(mBounds), NSMaxY(mBounds), 2.0f);
    glEnd();
    
    if (mSelected)
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
        glVertex3f(NSMinX(mBounds), NSMinY(mBounds), 2.0f);
        glVertex3f(NSMaxX(mBounds), NSMinY(mBounds), 2.0f);
        glVertex3f(NSMaxX(mBounds), NSMaxY(mBounds), 2.0f);
        glVertex3f(NSMinX(mBounds), NSMaxY(mBounds), 2.0f);
    
        glVertex3f(NSMidX(mBounds), NSMinY(mBounds), 2.0f);
        glVertex3f(NSMidX(mBounds), NSMaxY(mBounds), 2.0f);
        glVertex3f(NSMaxX(mBounds), NSMidY(mBounds), 2.0f);
        glVertex3f(NSMinX(mBounds), NSMidY(mBounds), 2.0f);
        glEnd();
    }
}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    // Write the min and the max
    [stream writeFloat:NSMinX(mBounds)];
    [stream writeFloat:NSMinY(mBounds)];
    [stream writeFloat:NSMaxX(mBounds)];
    [stream writeFloat:NSMaxY(mBounds)];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];

}

- (void)readFromStream:(CFileInputStream*)stream
{
    // Min Max
    float minX = [stream readFloat];
    float minY = [stream readFloat];
    float maxX = [stream readFloat];
    float maxY = [stream readFloat];
    
    // Set the bounds
    mBounds = NSMakeRect(minX, minY, maxX-minX, maxY-minY);
    
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        // All children are (should be) lights
        child = [[CSceneLight alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

@end

@implementation CSceneLight

- (id)init
{
    mNodeType = @"SceneLight";
    self = [super init];
    
    mPositionX = 0.0f;
    mPositionY = 0.0f;
    mIntensity = 1.0f;
    mColRed = 255;
    mColBlue = 255;
    mColGreen = 255;
    
    return self;
}

- (void)setPositionX:(float) x Y:(float)y
{
    mPositionX = x;
    mPositionY = y;
}

- (float)getPositionX
{
    return mPositionX;
}

- (float)getPositionY
{
    return mPositionY;
}

- (void)render
{
    // Not the most clever marking for a light but I really don't care
    glPointSize(20.0f);
    glColor3f(1.0f, 1.0f, 0.7f);
    glBegin(GL_POINTS);
    glVertex3f(mPositionX, mPositionY, 0.0f);
    glEnd();
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    return 6;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    if ([(NSString*)[aTableColumn identifier] compare:@"Property"] == NSOrderedSame)
    {
        switch (rowIndex)
        {
            case 0:
                return @"PositionX";
                break;
            case 1:
                return @"PositionY";
                break;
            case 2:
                return @"Intensity";
                break;
            case 3:
                return @"ColorR";
                break;
            case 4:
                return @"ColorG";
                break;
            case 5:
                return @"ColorB";
                break;
            default:
                return @"";
                break;
        };
    }
    else if ([(NSString*)[aTableColumn identifier] compare:@"Value"] == NSOrderedSame)
    {
        switch (rowIndex)
        {
            case 0:
                return [NSString stringWithFormat:@"%f", mPositionX];
                break;
            case 1:
                return [NSString stringWithFormat:@"%f", mPositionY];
                break;
            case 2:
                return [NSString stringWithFormat:@"%f", mIntensity];
                break;
            case 3:
                return [NSString stringWithFormat:@"%u", mColRed];
                break;
            case 4:
                return [NSString stringWithFormat:@"%u", mColGreen];
                break;
            case 5:
                return [NSString stringWithFormat:@"%u", mColBlue];
                break;
            default:
                return @"";
                break;
        };
    }
    return nil;
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    
    if ([(NSString*)[aTableColumn identifier] compare:@"Value"] == NSOrderedSame)
    {
        switch (rowIndex)
        {
            case 0:
                mPositionX = [anObject floatValue];
                break;
            case 1:
                mPositionY = [anObject floatValue];
                break;
            case 2:
                mIntensity = [anObject floatValue];
                break;
            case 3:
                mColRed = [anObject intValue];
                break;
            case 4:
                mColGreen = [anObject intValue];
                break;
            case 5:
                mColBlue = [anObject intValue];
                break;
            default:
                break;
        };

    }
    
}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    // Write the position
    [stream writeFloat:mPositionX];
    [stream writeFloat:mPositionY];
    
    // Intensity
    [stream writeFloat:mIntensity];
    
    // Color
    [stream writeUByte:mColRed];
    [stream writeUByte:mColGreen];
    [stream writeUByte:mColBlue];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
    
}

- (void)readFromStream:(CFileInputStream*)stream
{
    // Read the position
    mPositionX = [stream readFloat];
    mPositionY = [stream readFloat];
    
    // Intensity
    mIntensity = [stream readFloat];
    
    // Color
    mColRed = [stream readUByte];
    mColGreen = [stream readUByte];
    mColBlue = [stream readUByte];
    
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}

@end

@implementation CScenePath

- (id)init
{
    mNodeType = @"ScenePath";
    self = [super init];
    
    return self;
}

- (void)render
{
    glDisable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
    // Set up line characteristics
    glLineWidth(2.0f);
    glPointSize(5.0f);
    
    // Color everything white
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Begin rendering the lines
    glBegin(GL_LINE_STRIP);
    
    // Basically loop through all our children and add a new point for each child
    for (CScenePathNode *node in mChildren)
    {
        glVertex3f([node getPositionX], [node getPositionY], 0.0f);
    }
    
    glEnd();
    
    // Repeat the above, but draw points instead of lines
    glBegin(GL_POINTS);
    for (CScenePathNode *node in mChildren)
    {
        glVertex3f([node getPositionX], [node getPositionY], 0.0f);
    }
    glEnd();
}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
       
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
    
}

- (void)readFromStream:(CFileInputStream*)stream
{
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}


@end

@implementation CScenePathNode

- (id)init
{
    mNodeType = @"SceneLight";
    self = [super init];
    
    mPositionX = 0.0f;
    mPositionY = 0.0f;
    mTime = 0.0f;
    mDelay = 0.0f;
    
    return self;
}

- (void)setPositionX:(float)x
{
    mPositionX = x;
}

- (void)setPositionY:(float)y
{
    mPositionY = y;
}

- (void)setTime:(float)time
{
    mTime = time;
}

- (void)setDelay:(float)delay
{
    mDelay = delay;
}

- (float)getPositionX
{
    return mPositionX;
}

- (float)getPositionY
{
    return mPositionY;
}

- (float)getTime
{
    return mTime;
}

- (float)getDelay
{
    return mDelay;
}

- (void)writeToStream:(CFileStream*)stream
{
    // Write our name and type
    [stream writeString:name];
    [stream writeString:mNodeType];
    
    // Write the position
    [stream writeFloat:mPositionX];
    [stream writeFloat:mPositionY];
    
    // Write other stuffs
    [stream writeFloat:mTime];
    [stream writeFloat:mDelay];
    
    // Write the number of children and the children
    [stream writeUInt:[mChildren count]];
    
    for (CHeirarchyNode *node in mChildren)
        [node writeToStream:stream];
    
}

- (void)readFromStream:(CFileInputStream*)stream
{
    // Read the position
    mPositionX = [stream readFloat];
    mPositionY = [stream readFloat];
    
    // Read other stuffs
    mTime = [stream readFloat];
    mDelay = [stream readFloat];
    
    // Get the number of children
    unsigned int childrenCount = [stream readUInt];
    int i;
    for (i = 0; i < childrenCount; i++)
    {
        NSString *nodeName = [stream readString];
        NSString *nodeType = [stream readString];
        CHeirarchyNode *child;
        
        child = [[CHeirarchyNode alloc] init];
        
        [child setName:nodeName];
        [self insertChild:child];
        [child readFromStream:stream];
    }
}


@end