//
//  BattleTestGame.cpp
//  mythreal
//
//  Created by Ian Ewell on 3/29/13.
//
//

#include "BattleTestGame.h"
#include <sstream>
#import <UIKit/UIKit.h>

extern UIView *gUIView;

@interface _BattleUIProxy : NSObject
{
    _BattleUIController *forward;
}

- (id)initWithController:(_BattleUIController*)controller;
- (void)moveButton:(id)sender;
- (void)attackButton:(id)sender;

@end

@interface _BattleAttackTableSource : NSObject <UITableViewDataSource, UITableViewDelegate>
{
    UITableViewCell *physical;
    UITableViewCell *whtMagic;
    UITableViewCell *blkMagic;
    UITableViewCell *cancelButton;
    UITableViewCell *backButton;
    _BattleUIController *controller;
    
    CGBSceneBattleActor *sourceActor;
    
    NSMutableArray *attacks;
    
    enum
    {
        LEVEL_ROOT = -1,
        LEVEL_PHYS = 0,
        LEVEL_BLKM = 1,
        LEVEL_WHTM = 2,
        LEVEL_BLANK = 3,
        LEVEL_ATTK_READY = 4,
    } currentLevel;
}

- (id)initWithController:(_BattleUIController*)ctl;

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView;
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath;
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section;

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath;

- (void)cellSelected:(NSUInteger)cellNum;

- (void)setActive:(BOOL)active;

- (void)setSourceActor:(CGBSceneBattleActor*)actor;

@end

class _BattleUIController
{
protected:
    UIView *mBlankDisplay;
    UIView *mPlayerDisplay;
    
    CGBSceneBattleActor *mActivePlayer;
    CGBBattleTester *mBattleTester;
    
    // Objective-C proxy to receive and forward events
    _BattleUIProxy *mProxy;
    
    // Data sources and controllers
    _BattleAttackTableSource *mAttackTableSource;
    
    // Controls for various items on player screen
    UILabel *mNameLablel;
    UILabel *mStatsLabel;
    
    UIButton *mMoveButton;
    UIButton *mAttackButton;
    
    UITableView *mAttackTable;
    
public:
    _BattleUIController(CGBBattleTester *tester)
    {
        mProxy = [[_BattleUIProxy alloc] initWithController:this];
        
        mAttackTableSource = [[_BattleAttackTableSource alloc] initWithController:this];
        
        mBattleTester = tester;
        
        CGRect bounds = CGRectMake(0, 568, 1024, 200);
        mBlankDisplay = [[UIView alloc] initWithFrame:bounds];
        mBlankDisplay.backgroundColor = [UIColor whiteColor];
        
        mPlayerDisplay = [[UIView alloc] initWithFrame:bounds];
        mPlayerDisplay.backgroundColor = [UIColor whiteColor];
        
        [gUIView insertSubview:mBlankDisplay atIndex:0];
        
        // Label to tell to pick a character on the grid
        bounds = CGRectMake(0, 0, 1024, 200);
        UILabel *blankLabel = [[UILabel alloc] initWithFrame:bounds];
        [blankLabel setText:@"Please make a selection"];
        [blankLabel setTextAlignment:NSTextAlignmentCenter];
        [mBlankDisplay addSubview:blankLabel];
        
        // Player name label
        bounds = CGRectMake(0, 0, 200, 17);
        mNameLablel = [[UILabel alloc] initWithFrame:bounds];
        [mNameLablel setText:@"Player:"];
        [mPlayerDisplay addSubview:mNameLablel];
        
        // Player stats label
        bounds = CGRectMake(0, 20, 200, 102);
        mStatsLabel = [[UILabel alloc] initWithFrame:bounds];
        [mStatsLabel setText:@""];
        [mStatsLabel setNumberOfLines:6];
        [mPlayerDisplay addSubview:mStatsLabel];
        
        // Move button
        bounds = CGRectMake(200, 10, 150, 85);
        mMoveButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        mMoveButton.frame = bounds;
        [mMoveButton addTarget:mProxy action:@selector(moveButton:) forControlEvents:UIControlEventTouchDown];
        [mMoveButton setTitle:@"Move" forState:UIControlStateNormal];
        [mPlayerDisplay addSubview:mMoveButton];
        
        // Attack button
        bounds = CGRectMake(200, 100, 150, 85);
        mAttackButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        mAttackButton.frame = bounds;
        [mAttackButton addTarget:mProxy action:@selector(attackButton:) forControlEvents:UIControlEventTouchDown];
        [mAttackButton setTitle:@"Attack" forState:UIControlStateNormal];
        [mPlayerDisplay addSubview:mAttackButton];
        
        // Attack list
        bounds = CGRectMake(375, 10, 200, 180);
        mAttackTable = [[UITableView alloc] initWithFrame:bounds style:UITableViewStylePlain];
        mAttackTable.bounces = NO;
        mAttackTable.dataSource = mAttackTableSource;
        mAttackTable.delegate = mAttackTableSource;
        [mPlayerDisplay addSubview:mAttackTable];
        
        mActivePlayer = nullptr;
    }
    
    void updateStatsDisplay()
    {
        if (!mActivePlayer)
            return;
        
        [mNameLablel setText:[NSString stringWithUTF8String:(std::string("Player: ") + mActivePlayer->getName()).c_str()]];
        [mBlankDisplay removeFromSuperview];
        [gUIView addSubview:mPlayerDisplay];
        
        std::ostringstream stream;
        stream << "HP: " << mActivePlayer->getCurrentHitPoints() << "/" << mActivePlayer->getMaxHitPoints() << "\n"
        << "MP: " << mActivePlayer->getCurrentMovementPoints() << "/" << mActivePlayer->getMaxMovementPoints() << "\n";
        [mStatsLabel setText:[NSString stringWithUTF8String:stream.str().c_str()]];
        
        [mAttackTableSource setActive:FALSE];
        [mAttackTable reloadData];
    }
    
    void setActivePlayer(CGBSceneBattleActor *player)
    {
        if (player == mActivePlayer)
            return;
        
        mActivePlayer = player;
        if (mActivePlayer)
        {
            updateStatsDisplay();
            [mAttackTableSource setActive:FALSE];
            [mAttackTableSource setSourceActor:player];
            [mAttackTable reloadData];
        }
        else
        {
            [mPlayerDisplay removeFromSuperview];
            [gUIView addSubview:mBlankDisplay];
        }
    }
    
    void moveButtonPressed()
    {
        mBattleTester->setTouchMode(TM_MOVE);
    }
    
    void attackButtonPressed()
    {
        [mAttackTableSource setActive:TRUE];
        [mAttackTable reloadData];
    }
    
    void setAttackMode(CGBBattleAttackDefinition* defn)
    {
        mBattleTester->setAttackMode(defn);
    }
};

@implementation _BattleUIProxy

- (id)initWithController:(_BattleUIController *)controller
{
    self = [super init];
    forward = controller;
    return self;
}

- (void)moveButton:(id)sender
{
    forward->moveButtonPressed();
}

- (void)attackButton:(id)sender
{
    forward->attackButtonPressed();
}

@end

@interface _BattleAttackTableButton : NSObject
{
    UITableViewCell *cell;
    CGBBattleAttackDefinition *_definition;
}

- (id)initWithCell:(UITableViewCell*)_cell atkdefn:(CGBBattleAttackDefinition*)defn;
- (UITableViewCell*)cell;
- (CGBBattleAttackDefinition*)_definition;

@end

@implementation _BattleAttackTableButton

- (id)initWithCell:(UITableViewCell *)_cell atkdefn:(CGBBattleAttackDefinition *)defn
{
    if (self = [super init])
    {
        cell = [_cell retain];
        _definition = defn;
    }
    return self;
}

- (UITableViewCell*)cell
{
    return cell;
}

- (CGBBattleAttackDefinition*)_definition
{
    return _definition;
}

@end

@implementation _BattleAttackTableSource

- (id)initWithController:(_BattleUIController *)ctl
{
    self = [super init];
    controller = ctl;
    
    currentLevel = LEVEL_BLANK;
    
    physical = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    physical.textLabel.text = @"Physical";
    
    blkMagic = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    blkMagic.textLabel.text = @"Black Magic";
    
    whtMagic = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    whtMagic.textLabel.text = @"White Magic";
    
    cancelButton = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    cancelButton.textLabel.text = @"Cancel";
    
    backButton = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    backButton.textLabel.text = @"Back";
    
    sourceActor = nullptr;
    
    attacks = [[NSMutableArray alloc] init];
    
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (currentLevel == LEVEL_ROOT)
    {
        if ([indexPath indexAtPosition:1] == 0)
            return cancelButton;
        else if ([indexPath indexAtPosition:1] == 1)
            return physical;
        else if ([indexPath indexAtPosition:1] == 2)
            return blkMagic;
        else if ([indexPath indexAtPosition:1] == 3)
            return whtMagic;
    }
    else if (currentLevel == LEVEL_ATTK_READY)
        return cancelButton;
    else
    {
        if ([indexPath indexAtPosition:1] == 0)
            return backButton;
        else
            return [[attacks objectAtIndex:[indexPath indexAtPosition:1]-1] cell];
    }
    return nil;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (currentLevel == LEVEL_ROOT)
        return 4;
    if (currentLevel == LEVEL_BLANK)
        return 0;
    if (currentLevel == LEVEL_ATTK_READY)
        return 1;
    else
    {
        if (!sourceActor)
            return 1;
        
        std::list<CGBBattleAttackDefinition*>* attacksL = sourceActor->getAvailableAttacks();
        U32 cnt = 1;
        [attacks removeAllObjects];
        for (auto it = attacksL->begin(); it != attacksL->end(); it++)
        {
            if ((*it)->getCategory() == currentLevel)
            {
                UITableViewCell *cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil] autorelease];
                cell.textLabel.text = [NSString stringWithUTF8String:(*it)->getName().c_str()];
                _BattleAttackTableButton *button = [[_BattleAttackTableButton alloc] initWithCell:cell atkdefn:(*it)];
                [attacks addObject:button];
                cnt++;
            }
        }
        return cnt;
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [self cellSelected:[indexPath indexAtPosition:1]];
    [tableView reloadData];
}

- (void)cellSelected:(NSUInteger)cellNum
{
    if (currentLevel == LEVEL_ROOT)
    {
        if (cellNum == 0)
            currentLevel = LEVEL_BLANK;
        if (cellNum == 1)
            currentLevel = LEVEL_PHYS;
        if (cellNum == 2)
            currentLevel = LEVEL_BLKM;
        if (cellNum == 3)
            currentLevel = LEVEL_WHTM;
    }
    else if (cellNum == 0)
        currentLevel = LEVEL_ROOT;
    else if (currentLevel == LEVEL_PHYS || currentLevel == LEVEL_BLKM || currentLevel == LEVEL_WHTM)
    {
        currentLevel = LEVEL_ATTK_READY;
        controller->setAttackMode([[attacks objectAtIndex:cellNum-1] _definition]);
    }
}

- (void)setActive:(BOOL)active
{
    if (active && currentLevel != LEVEL_BLANK)
        return;
    if (active)
        currentLevel = LEVEL_ROOT;
    else
        currentLevel = LEVEL_BLANK;
}

- (void)setSourceActor:(CGBSceneBattleActor*)actor
{
    sourceActor = actor;
}

@end

CGBBattleTester::CGBBattleTester(CSSceneManager *manager)
{
    mSceneManager = manager;
    mBattleManager = new CGBBattleManager(1);
    mBattleGrid = mBattleManager->getGrid();
    mSceneGrid = new CGBSceneBattleGrid(mBattleGrid);
    mSceneManager->insertObject(mSceneGrid);
    mCamera = nullptr;
    mSelection = nullptr;
    mTouchMode = TM_SELECT;
    mUIController = new _BattleUIController(this);
}

void CGBBattleTester::setupScene()
{
    // Create some default attacks
    CGBBattleAttackDefinition *tackleAttack = new CGBBattleAttackDefinition("Tackle",  5, 1, CGBBattleAttackDefinition::AT_PHYSICAL, CGBBattleAttackDefinition::CAT_PHYSICAL);
    CGBBattleAttackDefinition *fireAttack = new CGBBattleAttackDefinition("Fire",  9, 1, CGBBattleAttackDefinition::AT_MAGIC, CGBBattleAttackDefinition::CAT_MAGIC_BLACK);
    fireAttack->setMPCost(2);
    CGBBattleAttackDefinition *cureSpell = new CGBBattleAttackDefinition("Cure",  5, 1, CGBBattleAttackDefinition::AT_CURE, CGBBattleAttackDefinition::CAT_MAGIC_WHITE);
    cureSpell->setMPCost(3);
    
    // Create a broad area (get rid of this bad system soon)
    CRArea *area = new CRArea();
    area->setMin(CMVector2(-10000.0f, -10000.0f));
    area->setMax(CMVector2(10000.0f, 10000.0f));
    
    // Create a LIGHT!!!
    CRLight *light = new CRLight();
    light->setPosition(CMVector2(0.0f, 50.0f));
    light->setIntensity(50.0f);
    light->setColor(CMVector3(1.0f, 1.0f, 1.0f));
    area->insertChild(light);
    
    mSceneManager->insertArea(area);
    
    CCHeirarchyNode *areas = new CCHeirarchyNode();
    areas->insertChild(area);
    
    mSceneGrid->setGridScale(30.0f);
    mSceneGrid->setGridMat(dynamic_cast<CRMaterialResource*>(CRResourceManager::Get()->findResourceOfType("floormat", RT_MATERIAL)->getResource()));
    mSceneGrid->setPlaneMat(dynamic_cast<CRMaterialResource*>(CRResourceManager::Get()->findResourceOfType("floormat", RT_MATERIAL)->getResource()));
    mSceneGrid->setArea(area);
    
    for (int i = 0; i < 5; i++)
    {
        CGBSceneBattleActor *actor = new CGBSceneBattleActor();
        actor->setSceneGrid(mSceneGrid);
        std::ostringstream nameStream;
        nameStream << "Bowser " << i;
        actor->setName(nameStream.str());
        actor->setMaxHitPoints(20);
        actor->setMaxMovementPoints(20);
        actor->setPhysicalAttack(5);
        actor->setPhysicalDefence(4);
        actor->setMagicAttack(8);
        actor->setMagicDefense(3);
        
        actor->addAvailableAttack(tackleAttack);
        actor->addAvailableAttack(fireAttack);
        actor->addAvailableAttack(cureSpell);
        
        if (!actor->insertIntoGrid(mBattleGrid, i, 0))
            delete actor;
        
        actor->loadFromResource("bowser");
        actor->getRenderer()->assignAreas(areas);
        mSceneManager->insertObject(actor);
    }
    mBattleManager->updateActorLists();
    mBattleManager->setActiveTeam(0);
    mBattleManager->nextTeam();
    
    CETouchEvent *event = new CETouchEvent();
    event->setReciever(this);
    event->setReusable(true);
    gGlobalEventManager->addEvent(event);
}

CGBBattleTester::~CGBBattleTester()
{
    delete mBattleManager;
}

void CGBBattleTester::setCamera(CSCinemaCamera *camera)
{
    mCamera = camera;
}

void CGBBattleTester::setTouchMode(ETouchMode mode)
{
    mTouchMode = mode;
}

void CGBBattleTester::setAttackMode(CGBBattleAttackDefinition *defn)
{
    mActiveAttack = defn;
    mTouchMode = TM_ATTACK;
}

void CGBBattleTester::receiveEvent(CEEvent *event)
{
    if (event->getEventType() == EVENT_TOUCH && mCamera)
    {
        STouchInfo info = ((CETouchEvent*)event)->getTouchInfo();
        if (info.type == TOUCH_BEGIN)
        {
            mPrevTouchLoc = info.touchLoc;
        }
        else if (info.type == TOUCH_MOVED)
        {
            CMVector2 diff = info.touchLoc;
            diff.subtract(mPrevTouchLoc);
            diff.multiplyScalar(1.0f);
            CMVector3 aimMod(0.0f, -135.0f, -75.0f);
            CMVector3 targetMod(diff.getY(), 0.0f, -diff.getX());
            CMVector3 currentCamLoc = mCamera->getCurrentPosition();
            CMVector3 targetCamLoc = currentCamLoc;
            targetCamLoc.add(targetMod);
            CMVector3 currentAim = currentCamLoc;
            currentAim.add(aimMod);
            CMVector3 targetAim = targetCamLoc;
            targetAim.add(aimMod);
            mCamera->setStart(currentCamLoc, currentAim);
            mCamera->setEnd(targetCamLoc, targetAim);
            mCamera->setDuration(0.01f*diff.length()>0.1?0.01f*diff.length():0.1);
            mCamera->start();
            mPrevTouchLoc = info.touchLoc;
        }
        else if (info.type == TOUCH_TAP)
        {
            CMVector3 pos;
            CMVector3 ray;
            mCamera->getRay(info.touchLoc.getX(), info.touchLoc.getY(), pos, ray);
            CMVector3 intersection;
            CMPlane plane(CMVector3(0.0f, 1.0f, 0.0f), CMVector3(0.0f, 0.0f, 0.0f));
            plane.intercectRay(pos, ray, &intersection);
            U32 cellX = intersection.getX()/mSceneGrid->getScale();
            U32 cellY = intersection.getZ()/mSceneGrid->getScale();
            //printf("FX: %f FY: %f\n", intersection.getX(), intersection.getZ());
            //printf("CellX: %d CellY: %d\n", cellX, cellY);
            
            if (mTouchMode == TM_SELECT)
            {
                // Attempt to select
                mSelection = (CGBSceneBattleActor*)mBattleGrid->getActorAtLocation(cellX, cellY);
                mUIController->setActivePlayer(mSelection);
                if (mSelection)
                {
                    if (mSelection->isDead())
                        return;
                    CMVector3 aimMod(0.0f, -135.0f, -75.0f);
                    CMVector3 targetpos = mSelection->getPosition();
                    CMVector3 camPos = targetpos - aimMod;
                    mCamera->setStart(mCamera->getCurrentPosition(), mCamera->getCurrentPosition()+aimMod);
                    mCamera->setEnd(camPos, targetpos);
                    mCamera->setDuration(mCamera->getCurrentPosition().distanceTo(camPos)*0.01f);
                    mCamera->start();
                }
            }
            else if (mTouchMode == TM_MOVE)
            {
                if (!mSelection)
                {
                    mTouchMode = TM_SELECT;
                    return;
                }
                mSelection->moveToLocation(cellX, cellY);
                mUIController->updateStatsDisplay();
                mTouchMode = TM_SELECT;
            }
            else if (mTouchMode == TM_ATTACK)
            {
                if (!mSelection)
                {
                    mTouchMode = TM_SELECT;
                    mUIController->updateStatsDisplay();
                    return;
                }
                mTouchMode = TM_SELECT;
                mSelection->attack(mActiveAttack, cellX, cellY);
                mUIController->updateStatsDisplay();
                return;
            }
        }
        else if (info.type == TOUCH_DOUBLE_TAP)
        {
            printf("Double Tap!\n");
        }
    }
}