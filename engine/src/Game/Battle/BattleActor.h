//
//  BattleActor.h
//  mythreal
//
//  Created by Ian Ewell on 2/1/13.
//
//

#ifndef __mythreal__BattleActor__
#define __mythreal__BattleActor__

#include <iostream>
#include <list>
#include "Types.h"
#include "BattleGrid.h"
#include "Vector.h"

class CGBBattleActor;
class CGBBattleGrid;

class CGBBattleSpecialAttackController
{
public:
    virtual void apply(CGBBattleActor *target);
};

typedef struct
{
    CGBBattleActor *source;
    U32 power;
    U32 force;
    enum etype
    {
        AT_PHYSICAL,
        AT_MAGIC,
        AT_CURE,
        AT_SPECIAL,
    } type;
    CGBBattleSpecialAttackController *specialAttackController;
} SAttackData;

class CGBBattleAttackDefinition
{
private:
    std::string mAttackName;
    U32 mPower;
    U32 mForce;
    
public:
    enum type
    {
        AT_PHYSICAL,
        AT_MAGIC,
        AT_CURE,
        AT_SPECIAL,
    } mType;
    
    enum cat
    {
        CAT_PHYSICAL,
        CAT_MAGIC_BLACK,
        CAT_MAGIC_WHITE,
    } mCategory;
    
private:
    CGBBattleSpecialAttackController *mSpecialController;
    
    U32 mMPCost;
    
public:
    CGBBattleAttackDefinition();
    CGBBattleAttackDefinition(std::string name, U32 pow, U32 force, type typ, cat category);
    virtual ~CGBBattleAttackDefinition();
    
    void setName(std::string name);
    std::string getName();
    
    void setPower(U32 pow);
    U32 getPower();
    void setForce(U32 force);
    U32 getForce();
    void setType(type typ);
    type getType();
    void setCategory(cat category);
    cat getCategory();
    
    void setSpecialController(CGBBattleSpecialAttackController *controller);
    CGBBattleSpecialAttackController* getSpecialController();
    
    void setMPCost(U32 cost);
    U32 getMPCost();

    SAttackData buildAttackData(CGBBattleActor *source);
};

class CGBBattleActor
{
protected:
    // Pointer to grid
    CGBBattleGrid *mBattleGrid;
    
    // Location
    U32 mLocationX;
    U32 mLocationY;
    
    // Battle Stats
    U32 mMaxHitPoints;
    U32 mCurrentHitPoints;
    U32 mMaxMovementPoints;
    U32 mCurrentMovementPoints;
    
    // Stats used in damage calculations
    U32 mPhysicalAttack;
    U32 mPhysicalDefense;
    U32 mMagicAttack;
    U32 mMagicDefense;
    U32 mActorMass;
    
    // Other variables
    std::string mName;
    bool mImmovable;
    bool mDead;
    
    // Attack information
    std::list<CGBBattleAttackDefinition*> mAvailableAttacks;
    
public:
    CGBBattleActor();
    CGBBattleActor(CGBBattleGrid *grid, U32 locX, U32 locY);
    virtual ~CGBBattleActor();
    
    virtual bool insertIntoGrid(CGBBattleGrid *grid, U32 locX, U32 locY);
    
    // Accessor methods :/
    
    // Do not call setLocation directly. Use moveToLocation()
    void setLocation(F32 locX, F32 locY);
    F32 getLocationX();
    F32 getLocationY();
    
    void setMaxHitPoints(U32 HP);
    void setCurrentHitPoints(U32 HP);
    U32 getMaxHitPoints();
    U32 getCurrentHitPoints();
    
    void setMaxMovementPoints(U32 MP);
    void setCurrentMovementPoints(U32 MP);
    U32 getMaxMovementPoints();
    U32 getCurrentMovementPoints();
    
    void setPhysicalAttack(U32 atck);
    U32 getPhysicalAttack();
    
    void setPhysicalDefence(U32 dfnc);
    U32 getPhysicalDefence();
    
    void setMagicAttack(U32 atck);
    U32 getMagicAttack();
    
    void setMagicDefense(U32 dfnc);
    U32 getMagicDefense();
    
    void setName(const std::string &name);
    std::string getName();
    
    bool isDead();
    
    // Movement functions
    bool canMoveToLocation(U32 x, U32 y);
    S32 getMovementCost(U32 x, U32 y);
    bool moveToLocation(U32 x, U32 y);
    virtual void onLocationChange() {}
    
    // Attack receiving functions
    void applyDamage(U32 amt);
    void applyHeal(U32 amt);
    void receiveAttack(SAttackData attack);
    
    // Offensive attack functions
    void addAvailableAttack(CGBBattleAttackDefinition *attack);
    std::list<CGBBattleAttackDefinition*>* getAvailableAttacks();
    bool canUseAttack(CGBBattleAttackDefinition *attack);
    bool canAttack(CGBBattleAttackDefinition *attack, U32 targetX, U32 targetY);
    bool attack(CGBBattleAttackDefinition *attack, U32 targetX, U32 targetY);
};

#endif /* defined(__mythreal__BattleActor__) */
