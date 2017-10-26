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
    string sourceStr;
    int sourceIndex=-1;
    string destinationStr;
    int destinationIndex=-1;
    int armNum;
    bool attempted = false;

    //this while loop asks for source and loops if not owned
    while (sourceIndex<0) {

        if(attempted)
            std::cout << "Country is not owned please enter a country you own" << std::endl;
        else
            std::cout << "Please enter the source country" << std::endl;

        std::cin >> sourceStr;

        //Check if owned and get index
        for(int i =0 ; i< this->countries.size() ; i++) {
                if (sourceStr==countries[i]->getCountry().getName())
                    sourceIndex=i;
        }
        attempted=true;
    }

    attempted=false;

    //this while loop asks for destination and loops if not owned or if not connected to source through owned countries
    while (destinationIndex<0 || false) {//when not connected through owned countries is implemented change condition

        if(false)//when not connected through owned countries is implemented change condition
            std::cout << "Destination is not connected to source please enter a country connected to source" << std::endl;
        if(attempted)
            std::cout << "Country is not owned please enter a country you own" << std::endl;
        else
            std::cout << "Please enter the destination country" << std::endl;

        std::cin >> destinationStr;

        //Check if owned and get index
        for(int i =0 ; i< this->countries.size() ; i++) {
                if (destinationStr==countries[i]->getCountry().getName())
                        destinationIndex=i;
            }



            attempted=true;
        }

    attempted=false;

    while(armNum >= this->countries[sourceIndex]->getCountry().getNbrArmies() || armNum <= 0){

        if(attempted)
            std::cout << "invalid number of armies please reenter a valid number" << endl;
        else
            std::cout << "Please enter number of armies to move" << endl;

        std::cin >> armNum;

        attempted=true;
    }

    //Setting new number of armies in source and destination
    this->countries[sourceIndex]->getCountry().setNbrArmies(this->countries[sourceIndex]->getCountry().getNbrArmies()-armNum);
    this->countries[destinationIndex]->getCountry().setNbrArmies(this->countries[sourceIndex]->getCountry().getNbrArmies()+armNum);
    std::cout << armNum+" armies have benn moved from "+sourceStr+" to "+destinationStr << std::endl;
}