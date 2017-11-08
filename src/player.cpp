/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Class representing a Risk Player
*/
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include "../include/player.h"

// Constants
static const int MIN_NUMBER_OF_ARMIES = 3;
static const int INFANTRY_BONUS = 1;
static const int CAVALRY_BONUS = 5;
static const int ARTILLERY_BONUS = 10;

/**
 * PLAYER CLASS
 */

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

Dice* Player::getDice()
{
    return this->dice;
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

void Player::setStrategy(Strategy *targetStrategy)
{
    this->strategy = targetStrategy;
}

/**
 * GAME PHASES USING STRATEGY OBJECT
*/

std::vector<ReinforceResponse> Player::reinforce(std::map<string, Graph> *graph)
{
    return this->strategy->reinforce(this, graph);
}

AttackResponse Player::attack(Graph &map, std::vector<Player *> &players)
{
    return this->strategy->attack(this, map, players);
}

FortifyResponse Player::fortify(Graph &map)
{
    return this->strategy->fortify(this, map);
}

void Player::removeNode(Node* n)
{
    list<Node*>::const_iterator countryIterator;
    for (countryIterator = nodes.begin(); countryIterator != nodes.end(); ++countryIterator)
    {
        if(*countryIterator == n)
        {
            nodes.erase(countryIterator++);  // alternatively, i = items.erase(i);
        }
    }
}

bool Player::containsNode(Node &node){
    std::list<Node*>::iterator nodeIterator;
    for(nodeIterator = this->nodes.begin(); nodeIterator != this->nodes.end(); nodeIterator++){
        //TODO: Uncomment this once and remove other if statement the adjacency list contains pointers not copies
//        if(*nodeIterator == &node){
//            return true;
//        }
        if((*nodeIterator)->getPointerToCountry()->getName() == node.getPointerToCountry()->getName()){
            return true;
        }
    }
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

list<string> Player::getsContinentsOwned(std::map<string, Graph>* continents) {
    list<string> continentsOwned;
    map<string, Graph>::reverse_iterator graphIterator;
    //We iterate through all the continents and add the ones that the player completely owns
    for (graphIterator = continents->rbegin(); graphIterator != continents->rend(); ++graphIterator) {
        bool continentOwned = true;
        Graph *current_continent = &(graphIterator->second);
        vector<Node> *nodesInContinent = current_continent->getVectorOfNodes();
        //we iterate through all the countries in the current continent and check if the player owns them all
        for (int i = 0; i < nodesInContinent->size(); i++) {
            bool countryOwned = false;
            list<Node *>::const_iterator countryIterator;
            //We iterate through all the countries the player owns and see if he owns the current country from the continent we are checking
            for (countryIterator = nodes.begin(); countryIterator != nodes.end(); ++countryIterator) {
                if ((*nodesInContinent)[i].getCountry() == (*countryIterator)->getCountry()) {
                    countryOwned = true;
                    break;
                }
            }
            //If the current country does not belong to the player, then the whole continent does not belong to them.
            if (!countryOwned) {
                //cout << "allo";
                continentOwned = false;
                break;
            }
        }
        if (continentOwned)
            continentsOwned.push_back(graphIterator->first);
    }
    return continentsOwned;
}

void Player::printNodes()
{
    list<Node*>::const_iterator countryIterator;
    cout << "Countries that belong to " << name << ":" << endl;
    for (countryIterator = nodes.begin(); countryIterator != nodes.end(); ++countryIterator)
    {
        Country c = (*countryIterator)->getCountry();
        cout << c.getName() << "; ";
    }
    cout << endl << endl;
}
