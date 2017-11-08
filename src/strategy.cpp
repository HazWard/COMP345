#include <iostream>
#include <list>
#include <algorithm>
#include "../include/strategy.h"

/**
 * STRATEGY CLASSES
 */

/**
 * Checks if a Player owns the provided Node
 * @param player Player
 * @param targetNode Node to check
 * @return
 */
bool Strategy::containsNode(Player *player, Node &targetNode)
{
    std::list<Node*>::iterator nodeIterator;
    for(nodeIterator = player->getNodes().begin(); nodeIterator != player->getNodes().end(); nodeIterator++){
        //TODO: Uncomment this once and remove other if statement the adjacency list contains pointers not copies
        //if(*nodeIterator == &node){
        //  return true;
        //}
        if((*nodeIterator)->getPointerToCountry()->getName() == targetNode.getPointerToCountry()->getName()){
            return true;
        }
    }
    return false;
}

std::vector<ReinforceResponse> HumanStrategy::reinforce(Player *targetPlayer, std::vector<Continent*> continents)
{
    // Perform actions to reinforce
    std::cout << "== REINFORCEMENT PHASE for " << targetPlayer->getName() << " ==" << std::endl;
    unsigned long totalNbArmies = targetPlayer->getNodes().size() / Player::MIN_NUMBER_OF_ARMIES;
    std::vector<ReinforceResponse> responses = std::vector<ReinforceResponse>();
    if (totalNbArmies >= Player::MIN_NUMBER_OF_ARMIES)
    {
        std::vector<Continent*> continentsOwned =  targetPlayer->getsContinentsOwned(continents);

        for (unsigned int i = 0; i < continentsOwned.size(); i++) {
            totalNbArmies += continentsOwned[i]->getBonus();
        }

        // Exchange process
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::INFANTRY)) ? Player::INFANTRY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::ARTILLERY)) ? Player::ARTILLERY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::CAVALRY)) ? Player::CAVALRY_BONUS + totalNbArmies : totalNbArmies;

        // Army placement
        std::string answer;
        int targetNbArmies = 0;
        std::list<Node*>::iterator countryIter;
        Node* currentNode;
        while (totalNbArmies > 0)
        {
            for (countryIter = targetPlayer->getNodes().begin(); countryIter != targetPlayer->getNodes().end(); ++countryIter)
            {
                if (totalNbArmies > 0)
                {
                    currentNode = *countryIter;
                    std::cout << "You now have " << totalNbArmies << " to place." << std::endl;
                    std::cout << "=== " << currentNode->getCountry().getName() << ": ";
                    std::cout << currentNode->getPointerToCountry()->getNbrArmies() << " armie(s) ===" << std::endl;
                    std::cout << "Do you want to add armies? (y/n) ";
                    std::cin >> answer;
                    if (answer == "y") {
                        std::cout << "You already have " << currentNode->getPointerToCountry()->getNbrArmies() << " armie(s) ?" << std::endl;
                        while(targetNbArmies <= 0 || targetNbArmies > totalNbArmies)
                        {
                            std::cout << "How many armies do you want to add? ";
                            std::cin >> targetNbArmies;
                        }
                        int total = targetNbArmies + currentNode->getCountry().getNbrArmies();
                        totalNbArmies -= targetNbArmies;
                        std::cout << "Setting number of armies on " << currentNode->getCountry().getName() << " to " << total << std::endl;
                        ReinforceResponse response = ReinforceResponse();

                        bool updatedExistingResponse = false;
                        for (int i = 0; i < responses.size(); i++)
                        {
                            if (responses[i].country->getPointerToCountry()->getName()
                                == currentNode->getPointerToCountry()->getName())
                            {
                                responses[i].nbArmies = total;
                                updatedExistingResponse = true;
                            }
                        }

                        if(!updatedExistingResponse)
                        {
                            response.country = currentNode;
                            response.nbArmies = total;
                            responses.push_back(response);
                        }
                    }
                }
            }
            currentNode = nullptr;
        }
    }
    else
    {
        std::cout << "Not enough armies to reinforce troops." << std::endl;
    }
    return responses;
}

AttackResponse HumanStrategy::attack(Player *targetPlayer, Graph &map, std::vector<Player *> &players)
{
    cout << targetPlayer->getName() << ", do you wish to attack this turn? (y/n)";
    std::string willAttack;
    cin >> willAttack;
    if(willAttack == "n"){
        return AttackResponse();
    }

    // TODO: Reevaluate the countries you can attack after every attack

    std::map<Node *, Node *> canAttack = std::map<Node *, Node *>();
    std::list<Node *>::iterator nodeIterator;
    for (nodeIterator = targetPlayer->getNodes().begin(); nodeIterator != targetPlayer->getNodes().end(); nodeIterator++) {
        Node *currentNode = *nodeIterator;
        if (currentNode->getPointerToCountry()->getNbrArmies() >= 2) {
            for (auto const &node : currentNode->getAdjList()) {
                if (!Strategy::containsNode(targetPlayer, *node)) {
                    Node *toAttack;
                    for (int i = 0; i < map.getVectorOfNodes()->size(); i++) {
                        if ((map.getVectorOfNodes()->at(i))->getPointerToCountry()->getName()
                            == node->getPointerToCountry()->getName()) {
                            toAttack = map.getVectorOfNodes()->at(i);
                            break;
                        }
                    }
                    canAttack.insert(make_pair(currentNode, toAttack));
                }
            }
        }
    }

    std::map<Node *, Node *>::iterator iterator;
    for (iterator = canAttack.begin(); iterator != canAttack.end(); iterator++) {
        cout << targetPlayer->getName() << ", you can attack " << iterator->second->getPointerToCountry()->getName()
             << " from your country " << iterator->first->getPointerToCountry()->getName() << "." << endl;
        cout << "You have " << iterator->first->getPointerToCountry()->getNbrArmies() << " armies and they have " <<
             iterator->second->getPointerToCountry()->getNbrArmies() << " armies." << endl;
        cout << "DO YOU WISH TO ATTACK? (y/n)";
        std::string answer;
        cin >> answer;
        if (answer != "y") {
            continue;
        }
        Player *defendingPlayer;
        //find who the other node belongs to
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
            cout << targetPlayer->getName() << ", you won!" << endl;

            //Add the conquered country to the winner's list and removing from the loser's list
            Node *n = (*iterator).second;
            defendingPlayer->removeNode(n);
            targetPlayer->getNodes().push_back(n);

            //Sending one army from the victorious country to the conquered country
            iterator->first->getPointerToCountry()->setNbrArmies(
                    iterator->first->getPointerToCountry()->getNbrArmies() - 1);
            iterator->second->getPointerToCountry()->setNbrArmies(1);

            cout << "=============================================" <<
                 "Here are your countries after the battle." << endl;
            for (auto const &node : targetPlayer->getNodes()) {
                cout << *node << endl;
            }
            cout << "=============================================" <<
                 "Here are the defenders countries after the battle." << endl;
            for (auto const &node : defendingPlayer->getNodes()) {
                cout << *node << endl;
            }
        } else {
            cout << "You lost this battle! Better luck next time." << endl;
        }
    }
    cout << "That concludes all your attacks, " << targetPlayer->getName() << "." << endl;
}

FortifyResponse HumanStrategy::fortify(Player *targetPlayer, Graph &map)
{
    targetPlayer->fortify(map);
}

bool HumanStrategy::attack(Player &attacker, Player &defender, Country &attackingCountry, Country &defendingCountry)
{
    return false;
}

/**
 * Aggressive Player Strategy Implementation
 * - Game phases for Aggressive Player
 */

/**
 * Reinforcement phase for Aggressive Player
 * - Reinforces strongest country only
 * @param graph Graph of continents
 */
std::vector<ReinforceResponse> AggressiveStrategy::reinforce(Player *targetPlayer, std::vector<Continent*> continents)
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
    std::vector<ReinforceResponse> responses = std::vector<ReinforceResponse>();
    int totalNbArmies = targetPlayer->getNodes().size() / Player::MIN_NUMBER_OF_ARMIES;
    if (totalNbArmies >= Player::MIN_NUMBER_OF_ARMIES)
    {
        // Get continent bonuses
        std::vector<Continent*> continentsOwned = targetPlayer->getsContinentsOwned(continents);
        for (unsigned int i = 0; i < continentsOwned.size(); i++) {
            totalNbArmies += continentsOwned[i]->getBonus();
        }

        // Exchange process
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::INFANTRY)) ? Player::INFANTRY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::ARTILLERY)) ? Player::ARTILLERY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::CAVALRY)) ? Player::CAVALRY_BONUS + totalNbArmies : totalNbArmies;

        // Placing all new armies

        int total =  totalNbArmies + strongestCountry->getPointerToCountry()->getNbrArmies();
        std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << total << std::endl;
        strongestCountry->getPointerToCountry()->setNbrArmies(total);

        ReinforceResponse response = ReinforceResponse();
        response.country = strongestCountry;
        response.nbArmies = total;
        responses.push_back(response);
    }
    else
    {
        std::cout << "Not enough armies to reinforce troops." << std::endl;
    }
    return responses;
}

/**
 * Attack phase for Aggressive Player
 * - Attacks with strongest country until it can't any more
 * @param map Game map
 * @param players List of players
 */
AttackResponse AggressiveStrategy::attack(Player *targetPlayer, Graph& map, std::vector<Player*> &players)
{
    // TODO: Attack with country with the country with most armies
    // TODO: Order canAttack list to make sure the weakest opponent is always attacked

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
                if ((map.getVectorOfNodes()->at(i))->getPointerToCountry()->getName()
                    == node->getPointerToCountry()->getName()) {
                    toAttack = map.getVectorOfNodes()->at(i);
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
FortifyResponse AggressiveStrategy::fortify(Player *targetPlayer, Graph &map)
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
    // secondStrongestCountry->getPointerToCountry()->setNbrArmies(1);
    // std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << total<< std::endl;
    // strongestCountry->getPointerToCountry()->setNbrArmies(total);

    FortifyResponse response = FortifyResponse();
    response.destinationCountry = strongestCountry;
    response.sourceCountry = secondStrongestCountry;
    response.nbArmies = total;
    return response;
}

/**
 * Benovolent Player Strategy Implementation
 */

std::vector<ReinforceResponse> BenevolentStrategy::reinforce(Player *targetPlayer, std::vector<Continent *> continents)
{
    // TODO: Implement method
    return std::vector<ReinforceResponse>();
}
AttackResponse BenevolentStrategy::attack(Player *targetPlayer, Graph &map, std::vector<Player *> &players)
{
    // TODO: Implement method
    return AttackResponse();
}
FortifyResponse BenevolentStrategy::fortify(Player *targetPlayer, Graph &map)
{
    // TODO: Implement method
    return FortifyResponse();
}

bool BenevolentStrategy::attack(Player &attacker, Player &defender, Country &attackingCountry,
                                Country &defendingCountry)
{
    return false;
}
