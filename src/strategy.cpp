#include <iostream>
#include <list>
#include "../include/strategy.h"

/**
 * STRATEGY CLASSES
 */

/**
 * Human Player Strategy Implementation
 * - Calls the original implementations
 * @param targetPlayer Player instance
 * @param map Graph of the current map
 * @param listOfPlayers List of Players
 * @param continents List of continents
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
 * - Game phases for Aggressive Player
 * @param targetPlayer Player instance
 * @param map Graph of the current map
 * @param listOfPlayers List of Players
 * @param continents List of continents
 */
void AggressiveStrategy::play(Player *targetPlayer,
                             Graph &map,
                             std::vector<Player *> &listOfPlayers,
                             std::map<string, Graph> *continents)
{
    this->reinforce(targetPlayer,continents);
    this->attack(targetPlayer, map, listOfPlayers);
    this->fortify(targetPlayer,map);
}

/**
 * Reinforcement phase for Aggressive Player
 * - Reinforces strongest country only
 * @param graph Graph of continents
 */
void AggressiveStrategy::reinforce(Player *targetPlayer, std::map<string, Graph>* graph)
{
    // Find strongest country
    std::list<Node*>::iterator countryIter;

    Node* strongestCountry = *targetPlayer->getNodes().begin();
    for (countryIter = targetPlayer->getNodes().begin(); countryIter != targetPlayer->getNodes().end(); ++countryIter)
    {
        if(strongestCountry->getPointerToCountry()->getNbrArmies()
           < (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            strongestCountry = *countryIter;
        }
    }

    // Reinforce the strongest country
    int totalNbArmies = targetPlayer->getNodes().size() / Player::MIN_NUMBER_OF_ARMIES;
    if (totalNbArmies >= Player::MIN_NUMBER_OF_ARMIES)
    {
        std::list<std::string> continentsOwned = targetPlayer->getsContinentsOwned(graph);

        for (unsigned int i = 0; i < continentsOwned.size(); i++) {
            totalNbArmies += 1; // Add 1 bonus point for each continent owned for now.
        }

        // Exchange process
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::INFANTRY)) ? Player::INFANTRY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::ARTILLERY)) ? Player::ARTILLERY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::CAVALRY)) ? Player::CAVALRY_BONUS + totalNbArmies : totalNbArmies;

        // Placing all new armies
        int total =  totalNbArmies + strongestCountry->getPointerToCountry()->getNbrArmies();
        std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << total << std::endl;
        strongestCountry->getPointerToCountry()->setNbrArmies(total);
    }
    else
    {
        std::cout << "Not enough armies to reinforce troops." << std::endl;
    }
}

/**
 * Attack phase for Aggressive Player
 * - Attacks with strongest country until it can't any more
 * @param map Game map
 * @param players List of players
 */
void AggressiveStrategy::attack(Player *targetPlayer, Graph& map, std::vector<Player*> &players)
{
    // TODO: Attack with country with the most armies

    // Find strongest country
    std::list<Node*>::iterator countryIter;

    Node* strongestCountry = *targetPlayer->getNodes().begin();
    for (countryIter = targetPlayer->getNodes().begin(); countryIter != targetPlayer->getNodes().end(); ++countryIter)
    {
        if(strongestCountry->getPointerToCountry()->getNbrArmies()
           < (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            strongestCountry = *countryIter;
        }
    }
}

/**
 * Fortification phase for Aggressive Player
 * - Aggregates maximum of armies on strongest country
 * @param map Game map
 */
void AggressiveStrategy::fortify(Player *targetPlayer, Graph &map)
{
    // TODO: Aggregate maximum troops on the country with the most armies
    // Step 1: Find the strongest country [X]
    // Step 2: Find every country with more than 1 armies [X]
    // Step 3: Move armies from countries in step 2 to the strongest
    //         while making sure 1 army is left on the 'weak' countries

    // Find strongest country
    std::list<Node*>::iterator countryIter;

    Node* strongestCountry = *targetPlayer->getNodes().begin();
    for (countryIter = targetPlayer->getNodes().begin(); countryIter != targetPlayer->getNodes().end(); ++countryIter)
    {
        if(strongestCountry->getPointerToCountry()->getNbrArmies()
           < (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            strongestCountry = *countryIter;
        }
    }

    // Find countries with more than 1 army
    std::vector<Country *> populousCountries = std::vector<Country*>();
    int totalArmiesToAdd = 0;
    for (size_t i = 0; i < strongestCountry->getAdjList().size(); i++)
    {
        if (strongestCountry->getAdjList()[i]->getPointerToCountry()->getNbrArmies() > 1)
        {
            totalArmiesToAdd += strongestCountry->getAdjList()[i]->getPointerToCountry()->getNbrArmies() - 1;
            strongestCountry->getAdjList()[i]->getPointerToCountry()->setNbrArmies(1);
        }
    }
    int newTotal = strongestCountry->getPointerToCountry()->getNbrArmies() + totalArmiesToAdd;
    std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << newTotal << std::endl;
    strongestCountry->getPointerToCountry()->setNbrArmies(newTotal);
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