/*
    Filename: player.h
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
#include "../include/response.h"
#include "../include/strategy.h"

class Strategy; // Forward declaration
class Player
{
    /*
        Player class implements the different methods to interact with the
        game as well as hold a Hand and a list of nodes the player owns.
    */
    public:
        // Constants
        static const int MIN_NUMBER_OF_ARMIES = 3;
        static const int INFANTRY_BONUS = 1;
        static const int CAVALRY_BONUS = 5;
        static const int ARTILLERY_BONUS = 10;

        Player();
        Player(string n);
        Player(string n, Strategy* playerStrategy);
        Player(string n, Hand* playerHand, std::list<Node*>* playerNodes);
        ~Player();
        int roll(int nbOfDice);

        Strategy* getStrategy() { return strategy; };
        void setStrategy(Strategy *targetStrategy);
        std::vector<ReinforceResponse*>* reinforce(std::vector<Continent*> continents);
        AttackResponse* attack(std::vector<Player*> *players);
        FortifyResponse* fortify(Graph& map);
        string getName();
        std::list<Node*>* getNodes();
        Hand* getHand();
        void printNodes();
        void setName(string n);
        void setHand(Hand* targetHand);
        void setNodes(std::list<Node*>* targetNodes);
        void setDice(Dice* targetDice);
        Dice* getDice();
        void addNode(Node* newNode);
        vector<Continent*>* getsContinentsOwned(vector<Continent*> *continents);
        bool controlsAllCountriesInMap(Graph& map);
        void removeNode(Node *n);
    private:
        Strategy *strategy;
        std::string name;
        Hand* hand;
        Dice* dice;
        std::list<Node*> nodes;
        bool containsNode(Node &node);
};