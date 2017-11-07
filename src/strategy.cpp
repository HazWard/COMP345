#include "../include/strategy.h"

/**
 * STRATEGY CLASSES
 */

/**
 * Human Player Strategy Implementation
 * @param targetPlayer Player instance
 * @param map Graph of the current map
 * @param listOfPlayers List of Players
 * @param contients List of continents
 */
void HumanStrategy::play(Player *targetPlayer,
                         Graph &map,
                         std::vector<Player *> &listOfPlayers,
                         std::map <string, Graph> *continents)
{
    targetPlayer->reinforce(continents);
    targetPlayer->attack(map, listOfPlayers);
    targetPlayer->fortify(map);
}

/**
 * Aggressive Player Strategy Implementation
 * @param targetPlayer Player instance
 * @param map Graph of the current map
 * @param listOfPlayers List of Players
 * @param contients List of continents
 */
void AggressiveStrategy::play(Player *targetPlayer,
                             Graph &map,
                             std::vector<Player *> &listOfPlayers,
                             std::map<string, Graph> *continents)
{
    this->reinforce(continents);
    this->attack(map, listOfPlayers);
    this->fortify(map);
}

/**
 * Reinforcement phase for Aggressive Player
 * @param graph Graph of continents
 */
void AggressiveStrategy::reinforce(std::map<string, Graph>* graph)
{
    // TODO: Find country with the most armies and reinforce it again
}

/**
 * Attack phase for Aggressive Player
 * @param map Game map
 * @param players List of players
 */
void AggressiveStrategy::attack(Graph& map, std::vector<Player*> &players)
{
    // TODO: Attack with country with the most armies
}

/**
 * Fortification phase for Aggressive Player
 * @param map Game map
 */
void AggressiveStrategy::fortify(Graph &map)
{
    // TODO: Aggregate maximum troops on the country with the most armies
}

/**
 * Benovolent Player Strategy Implementation
 * @param targetPlayer Player instance
 * @param map Graph of the current map
 * @param listOfPlayers List of Players
 * @param contients List of continents
 */
void BenevolentStrategy::play(Player *targetPlayer,
                              Graph &map,
                              std::vector<Player *> &listOfPlayers,
                              std::map<string,Graph> *continents)
{
    // TODO: Implement Benevolent Strategy
}