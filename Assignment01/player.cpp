/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header for the Player class
*/
#include <iostream>
#include "player.h"

Player::Player(int playerHand[], int playerCountries[])
{
    if (sizeof(playerHand)/sizeof(playerHand[0]) < 1 || sizeof(playerCountries)/sizeof(playerCountries[0]) < 1) {
        std::cout << "Invalid argument for Player constructor.";
    }
    hand = playerHand;
    countries = playerCountries;
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

