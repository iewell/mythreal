/*
 *
 * FILE: 3DLayoutEditorController.m
 *
 * AUTHOR: Ian Ewell
 *
 * PROJECT: Mythreal Engine Resource Editor
 *
 * MODULE: Controllers
 *
 * PURPOSE: Controls the 3D Level Layout editor
 *
 * CREATED ON: 11/02/10
 *
 * COPYRIGHT: (C) Copyright 2010 Mythreal Studios. All rights reserved.
 *
 */

#import "3DLayoutEditorController.h"

@implementation C3DLayoutSceneOutlineController

- (void)setSceneRoot:(CHeirarchyNode *)root
{
    if (mSceneRoot)
        [mSceneRoot release];
    mSceneRoot = [root retain];
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item
{
    if (!item)
		return 1;
	
	return [item numberOfChildren];
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
    if ([item numberOfChildren] > 0)
		return TRUE;
	return FALSE;
}

- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item
{
    if (!item)
		return mSceneRoot;
	
	return [item getChildAtIndex:index];
}

- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
	if (!item)
		return [[[NSString alloc] initWithUTF8String:"Scene Root"] autorelease];
	return [item name];
	
}

@end

@implementation CLayoutEditorTool

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview
{
    if (self = [super init])
    {
        mView = [view retain];
        mSceneRoot = [root retain];
        mSceneOutline = outview;
        
        // Register for notifications
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ViewLeftMouseDown:) name:@"LELeftMouseDown" object:mView];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ViewLeftMouseDragged:) name:@"LELeftMouseDragged" object:mView];
    }
    
    return self;
}

- (void)dealloc
{
    if (mView)
        [mView release];
    if (mSceneRoot)
        [mSceneRoot release];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super dealloc];
}

- (void)ViewLeftMouseDown:(NSNotification*)notification
{
    
}

- (void)ViewLeftMouseDragged:(NSNotification*)notification
{
    
}

@end

@implementation C2DCollisionLineTool

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview
{
    if (self = [super initWithView:view sceneRoot:root outline:outview])
    {
        mCurrentPoint = 0;
        mLineTypePopup = nil;
    }
    
    return self;
}

- (void)dealloc
{
    if (mCurrentLine)
        [mCurrentLine release];
    if (mLineTypePopup)
        [mLineTypePopup release];
    [super dealloc];
}

- (void)ViewLeftMouseDown:(NSNotification*)notification
{
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    
    mCurrentPoint++;
    
    if (mCurrentPoint == 3)
    {
        // We need a new line with the first point the same as the current line's last point
        [mCurrentLine release];
        mCurrentLine = [[C2DPhysicsLine alloc] init];
        [mCurrentLine setName:@"CollisionLine"];
        [mCurrentLine setLineType:[mLineTypePopup indexOfSelectedItem]];
        [mCurrentLine setLineWithPointA:mLinePoints[0] PointB:worldPoint];
        mLinePoints[0] = mLinePoints[1];
        [[mSceneRoot findChildWithName:@"2DCollisionData" isRecursive:FALSE] insertChild:mCurrentLine];
        mCurrentPoint = 2;
    }
    
    switch (mCurrentPoint) 
    {
        case 1:
            mLinePoints[0] = worldPoint;
            
            // Create a new line node
            mCurrentLine = [[C2DPhysicsLine alloc] init];
            [mCurrentLine setName:@"CollisionLine"];
            [mCurrentLine setLineType:[mLineTypePopup indexOfSelectedItem]];
            [mCurrentLine setLineWithPointA:worldPoint PointB:worldPoint];
            [[mSceneRoot findChildWithName:@"2DCollisionData" isRecursive:FALSE] insertChild:mCurrentLine];
            
            break;
        case 2:
            mLinePoints[1] = worldPoint;
            [mCurrentLine setLineWithPointA:mLinePoints[0] PointB:mLinePoints[1]];
            break;
        default:
            break;
    }
    
    // Create a mutable data structure with the points
    NSMutableData *data = [[NSMutableData alloc] initWithBytes:mLinePoints length:sizeof(NSPoint)*mCurrentPoint];
    [mView set2DLineArray:data];
    [mView setNeedsDisplay:TRUE];
    NSLog(@"%f, %f", worldPoint.x, worldPoint.y);
}

- (void)setLinePopup:(NSPopUpButton*)button
{
    if (mLineTypePopup)
        [mLineTypePopup release];
    mLineTypePopup = [button retain];
}

- (void)ViewLeftMouseDragged:(NSNotification*)notification
{
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    
    // Snapping Time
    CHeirarchyNode *collisionLines = [mSceneRoot findChildWithName:@"2DCollisionData" isRecursive:FALSE];
    NSPoint mousePosScreen = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:worldPoint];
    for (C2DPhysicsLine *line in [collisionLines getChildren])
    {
        // Don't snap to ourself
        if (mCurrentLine == line)
            continue;
        
        NSPoint p1ScreenSpace = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:[line getPoints][0]];
        
        if (mousePosScreen.x >= p1ScreenSpace.x - 10 && mousePosScreen.x <= p1ScreenSpace.x + 10 &&
            mousePosScreen.y >= p1ScreenSpace.y - 10 && mousePosScreen.y <= p1ScreenSpace.y + 10)
        {
            worldPoint = [line getPoints][0];
            break;
        }
        
        NSPoint p2ScreenSpace = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:[line getPoints][1]];
        
        if (mousePosScreen.x >= p2ScreenSpace.x - 10 && mousePosScreen.x <= p2ScreenSpace.x + 10 &&
            mousePosScreen.y >= p2ScreenSpace.y - 10 && mousePosScreen.y <= p2ScreenSpace.y + 10)
        {
            worldPoint = [line getPoints][1];
            break;
        }
    }
    
    switch (mCurrentPoint) 
    {
        case 1:
            mLinePoints[0] = worldPoint;
            [mCurrentLine setLineWithPointA:worldPoint PointB:worldPoint];
            break;
        case 2:
            // Attempt to snap the second point to the first
            if (ABS([mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:worldPoint].x - [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:mLinePoints[0]].x) <= 5.0f)
                worldPoint.x = mLinePoints[0].x;
             if (ABS([mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:worldPoint].y - [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:mLinePoints[0]].y) <= 5.0f)
                worldPoint.y = mLinePoints[0].y;

            mLinePoints[1] = worldPoint;
            [mCurrentLine setLineWithPointA:mLinePoints[0] PointB:mLinePoints[1]];
        default:
            break;
    }
    
    // Create a mutable data structure with the points
    NSMutableData *data = [[NSMutableData alloc] initWithBytes:mLinePoints length:sizeof(NSPoint)*mCurrentPoint];
    [mView set2DLineArray:data];
    [mView setNeedsDisplay:TRUE];
    
    [mSceneOutline reloadItem:NULL];
}

@end

@implementation C2DSelectCollisionPointTool

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview
{
    if (self = [super initWithView:view sceneRoot:root outline:outview])
    {
        mSelectedPoint = 0;
        mSelectedLine = nil;
    }
    
    return self;
}

- (void)dealloc
{
    if (mSelectedLine)
    {
        // Deselect anything selected
        [mSelectedLine selectPointOne:FALSE];
        [mSelectedLine selectPointTwo:FALSE];
        [mSelectedLine release];
    }
    [super dealloc];
}

- (void)ViewLeftMouseDown:(NSNotification*)notification
{    
    // Deselect what we have
    if (mSelectedLine)
    {
        [mSelectedLine selectPointOne:FALSE];
        [mSelectedLine selectPointTwo:FALSE];
        [mSelectedLine release];
        mSelectedLine = nil;
    }
    
    if (mSelection)
    {
        switch (mSelectionType)
        {
            case SELECTION_AREA:
                [(C2DArea*)mSelection setSelected:FALSE];
                break;
            default:
                break;
        }
        
        [mSelection release];
        mSelection = nil;
        mSelectionType = SELECTION_NONE;
    }
    
    CHeirarchyNode *collisionLines = [mSceneRoot findChildWithName:@"2DCollisionData" isRecursive:FALSE];
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    NSPoint mousePosScreen = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:worldPoint];
    
    mClickLocation = mouseLoc;
    
    for (C2DPhysicsLine *line in [collisionLines getChildren])
    {
        NSPoint p1ScreenSpace = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:[line getPoints][0]];
        
        if (mousePosScreen.x >= p1ScreenSpace.x - 5 && mousePosScreen.x <= p1ScreenSpace.x + 5 &&
            mousePosScreen.y >= p1ScreenSpace.y - 5 && mousePosScreen.y <= p1ScreenSpace.y + 5)
        {
            mSelectedPoint = 0;
            mSelectedLine = [line retain];
            [mSelectedLine selectPointOne:TRUE];
            break;
        }
        
        NSPoint p2ScreenSpace = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:[line getPoints][1]];
        
        if (mousePosScreen.x >= p2ScreenSpace.x - 5 && mousePosScreen.x <= p2ScreenSpace.x + 5 &&
            mousePosScreen.y >= p2ScreenSpace.y - 5 && mousePosScreen.y <= p2ScreenSpace.y + 5)
        {
            mSelectedPoint = 1;
            mSelectedLine = [line retain];
            [mSelectedLine selectPointTwo:TRUE];
            break;
        }
    }
    
    // If we haven't selected a line, see if we have anything else
    if (!mSelectedLine)
    {
        // Search for areas
        CHeirarchyNode *areas = [mSceneRoot findChildWithName:@"2DAreaData" isRecursive:FALSE];
        if (areas)
        {
            for (C2DArea *area in [areas getChildren])
            {
                // Go ahead and do the bounds check. If any lights collide, we will override later
                NSRect bounds = [area getBounds];
                if (NSPointInRect(worldPoint, bounds))
                {
                    mSelection = area;
                    mSelectionType = SELECTION_AREA;
                }
                
                // Now we check the light children in the area
                for (CSceneLight *light in [area getChildren])
                {
                    NSPoint lightPosWorld = NSMakePoint([light getPositionX], [light getPositionY]);
                    NSPoint lightPosScreen = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:lightPosWorld];
                    
                    // Is the mouse at the light?
                    if (mousePosScreen.x >= lightPosScreen.x - 10 && mousePosScreen.x <= lightPosScreen.x + 10 &&
                        mousePosScreen.y >= lightPosScreen.y - 10 && mousePosScreen.y <= lightPosScreen.y + 10)
                    {
                        mSelection = light;
                        mSelectionType = SELECTION_LIGHT;
                    }
                }
            }
        }
        
        // If we have a selection, retain it
        if (mSelection)
        {
            [mSelection retain];
            switch (mSelectionType)
            {
                case SELECTION_AREA:
                    [(C2DArea*)mSelection setSelected:TRUE];
                    mInitialAreaBounds = [(C2DArea*)mSelection getBounds];
                    break;
                case SELECTION_LIGHT:
                    break;
                default:
                    break;
            }
        }
    }
    
    [mView setNeedsDisplay:TRUE];
}

- (void)ViewLeftMouseDragged:(NSNotification*)notification
{
    if (!mSelectedLine && !mSelection)
        return;
    
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    NSPoint mousePosScreen = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:worldPoint];
    
    NSPoint clickPointWorld = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mClickLocation];
    NSPoint offset = NSMakePoint(worldPoint.x-clickPointWorld.x, worldPoint.y-clickPointWorld.y);
    
    if (mSelectedLine)
    {
        // Snapping Time
        CHeirarchyNode *collisionLines = [mSceneRoot findChildWithName:@"2DCollisionData" isRecursive:FALSE];
        for (C2DPhysicsLine *line in [collisionLines getChildren])
        {
            // Don't snap to ourself
            if (mSelectedLine == line)
                continue;
            
            NSPoint p1ScreenSpace = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:[line getPoints][0]];
        
            if (mousePosScreen.x >= p1ScreenSpace.x - 10 && mousePosScreen.x <= p1ScreenSpace.x + 10 &&
            mousePosScreen.y >= p1ScreenSpace.y - 10 && mousePosScreen.y <= p1ScreenSpace.y + 10)
            {
                worldPoint = [line getPoints][0];
                break;
            }
        
            NSPoint p2ScreenSpace = [mView getScreenSpaceCoordinateFromWorldSpaceCoordinate:[line getPoints][1]];
        
            if (mousePosScreen.x >= p2ScreenSpace.x - 10 && mousePosScreen.x <= p2ScreenSpace.x + 10 &&
            mousePosScreen.y >= p2ScreenSpace.y - 10 && mousePosScreen.y <= p2ScreenSpace.y + 10)
            {
                worldPoint = [line getPoints][1];
                break;
            }
        }

        [mSelectedLine getPoints][mSelectedPoint] = worldPoint;
    }
    else if (mSelection)
    {
        switch (mSelectionType)
        {
            case SELECTION_AREA:
                // Restore the area to its initial position
                [(C2DArea*)mSelection setBounds:mInitialAreaBounds];
                // Calculate a new rect
                NSRect newBounds = mInitialAreaBounds;
                newBounds.origin = NSMakePoint(mInitialAreaBounds.origin.x+offset.x, mInitialAreaBounds.origin.y+offset.y);
                [(C2DArea*)mSelection setBounds:newBounds];
                break;
            case SELECTION_LIGHT:
                // Update the light's position
                [(CSceneLight*)mSelection setPositionX:worldPoint.x Y:worldPoint.y];
                break;
            default:
                break;
        }
    }
    
    [mView setNeedsDisplay:TRUE];
}

@end

@implementation C2DCreateAreaTool

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview
{
    if (self = [super initWithView:view sceneRoot:root outline:outview])
    {
        mCurrentArea = nil;
    }
    return self;
}

- (void)dealloc
{
    if (mCurrentArea)
        [mCurrentArea release];
    
    [super dealloc];
}

- (void)ViewLeftMouseDown:(NSNotification*)notification
{
    if (mCurrentArea)
        [mCurrentArea release];
    mCurrentArea = [[C2DArea alloc] init];
    [mCurrentArea setName:@"newArea"];
    
    // Find the area container
    CHeirarchyNode *areaContainer = [mSceneRoot findChildWithName:@"2DAreaData" isRecursive:FALSE];
    
    // If we can't find it, create it
    if (!areaContainer)
    {
        areaContainer = [[CHeirarchyNode alloc] init];
        [areaContainer setName:@"2DAreaData"];
        [mSceneRoot insertChild:areaContainer];
    }
    
    // Add our new area
    [areaContainer insertChild:mCurrentArea];
    
    // Get the world space coordinate
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    mSourcePoint = worldPoint;
    
    // Build a rect from the coordinate for the bounds of the area
    NSRect bounds = NSMakeRect(worldPoint.x, worldPoint.y, 0, 0);
    [mCurrentArea setBounds:bounds];
    
    // Refresh some stuff
    [mView setNeedsDisplay:TRUE];
    [mSceneOutline reloadItem:NULL];
}

- (void)ViewLeftMouseDragged:(NSNotification*)notification
{
    // Get our point
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    
    // Since an NSRect originates in the lower left, we will need to find what quadrant the point
    // is in to find the correct origin point
    
    NSRect bounds;
    // First quadrant
    if (worldPoint.x >= mSourcePoint.x && worldPoint.y >= mSourcePoint.y)
    {
        // The source point is the origin of the rect
        bounds = NSMakeRect(mSourcePoint.x, mSourcePoint.y, worldPoint.x-mSourcePoint.x, worldPoint.y-mSourcePoint.y);
    }
    // Second quadrant
    else if (worldPoint.x <= mSourcePoint.x && worldPoint.y >= mSourcePoint.y)
    {
        // The origin of the rect is the relative x of the current point with the y of the source point
        bounds = NSMakeRect(worldPoint.x, mSourcePoint.y, mSourcePoint.x-worldPoint.x, worldPoint.y-mSourcePoint.y);
    }
    // Third quadrant
    else if (worldPoint.x <= mSourcePoint.x && worldPoint.y <= mSourcePoint.y)
    {
        // The origin of the rect is the current point
        bounds = NSMakeRect(worldPoint.x, worldPoint.y, mSourcePoint.x-worldPoint.x, mSourcePoint.y-worldPoint.y);
    }
    // Fourth quadrant
    else if (worldPoint.x >= mSourcePoint.x && worldPoint.y <= mSourcePoint.y)
    {
        // The origin of the rect is the x of the source point and the relative y of the current
        bounds = NSMakeRect(mSourcePoint.x, worldPoint.y, worldPoint.x-mSourcePoint.x, mSourcePoint.y-worldPoint.y);
    }
    
    [mCurrentArea setBounds:bounds];
    
    // Refresh some stuff
    [mView setNeedsDisplay:TRUE];
    [mSceneOutline reloadItem:NULL];
}

@end

@implementation CCreateLightTool

- (id)initWithView:(C3DSceneEditorView*)view sceneRoot:(CHeirarchyNode*)root outline:(NSOutlineView*)outview
{
    if (self = [super initWithView:view sceneRoot:root outline:outview])
    {
        mCurrentLight = nil;
    }
    return self;
}

- (void)ViewLeftMouseDown:(NSNotification*)notification
{
    // First, get what is selected and see if it is a legit area
    NSInteger selection = [mSceneOutline selectedRow];
    if (selection == -1)
        return;
    
	CHeirarchyNode *node = [mSceneOutline itemAtRow:selection];
    
    // Is this an area?
    if (![[node getNodeType] compare:@"2DArea"] == NSOrderedSame)
        return;
    
    // Great. Do we have room for another light? (max is three)
    if ([node numberOfChildren] >= 3)
        return;
    
    // We are good to go. Create a new light
    CSceneLight *light = [[CSceneLight alloc] init];
    [light setName:@"SceneLight"];
    
    // Put it in
    [node insertChild:light];
    
    // Set its position
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    [light setPositionX:worldPoint.x Y:worldPoint.y];
    
    // Keep it and release it
    if (mCurrentLight)
        [mCurrentLight release];
    mCurrentLight = [light retain];
    [light release];
    
    // Refresh some stuff
    [mView setNeedsDisplay:TRUE];
    [mSceneOutline reloadItem:NULL];
}

- (void)ViewLeftMouseDragged:(NSNotification*)notification
{
    // Set its position
    NSEvent *event = [[notification userInfo] valueForKey:@"Event"];
    NSPoint mouseLoc = [event locationInWindow];
    NSPoint worldPoint = [mView getWorldSpaceCoordinateFromScreenSpaceCoordinate:mouseLoc];
    [mCurrentLight setPositionX:worldPoint.x Y:worldPoint.y];
    
    // Refresh some stuff
    [mView setNeedsDisplay:TRUE];
}

@end

@implementation C3DLayoutEditorController

- (id)initWithWindow:(NSWindow *)window 
{
    if ((self = [super initWithWindow:window])) 
    {
        [mRibbonView setContentView:mTransformRibbonView];
        //mSceneRoot = [[CHeirarchyNode alloc] init];
        //[mView setScene:mSceneRoot];
        
        // Add some container nodes for our scene root node
        /*CHeirarchyNode *collisionContainer = [[CHeirarchyNode alloc] init];
        [collisionContainer setName:@"2DCollisionData"];
        [mSceneRoot insertChild:collisionContainer];
        [collisionContainer release];*/
        
        // Register for notifications
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ViewLeftMouseDown:) name:@"LELeftMouseDown" object:mView];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ViewLeftMouseDragged:) name:@"LELeftMouseDragged" object:mView];
    }
    
    return self;
}

- (void)dealloc {
    // Clean-up code here.
    
    [super dealloc];
}

- (void)windowDidLoad {
    [super windowDidLoad];
    
    [mRibbonView setContentView:mTransformRibbonView];
}

- (void)initWindow
{
    [mRibbonView setContentView:mTransformRibbonView];
    [mListController refresh];
    [mList reloadItem:NULL];
    //[mView setScene:mSceneRoot];
}

- (void)setLayoutResource:(C3DSceneLayoutResource*)resource
{
    if (mSceneResource)
        [mSceneResource release];
    
    mSceneResource = [resource retain];
    
    if (mSceneRoot)
        [mSceneRoot release];
    mSceneRoot = [[resource getRoot] retain];
    [mView setScene:mSceneRoot];
    [mSceneOutlineController setSceneRoot:mSceneRoot];
    [mSceneOutline reloadItem:NULL];
    
    if (mCurrentToolController)
    {
        [mCurrentToolController release];
        mCurrentToolController = nil;
    }
}

- (IBAction)switchRibbon:(id)sender
{
    NSButtonCell *selCell = [sender selectedCell];
    //NSLog(@"%d", [selCell tag]);
    switch ([selCell tag])
    {
        case 0:
            [mRibbonView setContentView:mTransformRibbonView];
            break;
            
        case 1:
            [mRibbonView setContentView:mCreateRibbonView];
            break;
        case 5:
            [mRibbonView setContentView:m2DLayoutRibbonView];
            [mView set2DMode:TRUE];
            [mView setNeedsDisplay:TRUE];
            break;
    }
}

- (IBAction)importObject:(id)sender
{
    NSInteger selection = [mList selectedRow];
    if (selection == -1)
        return;
    
    CResourceNode *resource = [mList itemAtRow:selection];
    
    if (![[resource getNodeType] compare:@"Resource"] == NSOrderedSame)
        return;
    
    C3DSceneObjectInstance *instance = [[C3DSceneObjectInstance alloc] init];
    [instance setName:[resource name]];
    [instance set3DModel:resource];
    [mSceneRoot insertChild:instance];
    [instance release];
    [mView setNeedsDisplay:TRUE];
    [mSceneOutline reloadItem:NULL];
}

- (IBAction)toolInvoked:(id)sender
{
    if (mCurrentToolController)
    {
        [mCurrentToolController release];
        mCurrentToolController = nil;
    }
    switch ([(NSButton*)sender tag])
    {
        case TOOL_2DCOLLISION_LINE:
            mCurrentTool = TOOL_2DCOLLISION_LINE;
            mCurrentToolController = [[C2DCollisionLineTool alloc] initWithView:mView sceneRoot:mSceneRoot outline:mSceneOutline];
            [(C2DCollisionLineTool*)mCurrentToolController setLinePopup:mLineTypeButton];
            break;
        case TOOL_SELECT_LINE:
            mCurrentTool = TOOL_SELECT_LINE;
            mCurrentToolController = [[C2DSelectCollisionPointTool alloc] initWithView:mView sceneRoot:mSceneRoot outline:mSceneOutline];
            break;
        case TOOL_CREATE_AREA:
            mCurrentTool = TOOL_CREATE_AREA;
            mCurrentToolController = [[C2DCreateAreaTool alloc] initWithView:mView sceneRoot:mSceneRoot outline:mSceneOutline];
            break;
        case TOOL_CREATE_LIGHT:
            mCurrentTool = TOOL_CREATE_LIGHT;
            mCurrentToolController = [[CCreateLightTool alloc] initWithView:mView sceneRoot:mSceneRoot outline:mSceneOutline];
            break;
        default:
            break;
    }
}

- (IBAction)exportCollision:(id)sender
{
    NSSavePanel *savePanel = [NSSavePanel savePanel];
    [savePanel setAllowedFileTypes:[[[NSArray alloc] initWithObjects:@"mcol", nil] autorelease]];
    [savePanel setTitle:@"Export Collision Data"];
    NSUInteger result = [savePanel runModal];
    
    if (result == NSFileHandlingPanelCancelButton)
        return;
    
    NSString *path = [[savePanel URL] path];
    
    // Now we will assemble the data
    NSMutableData *saveData = [NSMutableData data];
    [saveData appendBytes:"MCOL" length:4];
    
    // Get the node that stores all the collision
    CHeirarchyNode *collisionNode = [mSceneRoot findChildWithName:@"2DCollisionData" isRecursive:FALSE];
    if (!collisionNode)
    {
        [path release];
        [saveData release];
        return;
    }
    unsigned int numberOfChildren = [collisionNode numberOfChildren];
    [saveData appendBytes:&numberOfChildren length:4];
    
    // Write out the actual data
    for (C2DPhysicsLine *line in [collisionNode getChildren])
    {
        [saveData appendBytes:[line getPoints] length:sizeof(NSPoint)*2];
    }
    
    // Save it to disk
    [saveData writeToFile:path atomically:TRUE];
}

- (IBAction)sceneOutlineSelected:(id)sender
{
    // Find the node that was selected
    NSInteger selection = [mSceneOutline selectedRow];
    if (selection == -1)
        return;
    
	CHeirarchyNode *node = [mSceneOutline itemAtRow:selection];
    
    [mObjectPropertyTable setDataSource:node];
    [mObjectPropertyTable reloadData];
}

- (void)ViewLeftMouseDown:(NSNotification*)notification
{
    
}

- (void)ViewLeftMouseDragged:(NSNotification*)notification
{
    
}
 
@end
