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
    for(nodeIterator = player->getNodes()->begin(); nodeIterator != player->getNodes()->end(); nodeIterator++){
        if(*nodeIterator == &targetNode){
          return true;
        }
    }
    return false;
}

std::vector<ReinforceResponse*>* HumanStrategy::reinforce(Player *targetPlayer, std::vector<Continent*> continents)
{
    // Perform actions to reinforce
    unsigned long totalNbArmies = targetPlayer->getNodes()->size() / Player::MIN_NUMBER_OF_ARMIES;
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
            for (countryIter = targetPlayer->getNodes()->begin(); countryIter != targetPlayer->getNodes()->end(); ++countryIter)
            {
                if (totalNbArmies >= 1)
                {
                    currentNode = (*countryIter);
                    std::cout << "You now have " << totalNbArmies << " to place." << std::endl;
                    std::cout << "=== " << currentNode->getCountry().getName() << ": ";
                    std::cout << currentNode->getCountry().getNbrArmies() << " armie(s) ===" << std::endl;
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
                        std::cout << "Adding " << targetNbArmies << " to " << currentNode->getPointerToCountry()->getName() << std::endl;
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
                        targetNbArmies = 0; // Resets the value
                    }
                }
            }
        }
    }
    return responses;
}

AttackResponse* HumanStrategy::attack(Player *targetPlayer, std::vector<Player *> &players)
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
    for (nodeIterator = targetPlayer->getNodes()->begin(); nodeIterator != targetPlayer->getNodes()->end(); nodeIterator++) {
        Node *playerOwnedNode = *nodeIterator;
        if (playerOwnedNode->getPointerToCountry()->getNbrArmies() >= 2) {
            for (auto const &adjacentNode : playerOwnedNode->getAdjList()) {
                if (!Strategy::containsNode(targetPlayer, *adjacentNode)) {
                    canAttack.insert(make_pair(playerOwnedNode, adjacentNode));
                }
            }
        }
    }

    if(!canAttack.empty()) { //Some possible attacks were found
        std::map<Node *, Node *>::iterator iterator;
        for (iterator = canAttack.begin(); iterator != canAttack.end(); iterator++) {
            //For every attack in the map, prompt the user if he wishes to perform the attack
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
            else if(answer != "y" && ((iterator != canAttack.end()) && (next(iterator) == canAttack.end()))){
                //Player didn't want to attack, and it was his last possible attack
                break;
            }

            //Determining who the defending player will be for this particular attack vector
            Player *defendingPlayer;
            for (int i = 0; i < players.size(); i++) {
                if (players.at(i) == targetPlayer) { //This player is our current player
                    continue;
                }
                for (auto const &node : *(players.at(i)->getNodes())) {
                    if (node->getPointerToCountry()->getName() == iterator->second->getPointerToCountry()->getName()) {
                        defendingPlayer = &(*players.at(i));
                        break;
                    }
                }
            }

            //return the first possible attack that the user approved
            std::pair<Player *, Node *> *attacker = new std::pair<Player *, Node *>(targetPlayer, iterator->first);
            std::pair<Player *, Node *> *defender = new std::pair<Player *, Node *>(defendingPlayer, iterator->second);
            return new AttackResponse(attacker, defender);
        }
    }
    return nullptr; //either no attacks were found or the user broke out of the loop by not selecting an attack
}
//The 2 following functions are used in the method fortify from the HumanStrategy class
//They are used to allow more flexibility when reading user input
static string tolower(string& str)
{
    for(int i = 0; i < str.size(); i++)
    {
        str[i] = (char)tolower(str[i]);
    }
    return str;
}
//Function taken from: https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string
static string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
FortifyResponse* HumanStrategy::fortify(Player *targetPlayer, Graph &map)
{
    bool fortify_can_be_done = false;
    for(auto const &node : *(targetPlayer->getNodes())) {
        bool valid_source_node = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1) {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes())) {
                for (auto const &node2 : node->getAdjList()) {
                    //If the current node has any destination countries for fortification, it is a valid node from which to fortify
                    if (node3->getCountry().getName() == node2->getCountry().getName()) {
                        valid_source_node = true;
                        break;
                    }
                }
            }
        }
        if(valid_source_node) {
            fortify_can_be_done = true;
            break;
        }
    }
    if(!fortify_can_be_done)
        return nullptr;

    std::string option;
    //return null if user does not want to fortify
    cout << targetPlayer->getName() << ", Would you like to fortify? (y/n)";
    cin >> option;
    if(option == "n"){
        return nullptr;
    }

    string sourceStr;
    string destinationStr;
    int armNum=0;
    bool validInput = false;
    Node* sourceCtr = nullptr;
    Node* destCtr = nullptr;
    std::set<Node*> destinations = std::set<Node*>();

    cout << targetPlayer->getName() << ", here are the countries you own from which fortification can be done: " << endl;
    for(auto const &node : *(targetPlayer->getNodes())) {
        bool valid_source_node = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1) {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes())) {
                for (auto const &node2 : node->getAdjList()) {
                    if (node3->getCountry().getName() == node2->getCountry().getName()) {
                        valid_source_node = true;
                        break;
                    }
                }
            }
        }
        if(valid_source_node) {
            cout << node->getPointerToCountry()->getName() << " -- Armies: "
                 << node->getPointerToCountry()->getNbrArmies() << endl;
        }
    }

    //cin.ignore(INT_MAX);
    //this while loop asks for source and loops if not owned
    do {
        std::cout << "Please enter the source country: ";
        getline(cin, sourceStr);
        sourceStr = tolower(sourceStr);

        list<Node*>::const_iterator sourceCountryIterator;
        for (sourceCountryIterator = targetPlayer->getNodes()->begin(); sourceCountryIterator != targetPlayer->getNodes()->end(); ++sourceCountryIterator)
        {
            string currentNameCountry = (*sourceCountryIterator)->getCountry().getName();
            currentNameCountry = tolower(currentNameCountry);
            if (sourceStr == currentNameCountry && (*sourceCountryIterator)->getCountry().getNbrArmies() > 1) {
                sourceCtr = *sourceCountryIterator;
                validInput = true;
                break;
            }
        }
        if(!validInput)
            std::cout << "Invalid entry. You must own the country and it must have more than 1 armies." << std::endl;
        else {
            //Creating the list of destination countries:
            for (auto const &node : *(targetPlayer->getNodes())) {
                for (auto const &node2 : sourceCtr->getAdjList()) {
                    if (node->getPointerToCountry()->getName() == node2->getPointerToCountry()->getName()) {
                        destinations.insert(node);
                    }
                }
            }
            if (destinations.empty()) {
                cout << sourceCtr->getCountry().getName()
                     << " does not have any valid destination country. Please choose a different source country."
                     << endl;
                destinations.clear();
                validInput = false;
            }
        }
    }   while(!validInput);

    validInput = false;

    //this while loop asks for destination and loops if not owned or if not connected to source
    do {
        cout << "Here are the valid destinations for this country." << endl;
        for(auto const &node : destinations){
            cout << node->getPointerToCountry()->getName() << " -- Armies: " << node->getPointerToCountry()->getNbrArmies() << endl;
        }

    //Get destination and check if valid
        std::cout << "Please enter the destination country: ";
        getline(cin, destinationStr);
        destinationStr = tolower(destinationStr);

        list<Node*>::const_iterator destinationCountryIterator;
        for (destinationCountryIterator = targetPlayer->getNodes()->begin(); destinationCountryIterator != targetPlayer->getNodes()->end(); ++destinationCountryIterator)
        {
            string currentNameCountry = (*destinationCountryIterator)->getCountry().getName();
            currentNameCountry = tolower(currentNameCountry);
            if (destinationStr == currentNameCountry)
            {
                destCtr = *destinationCountryIterator;
                validInput = true;
                break;
            }
        }
        if(!validInput)
            std::cout << "Invalid destination. You entered a country that does not belong to you or is not a neighboring country of the source." << std::endl;
    }   while(!validInput);

    validInput=false;

    //Get number of armies to move and check if valid
    while(armNum >= sourceCtr->getCountry().getNbrArmies() || armNum <= 0){

        if(validInput)
            std::cout << "invalid number of armies please reenter a valid number" << endl;
        else
            std::cout << "Please enter number of armies to move" << endl;

        std::cin >> armNum;

        validInput=true;
    }
    validInput=false;
    return new FortifyResponse(armNum,sourceCtr,destCtr);

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

    Node* strongestCountry = *targetPlayer->getNodes()->begin();
    for (countryIter = targetPlayer->getNodes()->begin(); countryIter != targetPlayer->getNodes()->end(); ++countryIter)
    {
        if(strongestCountry->getPointerToCountry()->getNbrArmies()
           < (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            strongestCountry = *countryIter;
        }
    }

    // Reinforce the strongest country
    std::vector<ReinforceResponse*>* responses = new std::vector<ReinforceResponse*>();
    int totalNbArmies = targetPlayer->getNodes()->size() / Player::MIN_NUMBER_OF_ARMIES;
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

        responses->push_back(new ReinforceResponse(totalNbArmies, strongestCountry));
    }
    return responses;
}

/**
 * Attack phase for Aggressive Player
 * - Attacks with strongest country until it can't any more
 * @param players List of players
 */
AttackResponse* AggressiveStrategy::attack(Player *targetPlayer, std::vector<Player*> &players)
{
    // Sort the players countries by strongest
    std::vector<Node*> strongestCountries = targetPlayer->sortByStrongest();

    //Setting up some pointers for returning an attack response
    std::pair<Player*,Node*> *attacker;
    Node *defendingCountry;

    //Finding an attack vector between the strongest node and an adjacent weak node
    std::vector<Node *>::iterator nodeIterator;
    for (nodeIterator = strongestCountries.begin(); nodeIterator != strongestCountries.end(); ++nodeIterator) {
        Node *playerOwnedNode = *nodeIterator;
        if (playerOwnedNode->getPointerToCountry()->getNbrArmies() >= 2) {
            std::vector<Node*> adjacentEnemyNodes = {};
            for (auto const &adjacentNode : playerOwnedNode->getAdjList()) {
                if (!Strategy::containsNode(targetPlayer, *adjacentNode)) {
                    adjacentEnemyNodes.push_back(adjacentNode);
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
        bool validDefendingPlayer = false;
        if (players.at(i)->getName() == targetPlayer->getName()) { //the player is this player
            continue;
        }
        for (auto const &node : *(players.at(i)->getNodes())) {
            if (node->getPointerToCountry()->getName() == defendingCountry->getPointerToCountry()->getName()) {
                defendingPlayer = &(*players.at(i));
                validDefendingPlayer = true;
                break;
            }
        }
        if(validDefendingPlayer)
            break;
    }

    std::pair<Player*, Node*> *defender = new std::pair<Player*, Node*>(defendingPlayer, defendingCountry);
    return new AttackResponse(attacker, defender);
}

/**
 * Fortification phase for Aggressive Player
 * - Aggregates maximum of armies on strongest country
 * @param map Game map
 */
FortifyResponse* AggressiveStrategy::fortify(Player *targetPlayer, Graph &map)
{
    bool fortify_can_be_done = false;
    for(auto const &node : *(targetPlayer->getNodes())) {
        bool valid_source_node = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1) {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes())) {
                for (auto const &node2 : node->getAdjList()) {
                    //If the current node has any destination countries for fortification, it is a valid node from which to fortify
                    if (node3->getCountry().getName() == node2->getCountry().getName()) {
                        valid_source_node = true;
                        break;
                    }
                }
            }
        }
        if(valid_source_node) {
            fortify_can_be_done = true;
            break;
        }
    }
    if(!fortify_can_be_done)
        return nullptr;

    std::set<Node*> destinations = std::set<Node*>();
    for(auto const &node : *(targetPlayer->getNodes())) {
        bool current_node_valid_source = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1) {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes())) {
                for (auto const &node2 : node->getAdjList()) {
                    if (node3->getCountry().getName() == node2->getCountry().getName()) {
                        current_node_valid_source = true;
                        break;
                    }
                }
            }
        }
        if (current_node_valid_source) {
            destinations.insert(node);
        }
    }
    //return null if fortification cannot be done (there are no matching source/destination countries for fortification)
    if(destinations.empty()) {
        return nullptr;
    }

    // Find strongest country
    std::list<Node*>::iterator countryIter;
    Node* strongestCountry = *targetPlayer->getNodes()->begin();
    for (countryIter = targetPlayer->getNodes()->begin(); countryIter != targetPlayer->getNodes()->end(); ++countryIter)
    {
        if(strongestCountry->getPointerToCountry()->getNbrArmies()
           < (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            strongestCountry = *countryIter;
        }
    }
    //If all of the player's countries contain at most one army, then we cannot fortify.
    if(strongestCountry->getPointerToCountry()->getNbrArmies() <= 1)
        return nullptr;

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
    if (total==0)
        return nullptr;
    return new FortifyResponse(total, secondStrongestCountry, strongestCountry);
}

/**
 * Benovolent Player Strategy Implementation
 */

std::vector<ReinforceResponse *> *BenevolentStrategy::reinforce(Player *targetPlayer, std::vector<Continent *> continents)
{
    // Reinforce the weakest country
    std::vector<ReinforceResponse *> *responses = new std::vector<ReinforceResponse *>();
    int totalNbArmies = targetPlayer->getNodes()->size() / Player::MIN_NUMBER_OF_ARMIES;

    if (totalNbArmies >= Player::MIN_NUMBER_OF_ARMIES) {
        // Get continent bonuses
        std::vector<Continent *> continentsOwned = targetPlayer->getsContinentsOwned(continents);
        for (unsigned int i = 0; i < continentsOwned.size(); i++) {
            totalNbArmies += continentsOwned[i]->getBonus();
        }

        // Exchange process
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::INFANTRY)) ? Player::INFANTRY_BONUS + totalNbArmies
                                                                            : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::ARTILLERY)) ? Player::ARTILLERY_BONUS + totalNbArmies
                                                                             : totalNbArmies;
        totalNbArmies = (targetPlayer->getHand()->exchange(Card::CAVALRY)) ? Player::CAVALRY_BONUS + totalNbArmies
                                                                           : totalNbArmies;

        std::list<Node *>::iterator countryIter;

        Node *weakestCountry;
        Node *secondWeakestCountry;
        weakestCountry = *targetPlayer->getNodes()->begin();
        secondWeakestCountry = *targetPlayer->getNodes()->begin();

        // Find weakest country
        for (countryIter = targetPlayer->getNodes()->begin();
             countryIter != targetPlayer->getNodes()->end(); ++countryIter) {
            if (weakestCountry->getPointerToCountry()->getNbrArmies()
                > (*countryIter)->getPointerToCountry()->getNbrArmies()) {
                weakestCountry = *countryIter;
            }
        }

        // Find second weakest country
        for (countryIter = targetPlayer->getNodes()->begin();
             countryIter != targetPlayer->getNodes()->end(); ++countryIter) {
            if ((*countryIter)->getPointerToCountry()->getName()
                != weakestCountry->getPointerToCountry()->getName()) {
                if (secondWeakestCountry->getPointerToCountry()->getNbrArmies()
                    > (*countryIter)->getPointerToCountry()->getNbrArmies()) {
                    secondWeakestCountry = *countryIter;
                }
            }
        }

        int targetNbArmies = (totalNbArmies % 2 == 0) ? totalNbArmies / 2 : (totalNbArmies + 1) / 2;
        responses->push_back(new ReinforceResponse(totalNbArmies - targetNbArmies, weakestCountry));
        responses->push_back(new ReinforceResponse(targetNbArmies, secondWeakestCountry));
    }
    return responses;
}

AttackResponse* BenevolentStrategy::attack(Player *targetPlayer, std::vector<Player *> &players)
{
    return nullptr;
}
FortifyResponse* BenevolentStrategy::fortify(Player *targetPlayer, Graph &map)
{
    bool fortify_can_be_done = false;
    for(auto const &node : *(targetPlayer->getNodes())) {
        bool valid_source_node = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1) {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes())) {
                for (auto const &node2 : node->getAdjList()) {
                    //If the current node has any destination countries for fortification, it is a valid node from which to fortify
                    if (node3->getCountry().getName() == node2->getCountry().getName()) {
                        valid_source_node = true;
                        break;
                    }
                }
            }
        }
        if(valid_source_node) {
            fortify_can_be_done = true;
            break;
        }
    }
    if(!fortify_can_be_done)
        return nullptr;

    std::set<Node*> destinations = std::set<Node*>();
    for(auto const &node : *(targetPlayer->getNodes())) {
        bool current_node_valid_source = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1) {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes())) {
                for (auto const &node2 : node->getAdjList()) {
                    if (node3->getCountry().getName() == node2->getCountry().getName()) {
                        current_node_valid_source = true;
                        break;
                    }
                }
            }
        }
        if (current_node_valid_source) {
            destinations.insert(node);
        }
    }
    //return null if fortification cannot be done (there are no matching source/destination countries for fortification)
    if(destinations.empty()) {
        return nullptr;
    }
    // Find weakest country
    std::list<Node*>::iterator countryIter;
    Node* weakestCountry = *targetPlayer->getNodes()->begin();
    for (countryIter = targetPlayer->getNodes()->begin(); countryIter != targetPlayer->getNodes()->end(); ++countryIter)
    {
        if(weakestCountry->getPointerToCountry()->getNbrArmies()
           < (*countryIter)->getPointerToCountry()->getNbrArmies())
        {
            weakestCountry = *countryIter;
        }
    }

    // Find the strongest adjacent country to the weakest country
    Node* strongestAdjacentCountry = weakestCountry->getAdjList()[0];
    for (size_t i = 1; i < weakestCountry->getAdjList().size(); i++)
    {
        if (strongestAdjacentCountry->getPointerToCountry()->getNbrArmies()
            > weakestCountry->getAdjList()[i]->getPointerToCountry()->getNbrArmies())
        {
            strongestAdjacentCountry = weakestCountry->getAdjList()[i];
        }
    }

    //If the adjacent countries to the weakest country contain at most one army, then we cannot fortify.
    if(strongestAdjacentCountry->getPointerToCountry()->getNbrArmies() <= 1)
        return nullptr;

    // Aggregate armies to strongest country
    int total = strongestAdjacentCountry->getPointerToCountry()->getNbrArmies() - 1;
    if (total == 0)
        return nullptr;
    return new FortifyResponse(total, strongestAdjacentCountry, weakestCountry);
}

/**
 * Random Player Strategy Implementation
 */

/**
 * Reinforcement phase for Random Player
 * - Reinforces random country
 * @param graph Graph of continents
 */
std::vector<ReinforceResponse*>* RandomStrategy::reinforce(Player *targetPlayer, std::vector<Continent*> continents)
{
    // Random generator
    std::random_device rd;
    std::mt19937 mt(rd());

    // Perform actions to reinforce
    unsigned long totalNbArmies = targetPlayer->getNodes()->size() / Player::MIN_NUMBER_OF_ARMIES;
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
            for (countryIter = targetPlayer->getNodes()->begin(); countryIter != targetPlayer->getNodes()->end(); ++countryIter)
            {
                std::uniform_int_distribution<int> dist(0, totalNbArmies);
                if (totalNbArmies >= 1)
                {
                    currentNode = (*countryIter);
                    while(targetNbArmies <= 0 || targetNbArmies > totalNbArmies)
                    {
                        targetNbArmies = dist(mt);
                    }
                    totalNbArmies -= targetNbArmies;
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
                    targetNbArmies = 0; // Resets the value
                }
            }
        }
    }
    return responses;
}

/**
 * Attack phase for Random Player
 * - Selects a random country to attack
 * @param targetPlayer Player executing the phase
 * @param players List of players
 * @return
 */
AttackResponse* RandomStrategy::attack(Player *targetPlayer, std::vector<Player *> &players)
{
    std::random_device rd;
    std::mt19937 mt(rd());

    //Creating a map of possible attack vectors between nodes this player owns and ones that are adjacent and not owned
    std::map<Node *, Node *> canAttack = std::map<Node *, Node *>();
    std::list<Node *>::iterator nodeIterator;
    for (nodeIterator = targetPlayer->getNodes()->begin(); nodeIterator != targetPlayer->getNodes()->end(); nodeIterator++) {
        Node *playerOwnedNode = *nodeIterator;
        if (playerOwnedNode->getPointerToCountry()->getNbrArmies() >= 2) {
            for (auto const &adjacentNode : playerOwnedNode->getAdjList()) {
                if (!Strategy::containsNode(targetPlayer, *adjacentNode)) {
                    canAttack.insert(make_pair(playerOwnedNode, adjacentNode));
                }
            }
        }
    }

    if(!canAttack.empty()) { //Some possible attacks were found
        std::uniform_int_distribution<int> dist(0, canAttack.size()-1);
        int chosenCountryInd = dist(mt);
        int counter = 0;
        std::map<Node *, Node *>::iterator iterator;
        for (iterator = canAttack.begin(); iterator != canAttack.end(); iterator++) {

            if (counter == chosenCountryInd)
            {
                //Determining who the defending player will be for this particular attack vector
                Player *defendingPlayer;
                for (int i = 0; i < players.size(); i++) {
                    if (players.at(i) == targetPlayer) { //This player is our current player
                        continue;
                    }
                    for (auto const &node : *(players.at(i)->getNodes())) {
                        if (node->getPointerToCountry()->getName() == iterator->second->getPointerToCountry()->getName()) {
                            defendingPlayer = &(*players.at(i));
                            break;
                        }
                    }
                }
                //return the first possible attack that the user approved
                std::pair<Player *, Node *> *attacker = new std::pair<Player *, Node *>(targetPlayer, iterator->first);
                std::pair<Player *, Node *> *defender = new std::pair<Player *, Node *>(defendingPlayer, iterator->second);
                return new AttackResponse(attacker, defender);
            }
            ++counter;
        }
    }
    return nullptr; //no attacks were found
}

/**
 * Fortification phase for Random Player
 * - Fortifies a random country
 * @param map Game map
 */
FortifyResponse* RandomStrategy::fortify(Player *targetPlayer, Graph &map)
{
    // Random generator
    std::random_device rd;
    std::mt19937 mt(rd());

    bool fortify_can_be_done = false;
    for(auto const &node : *(targetPlayer->getNodes()))
    {
        bool valid_source_node = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1)
        {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes()))
            {
                for (auto const &node2 : node->getAdjList()) {
                    //If the current node has any destination countries for fortification, it is a valid node from which to fortify
                    if (node3->getCountry().getName() == node2->getCountry().getName())
                    {
                        valid_source_node = true;
                        break;
                    }
                }
            }
        }
        if(valid_source_node)
        {
            fortify_can_be_done = true;
            break;
        }
    }
    if(!fortify_can_be_done)
    {
        return nullptr;
    }

    string sourceStr;
    string destinationStr;
    int armNum=0;
    bool validInput = false;
    Node* sourceCtr = nullptr;
    Node* destCtr = nullptr;
    std::set<Node*> destinations = std::set<Node*>();

    for(auto const &node : *(targetPlayer->getNodes()))
    {
        bool valid_source_node = false;
        if (node->getPointerToCountry()->getNbrArmies() > 1)
        {
            //Creating the list of destination countries:
            for (auto const &node3 : *(targetPlayer->getNodes()))
            {
                for (auto const &node2 : node->getAdjList())
                {
                    if (node3->getCountry().getName() == node2->getCountry().getName())
                    {
                        valid_source_node = true;
                        break;
                    }
                }
            }
        }
    }

    //cin.ignore(INT_MAX);
    //this while loop asks for source and loops if not owned
    do
    {
        std::uniform_int_distribution<int> sourceCountryRNG(0, targetPlayer->getNodes()->size() - 1);

        int counter = 0;
        int sourceCountryIndex = sourceCountryRNG(mt);

        list<Node*>::const_iterator sourceCountryIterator;
        for (sourceCountryIterator = targetPlayer->getNodes()->begin(); sourceCountryIterator != targetPlayer->getNodes()->end(); ++sourceCountryIterator)
        {
            if (counter == sourceCountryIndex)
            {
                sourceCtr = *sourceCountryIterator;
                validInput = true;
                break;
            }
            ++counter;
        }
        if(validInput)
        {
            //Creating the list of destination countries:
            for (auto const &node : *(targetPlayer->getNodes()))
            {
                for (auto const &node2 : sourceCtr->getAdjList())
                {
                    if (node->getPointerToCountry()->getName() == node2->getPointerToCountry()->getName())
                    {
                        destinations.insert(node);
                    }
                }
            }
            if (destinations.empty())
            {
                destinations.clear();
                validInput = false;
            }
        }
    }   while(!validInput);

    validInput = false;

    //this while loop asks for destination and loops if not owned or if not connected to source
    do
    {
        std::uniform_int_distribution<int> destinationCountryRNG(0, destinations.size() - 1);
        int counter = 0;
        int destinationCountryIndex = destinationCountryRNG(mt);

        //Get destination and check if valid

        list<Node*>::const_iterator destinationCountryIterator;
        for (destinationCountryIterator = targetPlayer->getNodes()->begin(); destinationCountryIterator != targetPlayer->getNodes()->end(); ++destinationCountryIterator)
        {
            if (counter == destinationCountryIndex)
            {
                destCtr = *destinationCountryIterator;
                validInput = true;
                break;
            }
        }
    }   while(!validInput);

    validInput=false;


    //Get number of armies to move and check if valid
    std::uniform_int_distribution<int> randomArmies(0, sourceCtr->getCountry().getNbrArmies() - 1);
    while(armNum >= sourceCtr->getCountry().getNbrArmies() || armNum <= 0)
    {
        armNum = randomArmies(mt);
        validInput=true;
    }
    validInput=false;
    return new FortifyResponse(armNum,sourceCtr,destCtr);
}

void Strategy::printStrat() {
    cout << "I am an abstract strategy." << endl;
}
void HumanStrategy::printStrat() {
    cout << "I am a human strategy." << endl;
}
void AggressiveStrategy::printStrat() {
    cout << "I am an aggressive strategy." << endl;
}
void BenevolentStrategy::printStrat() {
    cout << "I am a benevolent strategy." << endl;
}
void RandomStrategy::printStrat() {
    cout << "I am a random strategy." << endl;
}
void CheaterStrategy::printStrat() {
    cout << "I am a cheater strategy." << endl;
}