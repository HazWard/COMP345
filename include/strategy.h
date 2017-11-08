
#pragma once

#include <vector>
#include "../include/player.h"
#include "../include/response.h"

class Player; // Forward declaration


/** Strategy Summary (PLEASE READ THIS!)
 * The Strategy class encapsulates the behaviors of Players. The general idea behind the strategy is that each
 * game phase requires the Player's instance to perform the actions and when each phase returns a Response object
 * contain the changes that occurred during the phase. The Game has the responsibility of interpreting the Response
 * and performing the concrete actions required on the game map. This ensures that the Game has knowledge of everything
 * that happens but also makes sure that it has complete control over the changes that occur throughout the game.
 */

/**
 * Strategy Interface to represent the different behaviors for Player
 */
class Strategy
{
public:
    virtual std::vector<ReinforceResponse> reinforce(Player* targetPlayer, std::vector<Continent*> continents) = 0;
    virtual AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players) = 0;
    virtual FortifyResponse fortify(Player* targetPlayer, Graph& map) = 0;

    bool containsNode(Player* targetPlayer, Node &node);
};

/**
 * Human Player Strategy
 * - Every phase requires user interaction
 */
class HumanStrategy : public Strategy
{
public:
    std::vector<ReinforceResponse> reinforce(Player* targetPlayer,std::vector<Continent*> continents);
    AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players);
    FortifyResponse fortify(Player* targetPlayer, Graph& map);
};

/**
 * Aggressive Computer Player Strategy
 * - Reinforces its strongest country
 * - Attacks with strongest country
 * - Fortifies one country
 */
class AggressiveStrategy : public Strategy
{
public:
    std::vector<ReinforceResponse> reinforce(Player* targetPlayer,std::vector<Continent*> continents);
    AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players);
    FortifyResponse fortify(Player* targetPlayer, Graph& map);
};

/**
 * Benevolent Computer Player Strategy
 * - Reinforces its weakest countries
 * - Never attacks
 * - Fortifies weakest countries
 */
class BenevolentStrategy : public Strategy
{
public:
    std::vector<ReinforceResponse> reinforce(Player* targetPlayer,std::vector<Continent*> continents);
    AttackResponse attack(Player* targetPlayer, Graph& map, std::vector<Player*> &players);
    FortifyResponse fortify(Player* targetPlayer, Graph& map);
};
