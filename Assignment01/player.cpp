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
    hand = playerHand;
    countries = playerCountries;
    std::cout << hand.size() << std::endl;
    std::cout << countries.size() << std::endl;
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

int main()
{
    std::list<int> l = { 7, 5, 16, 8 };
    std::cout << "\nIn Player constructor: " << std::endl;
    Player p = Player(l, l);
    return 0;
}