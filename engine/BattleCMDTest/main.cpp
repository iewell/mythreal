//
//  main.cpp
//  BattleCMDTest
//
//  Created by Ian Ewell on 3/1/13.
//
//

#include <iostream>
#include <vector>
#include "BattleManager.h"
#include "BattleGrid.h"

using namespace std;

class gameManager
{
private:
    CGBBattleManager *manager;
    bool endGame;
    
    EDirection parseDirection(string input)
    {
        if (input == "N")
            return DIR_NORTH;
        else if (input == "NE")
            return DIR_NORTHEAST;
        else if (input == "E")
            return DIR_EAST;
        else if (input == "SE")
            return DIR_SOUTHEAST;
        else if (input == "S")
            return DIR_SOUTH;
        else if (input == "SW")
            return DIR_SOUTHWEST;
        else if (input == "W")
            return DIR_WEST;
        else if (input == "NW")
            return DIR_NORTHWEST;
        else
            return DIR_INVALID;
    }
    
    void turn()
    {
        CGBBattleActor *actor = manager->getActiveActor();
        if (actor->isDead())
        {
            manager->nextActor();
            return;
        }
        cout << "\n";
        cout << actor->getName() << " is currently active.\n";
        cout << "HP: " << actor->getCurrentHitPoints() << "/" << actor->getMaxHitPoints();
        cout << " MP: " << actor->getCurrentMovementPoints() << "/" << actor->getMaxMovementPoints() << "\n";
        
        while (actor->getCurrentMovementPoints() > 0 && !actor->isDead())
        {
            cout << actor->getName() << " has " << actor->getCurrentMovementPoints() << " movement points left.\n";
            cout << "Current location is (" << actor->getLocationX() << ", " << actor->getLocationY() << ")\n";
            cout << "Please select a command:\n";
            cout << "View (G)rid, (M)ove, (A)ttack, (N)ext Actor, (Q)uit\n";
            string command;
            getline(cin, command);
            if (command == "G")
            {
                manager->getGrid()->printGrid();
            }
            else if (command == "M")
            {
                cout << "Enter direction (i.e. NW for northwest): ";
                string dir;
                getline(cin, dir);
                EDirection direction = parseDirection(dir);
                if (direction == DIR_INVALID)
                {
                    cout << "Invalid direction\n";
                    continue;
                }
                if (!manager->moveActorInDirection(direction))
                {
                    cout << "Actor was unable to move\n";
                    continue;
                }
                cout << actor->getName() << " Has moved to (" << actor->getLocationX() << ", " << actor->getLocationY() << ")\n";
            }
            else if (command == "A")
            {
                CGBBattleAttackDefinition::cat category = CGBBattleAttackDefinition::CAT_PHYSICAL;
                cout << "(P)hysical, (B)lack Magic, (W)hite Magic";
                string cat;
                getline(cin, cat);
                if (cat == "P")
                    category = CGBBattleAttackDefinition::CAT_PHYSICAL;
                else if (cat == "B")
                    category = CGBBattleAttackDefinition::CAT_MAGIC_BLACK;
                else if (cat == "W")
                    category = CGBBattleAttackDefinition::CAT_MAGIC_WHITE;
                else
                {
                    cout << "Invalid category\n";
                    continue;
                }
                
                // Build attack list while concurrently printing
                std::list<CGBBattleAttackDefinition*> *attacks = actor->getAvailableAttacks();
                std::vector<CGBBattleAttackDefinition*> catAttacks;
                int i = 0;
                for (std::list<CGBBattleAttackDefinition*>::iterator it = attacks->begin(); it != attacks->end(); it++)
                {
                    if ((*it)->getCategory() == category)
                    {
                        catAttacks.push_back((*it));
                        cout << i << ": " << (*it)->getName() << " MP " << (*it)->getMPCost() << "\n";
                        i++;
                    }
                }
                
                cout << "Select attack:";
                int index;
                cin >> index;
                
                cout << "Enter direction:";
                cin.get();
                string direction;
                getline(cin, direction);
                EDirection dir = parseDirection(direction);
                if (dir == DIR_INVALID)
                {
                    cout << "Invalid direction.\n";
                    continue;
                }
                
                U32 attackloc[2];
                manager->getLocationFromDirection(dir, attackloc);
                CGBBattleActor *target = manager->getGrid()->getActorAtLocation(attackloc[0], attackloc[1]);
                if (!target)
                {
                    cout << "No target in selected direction.\n";
                    continue;
                }
                
                int beforeHP = target->getCurrentHitPoints();
                CGBBattleAttackDefinition *atttack = catAttacks.at(index);
                if (!atttack)
                {
                    cout << "Invalid attack selected.\n";
                    continue;
                }
                if (!manager->attackInDirection(dir, atttack))
                {
                    cout << "Unable to perform attack.\n";
                    continue;
                }
                
                cout << "Dealt " << beforeHP-target->getCurrentHitPoints() << " damage to " << target->getName() << "\n";
                
            }
            else if (command == "N")
            {
                break;
            }
            else if (command == "Q")
            {
                endGame = true;
                break;
            }
            else
                cout << "Invalid command.\n";
        }
        manager->nextActor();
    }
    
    void startGame()
    {
        cout << "The battle has started.\n";
        cin.get();
        cout << "This is the initial playing grid.\n";
        cin.get();
        manager->getGrid()->printGrid();
        cout << "Press anything to begin\n";
        cin.get();
        while (!endGame)
            turn();
    }
    
public:
    gameManager()
    {
        manager = new CGBBattleManager(1);
        endGame = false;
    }
    
    void run()
    {
        std::cout << "Welcome to the MR RPG battle system!\n";
        std::cout << "(C) Copyright 2013 Ian Ewell.\n";
        std::cout << "Press enter to continue.\n";
        
        std::string input;
        std::getline(std::cin, input);
        
        std::cout << "If you would like to play with the default grid, press (d).\n";
        std::cout << "To play with a custom grid, press (c).\n";
        std::cout << "To exit, just press return.\n";
        std::getline(std::cin, input);
        
        if (input == "c")
        {
            std::cout << "Custom grid functionality not implemented yet.\n";
            return;
        }
        else if (input == "d")
        {
            std::cout << "Game will be played on 15x15 grid with 15 standard actors\n";
            std::cout << "Press enter to start the game.\n";
            std::getline(std::cin, input);
            manager->createTestFormation();
            startGame();
        }
    }
};

int main(int argc, const char * argv[])
{
    gameManager manager;
    manager.run();
    return 0;
}

