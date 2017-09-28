/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/

#include <vector>
#include "../include/cards.h"
#include "../include/dice.h"

#pragma once

class Player
{
    /*
        Player class implements the different methods to interact with the
        game as well as hold a Hand and a list of countries the player owns.
    */
    public:
        Player(Hand* playerHad, std::vector<Node*> playerCountries);
        ~Player();
        int roll(int nbOfDice);
        void reinforce();
        void attack();
        void fortify();
        Hand* hand;
        Dice* dice;
        std::vector<Country*> countries;
};
