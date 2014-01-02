//
//  BattleActor.cpp
//  mythreal
//
//  Created by Ian Ewell on 2/1/13.
//
//

#include "BattleActor.h"
#include <stdlib.h>

CGBBattleAttackDefinition::CGBBattleAttackDefinition()
{
    mAttackName = "";
    mPower = 1;
    mForce = 1;
    mType = AT_PHYSICAL;
    mCategory = CAT_PHYSICAL;
    mSpecialController = NULL;
    mMPCost = 1;
}

CGBBattleAttackDefinition::CGBBattleAttackDefinition(std::string name, U32 pow, U32 force, type typ, cat category)
{
    mAttackName = name;
    mPower = pow;
    mForce = force;
    mType = typ;
    mCategory = category;
    mSpecialController = NULL;
    mMPCost = 1;
}

CGBBattleAttackDefinition::~CGBBattleAttackDefinition()
{
    
}

void CGBBattleAttackDefinition::setName(std::string name)
{
    mAttackName = name;
}

std::string CGBBattleAttackDefinition::getName()
{
    return mAttackName;
}

void CGBBattleAttackDefinition::setPower(U32 pow)
{
    mPower = pow;
}

U32 CGBBattleAttackDefinition::getPower()
{
    return mPower;
}

void CGBBattleAttackDefinition::setForce(U32 force)
{
    mForce = force;
}

U32 CGBBattleAttackDefinition::getForce()
{
    return mForce;
}

void CGBBattleAttackDefinition::setType(CGBBattleAttackDefinition::type typ)
{
    mType = typ;
}

CGBBattleAttackDefinition::type CGBBattleAttackDefinition::getType()
{
    return mType;
}

void CGBBattleAttackDefinition::setCategory(CGBBattleAttackDefinition::cat category)
{
    mCategory = category;
}

CGBBattleAttackDefinition::cat CGBBattleAttackDefinition::getCategory()
{
    return mCategory;
}

void CGBBattleAttackDefinition::setSpecialController(CGBBattleSpecialAttackController *controller)
{
    mSpecialController = controller;
}

CGBBattleSpecialAttackController* CGBBattleAttackDefinition::getSpecialController()
{
    return mSpecialController;
}

void CGBBattleAttackDefinition::setMPCost(U32 cost)
{
    mMPCost = cost;
}

U32 CGBBattleAttackDefinition::getMPCost()
{
    return mMPCost;
}

SAttackData CGBBattleAttackDefinition::buildAttackData(CGBBattleActor *source)
{
    SAttackData attack;
    attack.source = source;
    attack.power = mPower;
    attack.force = mForce;
    attack.type = (SAttackData::etype)mType;
    attack.specialAttackController = mSpecialController;
    return attack;
}

//////////////////////////////////////////////////////////////////////////

CGBBattleActor::CGBBattleActor()
{
    mBattleGrid = NULL;
    
    mLocationX = 0;
    mLocationY = 0;
    
    mMaxHitPoints = 1;
    mCurrentHitPoints = 1;
    mMaxMovementPoints = 1;
    mCurrentMovementPoints = 1;
    
    mPhysicalAttack = 0;
    mPhysicalDefense = 1;
    mMagicAttack = 0;
    mMagicDefense = 1;
    mActorMass = 0;
    
    mImmovable = false;
    mDead = false;
}

CGBBattleActor::CGBBattleActor(CGBBattleGrid *grid, U32 locX, U32 locY)
{
    mBattleGrid = grid;
    
    mLocationX = locX;
    mLocationY = locY;
    
    mMaxHitPoints = 1;
    mCurrentHitPoints = 1;
    mMaxMovementPoints = 1;
    mCurrentMovementPoints = 1;
    
    mPhysicalAttack = 0;
    mPhysicalDefense = 1;
    mMagicAttack = 0;
    mMagicDefense = 1;
    mActorMass = 0;
    
    mImmovable = false;
    mDead = false;
}

CGBBattleActor::~CGBBattleActor()
{
    
}

bool CGBBattleActor::insertIntoGrid(CGBBattleGrid *grid, U32 locX, U32 locY)
{
    if (!grid->insertActorAtLocation(locX, locY, this))
        return false;
    mBattleGrid = grid;
    mLocationX = locX;
    mLocationY = locY;
    return true;
}

void CGBBattleActor::setLocation(F32 locX, F32 locY)
{
    mLocationX = locX;
    mLocationY = locY;
}

F32 CGBBattleActor::getLocationX()
{
    return mLocationX;
}

F32 CGBBattleActor::getLocationY()
{
    return mLocationY;
}

void CGBBattleActor::setMaxHitPoints(U32 HP)
{
    mMaxHitPoints = HP;
    mCurrentHitPoints = HP;
}

void CGBBattleActor::setCurrentHitPoints(U32 HP)
{
    mCurrentHitPoints = HP;
    if (mCurrentHitPoints > mMaxHitPoints)
        mCurrentHitPoints = mMaxHitPoints;
}

U32 CGBBattleActor::getMaxHitPoints()
{
    return mMaxHitPoints;
}

U32 CGBBattleActor::getCurrentHitPoints()
{
    return mCurrentHitPoints;
}

void CGBBattleActor::setMaxMovementPoints(U32 MP)
{
    mMaxMovementPoints = MP;
    mCurrentMovementPoints = MP;
}

void CGBBattleActor::setCurrentMovementPoints(U32 MP)
{
    mCurrentMovementPoints = MP;
    if (mCurrentMovementPoints > mMaxMovementPoints)
        mCurrentMovementPoints = mMaxMovementPoints;
}

U32 CGBBattleActor::getMaxMovementPoints()
{
    return mMaxMovementPoints;
}

U32 CGBBattleActor::getCurrentMovementPoints()
{
    return mCurrentMovementPoints;
}

void CGBBattleActor::setPhysicalAttack(U32 atck)
{
    mPhysicalAttack = atck;
}

U32 CGBBattleActor::getPhysicalAttack()
{
    return mPhysicalAttack;
}

void CGBBattleActor::setPhysicalDefence(U32 dfnc)
{
    mPhysicalDefense = (dfnc>0)?dfnc:1;
}

U32 CGBBattleActor::getPhysicalDefence()
{
    return mPhysicalDefense;
}

void CGBBattleActor::setMagicAttack(U32 atck)
{
    mPhysicalAttack = atck;
}

U32 CGBBattleActor::getMagicAttack()
{
    return mPhysicalAttack;
}

void CGBBattleActor::setMagicDefense(U32 dfnc)
{
    mPhysicalDefense = (dfnc>0)?dfnc:1;
}

U32 CGBBattleActor::getMagicDefense()
{
    return mPhysicalDefense;
}

void CGBBattleActor::setName(const std::string &name)
{
    mName = name;
}

std::string CGBBattleActor::getName()
{
    return mName;
}

bool CGBBattleActor::isDead()
{
    return mDead;
}

/////////////////////////////

bool CGBBattleActor::canMoveToLocation(U32 x, U32 y)
{
    if (mDead || !mBattleGrid)
        return false;
    
    // A* needed later, but say we can for now
    return mBattleGrid->canMove(this, x, y);
}

S32 CGBBattleActor::getMovementCost(U32 x, U32 y)
{
    // 1 MP for each tile difference for now
    return abs(x-mLocationX) + abs(y-mLocationY);
}

bool CGBBattleActor::moveToLocation(U32 x, U32 y)
{
    if (mDead || !mBattleGrid)
        return false;
    
    if (!canMoveToLocation(x, y))
        return false;
    
    if (getMovementCost(x, y) > mCurrentMovementPoints)
        return false;
    
    mCurrentMovementPoints -= getMovementCost(x, y);
    bool ret = mBattleGrid->moveActorToLocation(this, x, y);
    onLocationChange();
    return ret;
}

void CGBBattleActor::applyDamage(U32 amt)
{
    if (amt >= mCurrentHitPoints)
    {
        mCurrentHitPoints = 0;
        mDead = true;
    }
    else
        mCurrentHitPoints -= amt;
}

void CGBBattleActor::applyHeal(U32 amt)
{
    if (mDead)
        return;
    mCurrentHitPoints += amt;
    if (mCurrentHitPoints > mMaxHitPoints)
        mCurrentHitPoints = mMaxHitPoints;
}

void CGBBattleActor::receiveAttack(SAttackData attack)
{
    U32 damage = 0;
    
    // Apply special
    if (attack.specialAttackController)
        attack.specialAttackController->apply(this);
    
    // find nature of attack
    switch (attack.type)
    {
        case SAttackData::AT_PHYSICAL:
            damage = attack.power/mPhysicalDefense;
            applyDamage(damage);
            break;
        case SAttackData::AT_MAGIC:
            damage = attack.power/mMagicDefense;
            applyDamage(damage);
            break;
        case SAttackData::AT_CURE:
            damage = attack.power;
            applyHeal(damage);
            break;
        case SAttackData::AT_SPECIAL:
        default:
            break;
    }
}

void CGBBattleActor::addAvailableAttack(CGBBattleAttackDefinition *attack)
{
    if (attack)
        mAvailableAttacks.push_back(attack);
}

std::list<CGBBattleAttackDefinition*>* CGBBattleActor::getAvailableAttacks()
{
    return &mAvailableAttacks;
}

bool CGBBattleActor::canUseAttack(CGBBattleAttackDefinition *attack)
{
    // Linear search to find attack
    for (std::list<CGBBattleAttackDefinition*>::iterator it = mAvailableAttacks.begin(); it != mAvailableAttacks.end(); it++)
    {
        if (*it == attack)
            return true;
    }
    return false;
}

bool CGBBattleActor::canAttack(CGBBattleAttackDefinition *attack, U32 targetX, U32 targetY)
{
    if (!canUseAttack(attack))
        return false;
    
    if (!mBattleGrid->isOccupied(targetX, targetY))
        return false;
    
    if (abs(mLocationX-targetX) > 1 || abs(mLocationY-targetY) > 1)
        return false;
    
    if (attack->getMPCost() > mCurrentMovementPoints)
        return false;
    
    return true;
}

bool CGBBattleActor::attack(CGBBattleAttackDefinition *attack, U32 targetX, U32 targetY)
{
    if (!canAttack(attack, targetX, targetY))
        return false;
    CGBBattleActor *target = mBattleGrid->getActorAtLocation(targetX, targetY);
    SAttackData data = attack->buildAttackData(this);
    target->receiveAttack(data);
    mCurrentMovementPoints -= attack->getMPCost();
    return true;
}