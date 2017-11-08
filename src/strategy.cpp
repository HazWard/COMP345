#include <iostream>
#include <list>
#include <algorithm>
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
                            std::cout << "How many armies do you want now? ";
                            std::cin >> targetNbArmies;
                        }

                        totalNbArmies -= targetNbArmies;
                        std::cout << "Adding  " << targetNbArmies << " to " << currentNode->getCountry().getName() << std::endl;
                        ReinforceResponse response = ReinforceResponse();

                        // Check if the country has already been added to the response list
                        // if so, simply update the number of armies to add
                        bool updatedExistingResponse = false;
                        for (int i = 0; i < responses.size(); i++)
                        {
                            if (responses[i].country->getPointerToCountry()->getName()
                                == currentNode->getPointerToCountry()->getName())
                            {
                                responses[i].nbArmies = responses[i].nbArmies + targetNbArmies;
                                updatedExistingResponse = true;
                            }
                        }
                        if(!updatedExistingResponse)
                        {
                            response.country = currentNode;
                            response.nbArmies = targetNbArmies;
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

        // TODO: Verify implementation of attack()
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
    cout << "========== Fortification ==========" << endl;
    std::string option;
    cout << targetPlayer->getName() << ", Would you like to fortify? (y/n)";
    cin >> option;
    if(option == "n"){
        return FortifyResponse();
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

    FortifyResponse response = FortifyResponse();
    response.nbArmies = armNum;
    response.sourceCountry = sourceCtr;
    response.destinationCountry = destCtr;
    return response;
}

bool HumanStrategy::attack(Player &attacker, Player &defender, Country &attackingCountry, Country &defendingCountry)
{
    // TODO: Implement correct attack phase
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
        // std::cout << "Setting number of armies on " << strongestCountry->getCountry().getName() << " to " << total << std::endl;
        // strongestCountry->getPointerToCountry()->setNbrArmies(total);

        ReinforceResponse response = ReinforceResponse();
        response.country = strongestCountry;
        response.nbArmies = totalNbArmies;
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
    int total = secondStrongestCountry->getPointerToCountry()->getNbrArmies() - 1;
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
        // std::cout << "Setting number of armies on " << weakestCountry->getCountry().getName() << " to " << total << std::endl;
        // weakestCountry->getPointerToCountry()->setNbrArmies(total);

        ReinforceResponse response = ReinforceResponse();
        response.country = weakestCountry;
        response.nbArmies = totalNbArmies;
        responses.push_back(response);
    }
    else
    {
        std::cout << "Not enough armies to reinforce troops." << std::endl;
    }
    return responses;
}
AttackResponse BenevolentStrategy::attack(Player *targetPlayer, Graph &map, std::vector<Player *> &players)
{
    return AttackResponse();
}
FortifyResponse BenevolentStrategy::fortify(Player *targetPlayer, Graph &map)
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

    FortifyResponse response = FortifyResponse();
    response.destinationCountry = weakestCountry;
    response.sourceCountry = secondWeakestCountry;
    response.nbArmies = total;
    return response;
}

bool BenevolentStrategy::attack(Player &attacker, Player &defender, Country &attackingCountry,
                                Country &defendingCountry)
{
    // Dummy function because BenevolentStrategy never attacks
    return false;
}
