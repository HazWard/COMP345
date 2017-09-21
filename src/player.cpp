/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Class representing a Risk Player
*/

#include <iostream>
#include <list>
#include "../include/player.h"
#include "../include/cards.h"

Player::Player(Hand* playerHand, std::list<int>* playerCountries)
{

    if (countries.size() < 1)
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

Player::~Player(void)
{
    delete hand;
    hand = NULL;
    countries = NULL;
}

int Player::diceFactory(int nbOfDice)
{
    // Perform a roll
    return 0;
}

void Player::reinforce()
{
    // Perform actions to reinforce
    std::cout << "Player is reinforcing troops!" << std::endl;
}

void Player::attack()
{
    // Perform actions to attack
    std::cout << "Player is attacking using its troops!" << std::endl;
}

void Player::fortify()
{
    // Perform actions to fortify
    std::cout << "Player is fortifying!" << std::endl;
}

int main()
{
    return 0;
}
