#include <iostream>
#include <list>
#include <algorithm>
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
    // TODO: Attack with country with the country with most armies

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

    // Find countries the Player can attack with its strongest country
    std::map<Node *, Node *> canAttack = std::map<Node *, Node *>();
    for (auto const &node : strongestCountry->getAdjList()) {
        if (!containsNode(targetPlayer, *node)) {
            Node *toAttack;
            for (int i = 0; i < map.getVectorOfNodes()->size(); i++) {
                if (map.getVectorOfNodes()->at(i).getPointerToCountry()->getName()
                    == node->getPointerToCountry()->getName()) {
                    toAttack = &map.getVectorOfNodes()->at(i);
                    break;
                }
            }
            canAttack.insert(make_pair(strongestCountry, toAttack));
        }
    }

    // Perform the attacks
    std::map<Node *, Node *>::iterator iterator;
    for (iterator = canAttack.begin(); iterator != canAttack.end(); iterator++) {

        // Find the defending player
        Player *defendingPlayer;
        for (int i = 0; i < players.size(); i++) {
            if (players.at(i)->getName() == targetPlayer->getName()) { //the player is this player
                continue;
            }
            for (auto const &node : players.at(i)->getNodes()) {
                if (node->getPointerToCountry()->getName() == iterator->second->getPointerToCountry()->getName()) {
                    defendingPlayer = &(*players.at(i));
                    break;
                }
            }
        }
        bool wonBattle = this->attack(*targetPlayer, *defendingPlayer, *(iterator->first->getPointerToCountry()),
                                      *(iterator->second->getPointerToCountry()));
        if (wonBattle) {
            std::cout << targetPlayer->getName() << ", you won!" << std::endl;

            //Add the conquered country to the winner's list and removing from the loser's list
            Node *n = (*iterator).second;
            defendingPlayer->removeNode(n);
            targetPlayer->getNodes().push_back(n);

            //Sending one army from the victorious country to the conquered country
            iterator->first->getPointerToCountry()->setNbrArmies(
                    iterator->first->getPointerToCountry()->getNbrArmies() - 1);
            iterator->second->getPointerToCountry()->setNbrArmies(1);

            std::cout << "=============================================" <<
                 "Here are your countries after the battle." << std::endl;
            for (auto const &node : targetPlayer->getNodes()) {
                std::cout << *node << std::endl;
            }
            std::cout << "=============================================" <<
                 "Here are the defenders countries after the battle." << std::endl;
            for (auto const &node : defendingPlayer->getNodes()) {
                std::cout << *node << std::endl;
            }
        } else {
            std::cout << "You lost this battle! Better luck next time." << std::endl;
        }
    }
    std::cout << "That concludes all your attacks, " << targetPlayer->getName() << "." << std::endl;
}

/**
 * Helper method to know if the player already owns the Node
 */
bool AggressiveStrategy::containsNode(Player *targetPlayer, Node &node){
    std::list<Node*>::iterator nodeIterator;
    for(nodeIterator = targetPlayer->getNodes().begin(); nodeIterator != targetPlayer->getNodes().end(); nodeIterator++){
        //TODO: Uncomment this once and remove other if statement the adjacency list contains pointers not copies
//        if(*nodeIterator == &node){
//            return true;
//        }
        if((*nodeIterator)->getPointerToCountry()->getName() == node.getPointerToCountry()->getName()){
            return true;
        }
    }
    return false;
}

/**
 * Helper method to perform attacking phase
 */
bool AggressiveStrategy::attack(Player &attacker, Player &defender, Country &attackingCountry, Country &defendingCountry) {
    // Same implementation as bool Player::attack(...)
    int rounds = 1;
    while(attackingCountry.getNbrArmies() > 2 && defendingCountry.getNbrArmies() > 0){
        cout << "Round " << rounds << "." << endl;
        int attackerDice = attackingCountry.getNbrArmies() >= 4 ? 3 : attackingCountry.getNbrArmies() - 1;
        int defenderDice = defendingCountry.getNbrArmies() >= 2 ? 2 : 1;

        //Getting vectors of dice rolls
        std::vector<int> attackerDiceRolls = attacker.getDice()->howManyDice(attackerDice);
        std::vector<int> defenderDiceRolls = defender.getDice()->howManyDice(defenderDice);

        //Sorting the dice roll vectors in descending order
        std::sort(attackerDiceRolls.begin(), attackerDiceRolls.end(), std::greater<int>());
        std::sort(defenderDiceRolls.begin(), defenderDiceRolls.end(), std::greater<int>());

        //iterating through the dice rolls, until run our of descending dice
        for(int i = 0; i < defenderDiceRolls.size(); i++){
            cout << "You rolled " << attackerDiceRolls.at(i) << " and they rolled " << defenderDiceRolls.at(i) << endl;
            if(defenderDiceRolls.at(i) >= attackerDiceRolls.at(i)){
                attackingCountry.setNbrArmies(attackingCountry.getNbrArmies() - 1);
            }
            else{
                defendingCountry.setNbrArmies(defendingCountry.getNbrArmies() - 1);
            }
            if(defendingCountry.getNbrArmies() == 0){
                return true;
            }
            else if(attackingCountry.getNbrArmies() == 1){
                return false;
            }
        }
        rounds++;
    }
    return false;
}

/**
 * Fortification phase for Aggressive Player
 * - Aggregates maximum of armies on strongest country
 * @param map Game map
 */
void AggressiveStrategy::fortify(Player *targetPlayer, Graph &map)
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

    // Find second strongest country
    Node* secondStrongestCountry = strongestCountry->getAdjList()[0];
    for (size_t i = 1; i < strongestCountry->getAdjList().size(); i++)
    {
        if (secondStrongestCountry->getPointerToCountry()->getNbrArmies()
            < strongestCountry->getAdjList()[i]->getPointerToCountry()->getNbrArmies())
        {
            secondStrongestCountry = strongestCountry->getAdjList()[i];
        }
    }

    // Aggregate armies to strongest country
    int total = strongestCountry->getPointerToCountry()->getNbrArmies() + (secondStrongestCountry->getPointerToCountry()->getNbrArmies() - 1);
    secondStrongestCountry->getPointerToCountry()->setNbrArmies(1);
    std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << total<< std::endl;
    strongestCountry->getPointerToCountry()->setNbrArmies(total);
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