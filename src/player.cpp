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
#include "../include/dice.h"
#include "../include/map.h"

Player::Player(Hand* playerHand, std::vector<Node*>* playerCountries)
{
    this->hand = playerHand;

    this->countries = std::vector<Node*>();

    // Build the countries vector
    for (int i = 0; i < playerCountries->size(); ++i)
    {
        this->countries.push_back((*playerCountries)[i]);
    }
    
    // Create the dice rolling facility for the Player
    this->dice = new Dice();
}

Player::~Player(void)
{
    delete hand;
    delete dice;
    hand = NULL;
    dice = NULL;
}

int Player::roll(int nbOfDice)
{
    // Perform rolls
    int roll = 0;
    for (int i = 0; i < nbOfDice; ++i)
    {
        roll += this->dice->numGenerator();
    }
    return roll;
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