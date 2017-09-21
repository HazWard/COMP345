/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/

#include <list>
#include "../cards.h"
// #include "dice.h"

#pragma once

class Player
{
  public:
    Player(Hand*, std::list<int>*);
    ~Player(void);
    int diceFactory(int);
    void reinforce(void);
    void attack(void);
    void fortify(void);
    Hand* hand;
    std::list<int>* countries; // TODO: Change to array of Country
};
