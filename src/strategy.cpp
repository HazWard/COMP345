#include <iostream>
#include <list>
#include <algorithm>
#include <utility>
#include <set>
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

std::vector<ReinforceResponse*>* HumanStrategy::reinforce(Player *targetPlayer, std::vector<Continent*> continents)
{
    // Perform actions to reinforce
    std::cout << "== REINFORCEMENT PHASE for " << targetPlayer->getName() << " ==" << std::endl;
    unsigned long totalNbArmies = targetPlayer->getNodes().size() / Player::MIN_NUMBER_OF_ARMIES;
    std::vector<ReinforceResponse*>* responses = new std::vector<ReinforceResponse*>();
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

                        totalNbArmies -= targetNbArmies;
                        std::cout << "Adding " << targetNbArmies << " to " << currentNode->getCountry().getName() << std::endl;

                        // Check if the country has already been added to the response list
                        // if so, simply update the number of armies to add
                        bool updatedExistingResponse = false;
                        for (int i = 0; i < responses->size(); i++)
                        {
                            if (responses->at(i)->country->getPointerToCountry()->getName()
                                == currentNode->getPointerToCountry()->getName())
                            {
                                responses->at(i)->nbArmies = responses->at(i)->nbArmies + targetNbArmies;
                                updatedExistingResponse = true;
                            }
                        }
                        if(!updatedExistingResponse)
                        {
                            responses->push_back(new ReinforceResponse(targetNbArmies, currentNode));
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

AttackResponse* HumanStrategy::attack(Player *targetPlayer, Graph &map, std::vector<Player *> &players)
{
    cout << targetPlayer->getName() << ", do you wish to attack? (y/n)";
    std::string willAttack;
    cin >> willAttack;
    if(willAttack == "n"){
        return nullptr;
    }

    //Creating a map of possible attack vectors between nodes this player owns and ones that are adjacent and not owned
    std::map<Node *, Node *> canAttack = std::map<Node *, Node *>();
    std::list<Node *>::iterator nodeIterator;
    for (nodeIterator = targetPlayer->getNodes().begin(); nodeIterator != targetPlayer->getNodes().end(); nodeIterator++) {
        Node *playerOwnedNode = *nodeIterator;
        if (playerOwnedNode->getPointerToCountry()->getNbrArmies() >= 2) {
            for (auto const &adjacentNode : playerOwnedNode->getAdjList()) {
                if (!Strategy::containsNode(targetPlayer, *adjacentNode)) {
                    canAttack.insert(make_pair(playerOwnedNode, adjacentNode));
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

        //Determining who the defending player will be for this particular attack vector
        Player *defendingPlayer;
        for (int i = 0; i < players.size(); i++) {
            if (players.at(i) == targetPlayer) { //This player is our current player
                continue;
            }
            for (auto const &node : players.at(i)->getNodes()) {
                if (node->getPointerToCountry()->getName() == iterator->second->getPointerToCountry()->getName()) {
                    defendingPlayer = &(*players.at(i));
                    break;
                }
            }
        }

        std::pair<Player*, Node*> attacker = new std::pair<Player*, Node*>(targetPlayer, iterator->first);
        std::pair<Player*, Node*> defender = new std::pair<Player*, Node*>(defendingPlayer, iterator->second);
        return new AttackResponse(attacker, defender);
    }

}

FortifyResponse* HumanStrategy::fortify(Player *targetPlayer, Graph &map)
{
    cout << "========== Fortification ==========" << endl;
    std::string option;
    cout << targetPlayer->getName() << ", Would you like to fortify? (y/n)";
    cin >> option;
    if(option == "n"){
        return nullptr;
    }

    string sourceStr;
    string destinationStr;
    int armNum;
    bool validInput = false;
    Node* sourceCtr = nullptr;
    Node* destCtr = nullptr;

    cout << targetPlayer->getName() << ", here are the countries you own: " << endl;
    for(auto const &node : targetPlayer->getNodes()){
        cout << node->getPointerToCountry()->getName() << " -- Armies: " << node->getPointerToCountry()->getNbrArmies() << endl;
    }

    //this while loop asks for source and loops if not owned
    do {
        std::cout << "Please enter the source country: ";
        getline(cin, sourceStr);

        list<Node*>::const_iterator sourceCountryIterator;
        for (sourceCountryIterator = targetPlayer->getNodes().begin(); sourceCountryIterator != targetPlayer->getNodes().end(); ++sourceCountryIterator)
        {
            if (sourceStr == (*sourceCountryIterator)->getCountry().getName() && (*sourceCountryIterator)->getCountry().getNbrArmies() > 1) {
                sourceCtr = *sourceCountryIterator;
                validInput=true;
                break;
            }
        }
        if(!validInput)
            std::cout << "Invalid entry. You must own the country and it must have more than 1 armies." << std::endl;
    }   while(!validInput);

    validInput=false;

    //this while loop asks for destination and loops if not owned or if not connected to source
    do {
        cout << "Here are the valid destinations for this country." << endl;
        std::set<Node*> destinations = std::set<Node*>();
        for(auto const &node : targetPlayer->getNodes()){
            for(auto const &node2 : sourceCtr->getAdjList()){
                if(node->getPointerToCountry()->getName() == node2->getPointerToCountry()->getName()){
                    destinations.insert(node);
                }
            }
        }
        for(auto const &node : destinations){
            cout << node->getPointerToCountry()->getName() << " -- Armies: " << node->getPointerToCountry()->getNbrArmies() << endl;
        }


        std::cout << "Please enter the destination country: ";
        getline(cin, destinationStr);

        list<Node*>::const_iterator destinationCountryIterator;
        for (destinationCountryIterator = targetPlayer->getNodes().begin(); destinationCountryIterator != targetPlayer->getNodes().end(); ++destinationCountryIterator)
        {
            if (destinationStr==(*destinationCountryIterator)->getCountry().getName())
            {
                destCtr = *destinationCountryIterator;
                validInput=true;
                break;
            }
        }
        if(!validInput)
            std::cout << "Invalid destination. You entered a country that does not belong to you or is not a neighboring country of the source." << std::endl;
    }   while(!validInput);

    validInput=false;

    while(armNum >= sourceCtr->getCountry().getNbrArmies() || armNum <= 0){

        if(validInput)
            std::cout << "invalid number of armies please reenter a valid number" << endl;
        else
            std::cout << "Please enter number of armies to move" << endl;

        std::cin >> armNum;

        validInput=true;
    }

    //sourceCtr->getPointerToCountry()->setNbrArmies(sourceCtr->getPointerToCountry()->getNbrArmies() - armNum);
    //destCtr->getPointerToCountry()->setNbrArmies(destCtr->getPointerToCountry()->getNbrArmies() + armNum);
    //std::cout << armNum << " armies have been moved from "<<sourceStr<<" to "<<destinationStr << std::endl;

    validInput=false;
    return new FortifyResponse(armNum, sourceCtr,destCtr);
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
std::vector<ReinforceResponse*>* AggressiveStrategy::reinforce(Player *targetPlayer, std::vector<Continent*> continents)
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
    std::vector<ReinforceResponse*>* responses = new std::vector<ReinforceResponse*>();
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
        // std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << total << std::endl;
        // strongestCountry->getPointerToCountry()->setNbrArmies(total);
        responses->push_back(new ReinforceResponse(totalNbArmies, strongestCountry));
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
 * @param players List of players
 */
AttackResponse* AggressiveStrategy::attack(Player *targetPlayer, Graph& map, std::vector<Player*> &players)
{
    // Sort the players countries by strongest
    std::list<Node*> strongestCountries = targetPlayer->getNodes(); //Creates a copy of the list -> cplusplus.com/reference/list/list/operator=/
    strongestCountries.sort([](Node &a, Node &b) -> bool
            { return a.getPointerToCountry()->getNbrArmies() > b.getPointerToCountry()->getNbrArmies(); }); //Sorting the list with a lambda

    //Setting up some pointers for returning an attack response
    std::pair<Player*,Node*> *attacker;
    Node *defendingCountry;

    //Finding an attack vector between the strongest node and an adjacent weak node
    std::list<Node *>::iterator nodeIterator;
    for (nodeIterator = strongestCountries.begin(); nodeIterator != strongestCountries.end(); nodeIterator++) {
        Node *playerOwnedNode = *nodeIterator;
        if (playerOwnedNode->getPointerToCountry()->getNbrArmies() >= 2) {
            std::vector<Node*> adjacentEnemyNodes = {};
            for (auto const &adjacentNode : playerOwnedNode->getAdjList()) {
                if (!Strategy::containsNode(targetPlayer, *adjacentNode)) {
                    adjacentEnemyNodes.push_back(*adjacentNode);
                }
            }
            if(!adjacentEnemyNodes.empty()){
                Node *weakestNode = adjacentEnemyNodes.at(0);
                for(auto const &adjacentNode : adjacentEnemyNodes){
                    if(adjacentNode->getPointerToCountry()->getNbrArmies() < weakestNode->getPointerToCountry()->getNbrArmies()){
                        weakestNode = adjacentNode;
                    }
                }
                defendingCountry = weakestNode;
            }
        }
    }

    if(!defendingCountry){ //Checking if the pointer is NULL, ie. that no defending country was found
        return nullptr;
    }

    //Determine who the defending player is for the chosen defending country
    Player *defendingPlayer;
    for (int i = 0; i < players.size(); i++) {
        if (players.at(i)->getName() == targetPlayer->getName()) { //the player is this player
            continue;
        }
        for (auto const &node : players.at(i)->getNodes()) {
            if (node->getPointerToCountry()->getName() == defendingCountry->getPointerToCountry()->getName()) {
                defendingPlayer = &(*players.at(i));
                break;
            }
        }
    }

    std::pair<Player*, Node*> defender = new std::pair<Player*, Node*>(defendingPlayer, defendingCountry);
    return new AttackResponse(attacker, defender);
}

/**
 * Fortification phase for Aggressive Player
 * - Aggregates maximum of armies on strongest country
 * @param map Game map
 */
FortifyResponse* AggressiveStrategy::fortify(Player *targetPlayer, Graph &map)
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
    int total = secondStrongestCountry->getPointerToCountry()->getNbrArmies() - 1;
    // secondStrongestCountry->getPointerToCountry()->setNbrArmies(1);
    // std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << total<< std::endl;
    // strongestCountry->getPointerToCountry()->setNbrArmies(total);
    return new FortifyResponse(total, secondStrongestCountry, strongestCountry);
}

/**
 * Benovolent Player Strategy Implementation
 */

std::vector<ReinforceResponse*>* BenevolentStrategy::reinforce(Player *targetPlayer, std::vector<Continent *> continents)
{
    // Find weakest country
    std::list<Node*>::iterator countryIter;

    Node* weakestCountry = *targetPlayer->getNodes().begin();
    for (countryIter = targetPlayer->getNodes().begin(); countryIter != targetPlayer->getNodes().end(); ++countryIter)
    {
        if(weakestCountry->getPointerToCountry()->getNbrArmies()
           > (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            weakestCountry = *countryIter;
        }
    }

    // Reinforce the weakest country
    std::vector<ReinforceResponse*>* responses = new std::vector<ReinforceResponse*>();
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
        // std::cout << "Setting number of armies on " << weakestCountry->getCountry().getName() << " to " << total << std::endl;
        // weakestCountry->getPointerToCountry()->setNbrArmies(total);
        responses->push_back(new ReinforceResponse(totalNbArmies, weakestCountry));
    }
    else
    {
        std::cout << "Not enough armies to reinforce troops." << std::endl;
    }
    return responses;
}
AttackResponse* BenevolentStrategy::attack(Player *targetPlayer, Graph &map, std::vector<Player *> &players)
{
    return new AttackResponse(std::pair<Player*, Node*>(), std::pair<Player*, Node*>());
}
FortifyResponse* BenevolentStrategy::fortify(Player *targetPlayer, Graph &map)
{
    // Find weakest country
    std::list<Node*>::iterator countryIter;
    Node* weakestCountry = *targetPlayer->getNodes().begin();
    for (countryIter = targetPlayer->getNodes().begin(); countryIter != targetPlayer->getNodes().end(); ++countryIter)
    {
        if(weakestCountry->getPointerToCountry()->getNbrArmies()
           < (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            weakestCountry = *countryIter;
        }
    }

    // Find second weakest country
    Node* secondWeakestCountry = weakestCountry->getAdjList()[0];
    for (size_t i = 1; i < weakestCountry->getAdjList().size(); i++)
    {
        if (secondWeakestCountry->getPointerToCountry()->getNbrArmies()
            > weakestCountry->getAdjList()[i]->getPointerToCountry()->getNbrArmies())
        {
            secondWeakestCountry = weakestCountry->getAdjList()[i];
        }
    }

    // Aggregate armies to strongest country
    int total = secondWeakestCountry->getPointerToCountry()->getNbrArmies() - 1;
    // secondWeakestCountry->getPointerToCountry()->setNbrArmies(1);
    // std::cout << "Setting number of armies on " << weakestCountry->getCountry().getName() << " to " << total<< std::endl;
    // weakestCountry->getPointerToCountry()->setNbrArmies(total);
    return new FortifyResponse(total, secondWeakestCountry, weakestCountry);
}

