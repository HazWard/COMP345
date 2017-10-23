/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/

#include <vector>
#include "../include/cards.h"
#include "../include/dice.h"
#include "../include/map.h"

#pragma once

class Player
{
    /*
        Player class implements the different methods to interact with the
        game as well as hold a Hand and a list of countries the player owns.
    */
    public:
        Player();
        Player(std::string playerName, Hand* playerHand, std::vector<Node*>* playerCountries);
        ~Player();
        int roll(int nbOfDice);
        void reinforce();
        void attack();
        void fortify();
        void setHand(Hand* targetHand);
        Hand* getHand();
        void setName(std::string targetName);
        std::string getName();
        void setDice(Dice* targetDice);
        void addCountry(Node* newCountry);
        void setCountries(std::vector<Node*>* targetCountries);
        std::vector<Node*> getCountries();
    private:
        std::string name;
        Hand* hand;
        Dice* dice;
        std::vector<Node*> countries;
};
