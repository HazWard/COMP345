/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header for the Player class
*/

#include <iostream>
#include <list>
#include "player.h"

Player::Player(std::list<int> playerHand, std::list<int> playerCountries)
{

    if (hand.size() < 1 || countries.size() < 1)
    {
        std::cout << "Invalid arguments for Player contructor." << std::endl;
        // TODO: Find a way to get rid of an invalid object
    }
    else
    {
        hand = playerHand;
        countries = playerCountries;
    }
}

Player::~Player()
{
    // Actions to remove all data members
}

int Player::diceFactory(int nbOfDice)
{
    // Perform a roll
    return 0;
}

void Player::reinforce()
{
    // Perform actions to reinforce
}

void Player::attack()
{
    // Perform actions to attack
}

void Player::fortify()
{
    // Perform actions to fortify
}
