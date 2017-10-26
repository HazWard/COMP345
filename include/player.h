/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/
#pragma once
#include <list>
#include <map>
#include "../include/cards.h"
#include "../include/dice.h"
#include "../include/map.h"

class Player
{
    /*
        Player class implements the different methods to interact with the
        game as well as hold a Hand and a list of nodes the player owns.
    */
    public:
        Player();
        Player(string n);
        Player(string n, Hand* playerHand, std::list<Node*>* playerNodes);
        ~Player();
        int roll(int nbOfDice);
        void reinforce(std::map<string, Graph>* graph);
        void attack();
        void fortify(Graph& map);
        string getName();
        std::list<Node*> getNodes();
        Hand* getHand();
        void printNodes();
        void setName(string n);
        void setHand(Hand* targetHand);
        void setNodes(std::list<Node*>* targetNodes);
        void setDice(Dice* targetDice);
        void addCountry(Node* newCountry);
        void addNode(Node* newNode);
        list<string> getsContinentsOwned(map<string, Graph>* continents);
    private:
        std::string name;
        static bool attack(Player* attacker, Player* defender, Country* attackingCountry, Country* defendingCountry);
        bool controlsAllCountriesInMap(Graph& map);
        void placeArmies(int nbArmies);
        Hand* hand;
        Dice* dice;
        std::list<Node*> nodes;
        bool containsNode(const Node &node);
};
