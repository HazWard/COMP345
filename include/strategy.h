
#pragma once

#include <vector>
#include "../include/player.h"
#include "../include/response.h"

class Player; // Forward declaration

/**
 * Strategy Interface to represent the different behavoirs for Player
 */
class Strategy
{
public:
    virtual std::vector<ReinforceResponse> reinforce(Player* targetPlayer, std::map<string, Graph>* graph) = 0;
    virtual AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players) = 0;
    virtual FortifyResponse fortify(Player* targetPlayer, Graph& map) = 0;
    virtual bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry) = 0;
    bool containsNode(Player* targetPlayer, Node &node);
};

/**
 * Human Player Strategy
 * - Every phase requires user interaction
 */
class HumanStrategy : Strategy
{
public:
    std::vector<ReinforceResponse> reinforce(Player* targetPlayer, std::map<string, Graph>* graph);
    AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players);
    FortifyResponse fortify(Player* targetPlayer, Graph& map);
    bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry);
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
    std::vector<ReinforceResponse> reinforce(Player* targetPlayer, std::map<string, Graph>* graph);
    AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players);
    FortifyResponse fortify(Player* targetPlayer, Graph& map);
    bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry);
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
    std::vector<ReinforceResponse> reinforce(Player* targetPlayer, std::map<string, Graph>* graph);
    AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players);
    FortifyResponse fortify(Player* targetPlayer, Graph& map);
    bool attack(Player& attacker, Player& defender, Country& attackingCountry, Country& defendingCountry);
};
