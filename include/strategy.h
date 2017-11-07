
#pragma once

#include "../include/player.h"

class Player; // Forward declaration

/**
 * Strategy Interface to represent the different behavoirs for Player
 */
class Strategy
{
public:
    virtual void play(Player* targetPlayer,
                      Graph& map,
                      std::vector<Player*> &listOfPlayers,
                      std::map<string, Graph>* continents) = 0;
};

/**
 * Human Player Strategy
 * - Every phase requires user interaction
 */
class HumanStrategy : Strategy
{
public:
    void play(Player* targetPlayer,
              Graph& map,
              std::vector<Player*> &listOfPlayers,
              std::map<string, Graph>* continents);
};

/**
 * Aggressive Computer Player Strategy
 * - Reinforces its strongest country
 * - Attacks with strongest country
 * - Fortifies one country
 */
class AggressiveStrategy : Strategy
{
public:
    void play(Player* targetPlayer,
              Graph& map,
              std::vector<Player*> &listOfPlayers,
              std::map<string, Graph>* continents);
private:
    void reinforce(Player* targetPlayer, std::map<string, Graph>* graph);
    void attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players);
    void fortify(Player* targetPlayer, Graph& map);
};

/**
 * Benevolent Computer Player Strategy
 * - Reinforces its weakest countries
 * - Never attacks
 * - Fortifies weakest countries
 */
class BenevolentStrategy : Strategy
{
public:
    void play(Player* targetPlayer,
              Graph& map,
              std::vector<Player*> &listOfPlayers,
              std::map<string, Graph>* continents);
private:
    void reinforce(Player* targetPlayer, std::map<string, Graph>* graph);
    void fortify(Player* targetPlayer, Graph& map);
};
