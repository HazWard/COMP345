/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/

#include <list>
// #include "cards.h"
// #include "dice.h"

#pragma once

class Player
{
  public:
    Player(std::list<int>, std::list<int>);
    ~Player();
    int diceFactory(int);
    void reinforce();
    void attack();
    void fortify();
    std::list<int> countries; // TODO: Change to array of Country
    std::list<int> hand; // TODO: Change to array of Cards
};
