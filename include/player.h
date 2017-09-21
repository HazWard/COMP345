/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/

#include <list>
#include "../include/cards.h"

#pragma once

class Player
{
    /*
    Player class implements the different methods to interact with the game
    as well as hold a Hand and a list of countries the player owns.
    */
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
