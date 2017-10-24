/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/
#pragma once
#include <list>
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
        void reinforce();
        void attack();
        void fortify();
        string getName();
        std::list<Node*> getNodes();
        Hand* getHand();
        void printNodes();
        void setName(string n);
        void setHand(Hand* targetHand);
        void setNodes(std::list<Node*>* targetNodes);
        void setDice(Dice* targetDice);
        void addNode(Node* newNode);
        bool controlsAllCountriesInMap(Graph& map);
    private:
        string name;
        Hand* hand;
        Dice* dice;
        std::list<Node*> nodes;
};
