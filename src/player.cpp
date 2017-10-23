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
    /*
    Provide a group of C++ classes that implement the attack phase following the official rules of the game of Risk. In
    this phase, the player is allowed to declare a series of attacks to try to gain control of additional countries, and
    eventually control the entire map. The attack phase follows the following loop:
     The player decides if it will attack or not. If not, the attack phase is over.
     The player selects one of its countries to attack from, and one of the neighbors of this country to attack
    (i.e. the attacked country belongs to another player). The attacking country must have at least 2 armies
    on it.
     The attacker and defender players choose the number of dice to roll for their attack/defense. The attacker
    is allowed 1 to 3 dice, with the maximum number of dice being the number of armies on the attacking
    country, minus one. The defender is allowed 1 to 2 dice, with the maximum number of dice being the
    number of armies on the defending country.
     The dice are rolled for each player and sorted, then compared pair-wise. For each pair, the player with
    the lowest roll loses one army. If the pair is equal, the attacker loses an army.
     If the attacked country runs out of armies, it has been defeated. The defending country now belongs to
    the attacking player. The attacker is allowed to move a number of armies from the attacking country to the
    attacked country, in the range [1 to (number of armies on attacking country -1)].
     The player is allowed to initiate any number of attacks per turn, including 0.
    You must deliver a driver that demonstrates that 1) only valid attacks can be declared (i.e. valid attacker/attacked
    country); 2) only valid number of dice can be chosen by the attacker/defender; 3) given known dice values, that
    the right number of armies are deducted on the attacker/defender; 4) the attacker is allowed to initiate multiple
    attacks, until it declares that it does not want to attack anymore.
     */


    //TODO: Step 1, find the countries it can attack
    std::vector<Node*> canAttack;
    for(int i = 0; i < this->countries.size(); i++){
        //Getting a list of all countries adjacent to current node
        std::vector<Node*> adjacentToCurrentNode = this->countries[i]->getAdjList();
        for(int j = 0; j < adjacentToCurrentNode.size(); j++){
            if(/*adjacentToCurrentNode[j]->getCountry().getPlayer()*/){
                //TODO: Find if a country belongs to another player, if so add it to the canAttack list

            }

        }
    }
    //TODO: Step 2, declare attacks

    //TODO: Step 3, perform the attacking, possibly using some static function to perform the attacking

    //TODO: Step 4, conclude attacks and test results


}

void Player::fortify()
{
    // Perform actions to fortify
    std::cout << "Player is fortifying!" << std::endl;
}

bool Player::attack(Player *attacker, Player *defender, Country *attackingCountry, Country *defendingCountry) {
    //TODO: Write the code that will encapsulate the attacking process according to the Risk rules
    return false;
}
