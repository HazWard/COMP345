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
static const int MIN_NUMBER_OF_ARMIES = 3;
static const int MIN_NUMBER_OF_CARDS = 5;
static const int INFANTRY_BONUS = 1;
static const int CAVALRY_BONUS = 5;
static const int ARTILLERY_BONUS = 10;

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
    std::cout << this->name << " is reinforcing troops!" << std::endl;
    int totalNbArmies = 0;
    if (this->countries.size() >= MIN_NUMBER_OF_ARMIES)
    {
        totalNbArmies = this->countries.size() / MIN_NUMBER_OF_ARMIES;
        // TODO: Check if a whole continent is owned
        if(this->hand->getTotalCards() >= MIN_NUMBER_OF_CARDS)
        {
            totalNbArmies = (this->hand->exchange(Card::INFANTRY)) ? INFANTRY_BONUS + totalNbArmies : totalNbArmies;
            totalNbArmies = (this->hand->exchange(Card::ARTILLERY)) ? ARTILLERY_BONUS + totalNbArmies : totalNbArmies;
            totalNbArmies = (this->hand->exchange(Card::CAVALRY)) ? CAVALRY_BONUS + totalNbArmies : totalNbArmies;
        }
        // TODO: Check if there's a max number of armies we can place
        std::string answer;
        int targetNbArmies;
        std::cout << "On which countries would you like to place your " << totalNbArmies << " armies?" << std::endl;
        for (int i = 0; this->countries.size(); ++i) {
            std::cout << "Put armies on " << this->countries[i]->getCountry().getName() << "? (y/n)";
            std::cin >> answer;
            if (answer == "y") {
                std::cout << "You already have " << this->countries[i]->getCountry().getNbrArmies() << "armies?" << std::endl;
                std::cout << "How many armies do you want to add? ";
                std::cin >> targetNbArmies;
                Player::assignArmies(i, targetNbArmies);
            }
        }

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

void Player::assignArmies(int countryIndex, int nbArmies)
{
    int total = nbArmies + countries[countryIndex]->getCountry().getNbrArmies();
    std::cout << "Setting number of armies on " << countries[countryIndex]->getCountry().getName() << " to " << total << std::endl;
    this->countries[countryIndex]->getCountry().setNbrArmies(total);
}