/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Class representing a Risk Player
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/player.h"

// Constructors
Player::Player() : name(""), hand(new Hand), nodes(std::list<Node*>()), dice(new Dice) { }

Player::Player(string n) : name(n), hand(new Hand), nodes(std::list<Node*>()), dice(new Dice) { }

Player::Player(string n, Hand* playerHand, std::list<Node*>* playerNodes)
{
    this->setName(n);
    this->setHand(playerHand);
    this->setNodes(playerNodes);
    // Create the dice rolling facility for the Player
    this->setDice(new Dice());
}
// Destructor
Player::~Player()
{
    delete hand;
    delete dice;
}

// Setters and Getters
void Player::setName(string n)
{
    this->name = n;
}
string Player::getName() { return name; }

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

void Player::setNodes(std::list<Node*> *targetNodes)
{
    nodes = *targetNodes;
}

void Player::addNode(Node* newNode)
{
    this->nodes.push_back(newNode);
}

std::list<Node*> Player::getNodes()
{
    return this->nodes;
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
//    1
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


bool Player::controlsAllCountriesInMap(Graph& map)
{
    for(int i = 0; i < map.getNbrCountries(); i++)
    {
        Node* n = &(*map.getVectorOfNodes())[i];
        if(std::find(nodes.begin(), nodes.end(), n) != nodes.end())
            continue;
        else
            return false;
    }
    return true;
}

void Player::printNodes()
{
    list<Node*>::const_iterator iterator;
    cout << "Countries that belong to " << name << ":" << endl;
    for (iterator = nodes.begin(); iterator != nodes.end(); ++iterator)
    {
        Country c = (*iterator)->getCountry();
        cout << c.getName() << "; ";
    }
    cout << endl << endl;
}