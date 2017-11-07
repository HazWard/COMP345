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
        static const int MIN_NUMBER_OF_CARDS = 5;
        static const int INFANTRY_BONUS = 1;
        static const int CAVALRY_BONUS = 5;
        static const int ARTILLERY_BONUS = 10;

        Player();
        Player(string n);
        Player(string n, Hand* playerHand, std::list<Node*>* playerNodes);
        ~Player();
        int roll(int nbOfDice);
        Strategy *strategy;
        void setStrategy(Strategy *targetStrategy);
        void reinforce(std::map<string, Graph>* graph);
        void attack(Graph& map, std::vector<Player*> &players);
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
        bool controlsAllCountriesInMap(Graph& map);
        void placeArmies(int nbArmies);
    private:
        std::string name;
        static bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry);
        Hand* hand;
        Dice* dice;
        std::list<Node*> nodes;
        bool containsNode(Node &node);
        void removeNode(Node *n);
};