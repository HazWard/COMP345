
#pragma once

#include "../include/player.h"

class Player; // Forward declaration

/**
 * Strategy Interface to represent the different behavoirs for Player
 */
class Strategy
{
public:
    virtual void reinforce(Player* targetPlayer, std::map<string, Graph>* graph) = 0;
    virtual void attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players) = 0;
    virtual void fortify(Player* targetPlayer, Graph& map) = 0;
    virtual static bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry) = 0;
};

/**
 * Human Player Strategy
 * - Every phase requires user interaction
 */
class HumanStrategy : Strategy
{
public:
    void reinforce(Player* targetPlayer, std::map<string, Graph>* graph) = 0;
    void attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players) = 0;
    void fortify(Player* targetPlayer, Graph& map) = 0;
    static bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry) = 0;
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
    void reinforce(Player* targetPlayer, std::map<string, Graph>* graph) = 0;
    void attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players) = 0;
    void fortify(Player* targetPlayer, Graph& map) = 0;
    static bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry) = 0;
private:
    bool containsNode(Player* targetPlayer, Node &node);
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
    void reinforce(Player* targetPlayer, std::map<string, Graph>* graph) = 0;
    void attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players) = 0;
    void fortify(Player* targetPlayer, Graph& map) = 0;
    static bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry) = 0;
};
