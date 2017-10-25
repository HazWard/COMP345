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

// Constructors
Player::Player() : hand(new Hand), countries(std::vector<Node*>()), dice(new Dice) { }

Player::Player(Hand* playerHand, std::vector<Node*>* playerCountries)
{
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
void Player::setHand(Hand *targetHand)
{
    this->hand = targetHand;
}

Hand* Player::getHand()
{
    return this->hand;
}

void Player::setDice(Dice* targetDice)
{
    this->dice = targetDice;
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
    std::cout << "Player is reinforcing troops!" << std::endl;
}

void Player::attack()
{
    // Perform actions to attack
    std::cout << "Player is attacking using its troops!" << std::endl;
}

void Player::fortify()
{
    string source;
    string destination;
    int armNum;
    bool attempted = false;

    //Check if countries are connected
    while(/*source and destination are not connected*/) {
        if(attempted)
            cout << "Countries entered are not connected try again" << endl;
        //Ask for source country
        std::cout << "Please enter the source country" << std::endl;
        std::cin >> source;
        //Verify eligibility
        while (/*Code to check if country is owned by player*/) {
            std::cout << "Country is not owned please enter a country you own" << std::endl;
            std::cin >> source;
        }

        //Ask for destination country
        std::cout << "Please enter the destination country" << std::endl;
        std::cin >> destination;
        //Verify eligibility
        while (/*Code to check if country is owned by player*/) {
            std::cout << "Country is not owned please enter a country you own" << std::endl;
            std::cin >> destination;
        }
        attempted=true;
    }

    //Ask for number of armies to move
    std::cout << "Please enter number of armies to move" << endl;
    std::cin >> armNum;
    while(armNum >= /*source army number*/ || armNum <= 0){
        std::cout << "invalid number of armies please reenter a valid number" << endl;
        std::cin >> armNum;
    }

    source.setNumberOfArmies()
}