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

Player::Player(Hand* playerHand, std::list<int>* playerCountries)
{
    if (playerCountries->size() < 1)
    {
        std::cout << "Invalid arguments for Player contructor." << std::endl;
        // TODO: Find a way to get rid of an invalid object
    }
    else
    {
        this->hand = playerHand;
        this->countries = playerCountries;

        // Create the dice rolling facility for the Player
        this->dice = new Dice();
    }
}

Player::~Player(void)
{
    delete hand;
    hand = NULL;
    countries = NULL;
}

int Player::roll(int nbOfDice)
{
    // Perform rolls
    int roll = 0;
    int value = 0;
    for (int i = 0; i < nbOfDice; ++i)
    {
        value = this->dice->numGenerator();
        std::cout << "Roll: " << value << std::endl;
        roll += value;
    }
    std::cout << "Total Roll: " << roll << std::endl;
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

int main()
{
    Hand* testHand = new Hand();
    int arr[] = {1, 2, 3, 4};
    std::list<int> testList (arr, arr + sizeof(arr) / sizeof(int) );
    Player* user = new Player(testHand, &testList);
    
    user->roll(1);
    user->roll(2);
    user->roll(3);
    
    return 0;
}
