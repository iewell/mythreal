//
//  BattleManager.cpp
//  mythreal
//
//  Created by Ian Ewell on 3/1/13.
//
//

#include "BattleManager.h"
#include <sstream>

CGBBattleManager::CGBBattleManager(U32 teamCount)
{
    mBattleGrid = new CGBBattleGrid(5, 5);
    mBattleGrid->construct();
    mTeamCount = teamCount<1?1:teamCount;
    mTeamActorLists = new std::list<CGBBattleActor*>[teamCount];
    
    mActiveActor = NULL;
    mActiveActorIndex = 0;
    mActiveTeam = 0;
}

CGBBattleManager::~CGBBattleManager()
{
    delete mBattleGrid;
    delete [] mTeamActorLists;
}

void CGBBattleManager::createTestFormation()
{
    // Create some default attacks
    CGBBattleAttackDefinition *tackleAttack = new CGBBattleAttackDefinition("Tackle",  5, 1, CGBBattleAttackDefinition::AT_PHYSICAL, CGBBattleAttackDefinition::CAT_PHYSICAL);
    CGBBattleAttackDefinition *fireAttack = new CGBBattleAttackDefinition("Fire",  9, 1, CGBBattleAttackDefinition::AT_MAGIC, CGBBattleAttackDefinition::CAT_MAGIC_BLACK);
    fireAttack->setMPCost(2);
    CGBBattleAttackDefinition *cureSpell = new CGBBattleAttackDefinition("Cure",  5, 1, CGBBattleAttackDefinition::AT_CURE, CGBBattleAttackDefinition::CAT_MAGIC_WHITE);
    cureSpell->setMPCost(3);
    
    for (int i = 0; i < 15; i++)
    {
        CGBBattleActor *actor = new CGBBattleActor();
        std::ostringstream nameStream;
        nameStream << "Actor " << i;
        actor->setName(nameStream.str());
        actor->setMaxHitPoints(20);
        actor->setMaxMovementPoints(3);
        actor->setPhysicalAttack(5);
        actor->setPhysicalDefence(4);
        actor->setMagicAttack(8);
        actor->setMagicDefense(3);
        
        actor->addAvailableAttack(tackleAttack);
        actor->addAvailableAttack(fireAttack);
        actor->addAvailableAttack(cureSpell);
        
        if (!actor->insertIntoGrid(mBattleGrid, i, 0))
            delete actor;
    }
    updateActorLists();
    mActiveTeam = 0;
    mActiveActor = mTeamActorLists[0].front();
}

CGBBattleGrid* CGBBattleManager::getGrid()
{
    return mBattleGrid;
}

void CGBBattleManager::nextActor()
{
    mActiveActorIndex++;
    if (mActiveActorIndex >= mTeamActorLists[mActiveTeam].size())
    {
        nextTeam();
    }
    std::list<CGBBattleActor*>::iterator it = mTeamActorLists[mActiveTeam].begin();
    for (int i = 0; i < mActiveActorIndex; i++)
        it++;
    mActiveActor = *it;
    checkCurrentActor();
}

void CGBBattleManager::setActiveActor(CGBBattleActor *actor)
{
    int i = 0;
    for (std::list<CGBBattleActor*>::iterator it = mTeamActorLists[mActiveTeam].begin() ; it != mTeamActorLists[mActiveTeam].end(); it++)
    {
        if (actor == *it)
        {
            mActiveActor = actor;
            mActiveActorIndex = i;
            return;
        }
        i++;
    }
}

CGBBattleActor* CGBBattleManager::getActiveActor()
{
    return mActiveActor;
}

void CGBBattleManager::nextTeam()
{
    mActiveTeam++;
    if (mActiveTeam >= mTeamCount)
        mActiveTeam = 0;
    mActiveActorIndex = 0;
    mActiveActor = mTeamActorLists[mActiveTeam].front();
    
    // Restore MP of all actors on the team
    for (std::list<CGBBattleActor*>::iterator it = mTeamActorLists[mActiveTeam].begin(); it != mTeamActorLists[mActiveTeam].end(); it++)
        (*it)->setCurrentMovementPoints((*it)->getMaxMovementPoints());
}

void CGBBattleManager::checkCurrentActor()
{
    if (mActiveActor->isDead() || mActiveActor->getCurrentMovementPoints() == 0)
        nextActor();
}

void CGBBattleManager::setActiveTeam(U32 team)
{
    mActiveTeam = team<mTeamCount?team:mTeamCount-1;
}

U32 CGBBattleManager::getActiveTeam()
{
    return mActiveTeam;
}

void CGBBattleManager::updateActorLists()
{
    // Clear our old lists first
    for (int i = 0; i < mTeamCount; i++)
        mTeamActorLists[i].clear();
    
    std::list<CGBBattleActor*>* actors = mBattleGrid->getActors();
    for (std::list<CGBBattleActor*>::iterator it = actors->begin() ; it != actors->end(); it++)
    {
        // for now push back to team 0
        mTeamActorLists[0].push_back(*it);
    }
}

bool CGBBattleManager::moveActorToLoc(U32 x, U32 y)
{
    if (!mActiveActor)
        return false;

    if (mActiveActor->moveToLocation(x, y))
        return true;
    return false;
}

void CGBBattleManager::getLocationFromDirection(EDirection dir, U32 *loc)
{
    if (!mActiveActor || !loc)
        return;
    
    U32 targetX = mActiveActor->getLocationX();
    U32 targetY = mActiveActor->getLocationY();
    switch (dir)
    {
        case DIR_NORTH:
            targetY += 1;
            break;
        case DIR_NORTHEAST:
            targetX += 1;
            targetY += 1;
            break;
        case DIR_EAST:
            targetX += 1;
            break;
        case DIR_SOUTHEAST:
            targetX += 1;
            targetY -= 1;
            break;
        case DIR_SOUTH:
            targetY -= 1;
            break;
        case DIR_SOUTHWEST:
            targetX -= 1;
            targetY -= 1;
            break;
        case DIR_WEST:
            targetX -= 1;
            break;
        case DIR_NORTHWEST:
            targetX -= 1;
            targetY += 1;
            break;
        case DIR_INVALID:
            return;
            break;
    }
    loc[0] = targetX;
    loc[1] = targetY;
}

bool CGBBattleManager::moveActorInDirection(EDirection dir)
{
    if (!mActiveActor)
        return false;
    
    U32 target[2];
    getLocationFromDirection(dir, target);
    
    if (mActiveActor->moveToLocation(target[0], target[1]))
        return true;
    return false;
}

bool CGBBattleManager::attackInDirection(EDirection dir, CGBBattleAttackDefinition *attack)
{
    if (!mActiveActor)
        return false;
    
    U32 target[2];
    getLocationFromDirection(dir, target);
    
    if (mActiveActor->attack(attack, target[0], target[1]))
        return true;
    return false;
}