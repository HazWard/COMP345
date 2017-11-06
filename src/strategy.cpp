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
void AgressiveStrategy::play(Player *targetPlayer,
                             Graph &map,
                             std::vector<Player *> &listOfPlayers,
                             std::map<string, Graph> *continents)
{
    // TODO: Implement Aggressive Strategy
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