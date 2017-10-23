/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Class representing a Risk Player
*/
#include <iostream>
#include <vector>
#include "../include/player.h"
#include "../include/cards.h"
#include "../include/dice.h"
#include "../include/map.h"


// Constants
int MIN_NUMBER_OF_ARMIES = 3;

// Constructors
Player::Player() : name(""), hand(new Hand), countries(std::vector<Node*>()), dice(new Dice) { }

Player::Player(std::string playerName, Hand* playerHand, std::vector<Node*>* playerCountries)
{
    this->setName(playerName);
    this->setHand(playerHand);
    this->setCountries(playerCountries);
    // Create the dice rolling facility for the Player
    this->setDice(new Dice());
}
// Destructor
Player::~Player(void)
{
    delete hand;
    delete dice;
    hand = NULL;
    dice = NULL;
}

// Setters and Getters
void Player::setName(std::string targetName)
{
    this->name = targetName;
}

std::string Player::getName()
{
    return this->name;
}

void Player::setDice(Dice* targetDice)
{
    this->dice = targetDice;
}

Hand* Player::getHand()
{
    return this->hand;
}

void Player::setCountries(std::vector<Node *> *targetCountries)
{
    // Build the countries vector
    for (int i = 0; i < targetCountries->size(); ++i)
    {
        this->countries.push_back((*targetCountries)[i]);
    }
}

void Player::addCountry(Node* newCountry)
{
    this->countries.push_back(newCountry);
}

std::vector<Node*> Player::getCountries()
{
    return this->countries;
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
    std::cout << "is reinforcing troops!" << std::endl;
    int countryBonus = this->countries.size() / MIN_NUMBER_OF_ARMIES;
    if (countryBonus >= 3)
    {
        // TODO: Check if a whole continent is owned
    }
    else
    {
        std::cout << "Not enough armies to reinforce troops." << std::endl;
    }
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