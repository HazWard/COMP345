/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Class representing a Risk Player
*/
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include "../include/player.h"

// Constants
static const int MIN_NUMBER_OF_ARMIES = 3;
static const int INFANTRY_BONUS = 1;
static const int CAVALRY_BONUS = 5;
static const int ARTILLERY_BONUS = 10;

/**
 * PLAYER CLASS
 */

// Constructors
Player::Player() : name(""), hand(new Hand), nodes(std::list<Node*>()), dice(new Dice) { }
Player::Player(string n) : name(n), hand(new Hand), nodes(std::list<Node*>()), dice(new Dice) { }
Player::Player(string n, Hand* playerHand, std::list<Node*>* playerNodes)
{
    this->setName(n);
    this->setHand(playerHand);
    this->setNodes(playerNodes);
    // Create the dice rolling facility for the Player
    this->setDice(new Dice());
}
// Destructor
Player::~Player()
{
    delete hand;
    delete dice;
}

// Setters and Getters
void Player::setName(string n)
{
    this->name = n;
}
string Player::getName() { return name; }

void Player::setHand(Hand *targetHand)
{
    this->hand = targetHand;
}

void Player::setDice(Dice* targetDice)
{
    this->dice = targetDice;
}

Hand* Player::getHand()
{
    return this->hand;
}

void Player::setNodes(std::list<Node*> *targetNodes)
{
    nodes = *targetNodes;
}

void Player::addNode(Node* newNode)
{
    this->nodes.push_back(newNode);
}

std::list<Node*> Player::getNodes()
{
    return this->nodes;
}


int Player::roll(int nbOfDice)
{
    // Perform rolls
    int roll = 0;
    for (int i = 0; i < nbOfDice; ++i)
    {
        roll += this->dice->numGenerator();
    }
    return roll;
}

void Player::setStrategy(Strategy *targetStrategy)
{
    this->strategy = targetStrategy;
}

void Player::reinforce(std::map<string, Graph>* map)
{
    // Perform actions to reinforce
    std::cout << "== REINFORCEMENT PHASE for " << this->name << " =="<< std::endl;
    unsigned long totalNbArmies = this->nodes.size() / MIN_NUMBER_OF_ARMIES;
    if (totalNbArmies >= MIN_NUMBER_OF_ARMIES)
    {
        std::list<std::string> continentsOwned =  getsContinentsOwned(map);

        for (unsigned int i = 0; i < continentsOwned.size(); i++) {
            totalNbArmies += 1; // Add 1 bonus point for each continent owned for now.
        }

        // Exchange process
        totalNbArmies = (this->hand->exchange(Card::INFANTRY)) ? INFANTRY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (this->hand->exchange(Card::ARTILLERY)) ? ARTILLERY_BONUS + totalNbArmies : totalNbArmies;
        totalNbArmies = (this->hand->exchange(Card::CAVALRY)) ? CAVALRY_BONUS + totalNbArmies : totalNbArmies;

        // Recursive call in the case that not all armies are placed
        this->placeArmies(totalNbArmies);
    }
    else
    {
        std::cout << "Not enough armies to reinforce troops." << std::endl;
    }
}

/**
 * Recursive method to place armies on countries.
 *
 * If the place places less than the expected number of armies
 * we go through the list of countries again to place more
 * @param nbArmies to place on countries
 */
void Player::placeArmies(int nbArmies)
{
    if (nbArmies == 0)
    {
        return;
    }
    std::string answer;
    int targetNbArmies = 0;
    std::list<Node*>::iterator countryIter;

    Node* currentNode;
    for (countryIter = nodes.begin(); countryIter != nodes.end(); ++countryIter)
    {
        if (nbArmies > 0)
        {
            currentNode = *countryIter;
            std::cout << "You now have " << nbArmies << " to place." << std::endl;
            std::cout << "=== " << currentNode->getCountry().getName() << ": ";
            std::cout << currentNode->getPointerToCountry()->getNbrArmies() << " armie(s) ===" << std::endl;
            std::cout << "Do you want to add armies? (y/n) ";
            std::cin >> answer;
            if (answer == "y") {
                std::cout << "You already have " << currentNode->getPointerToCountry()->getNbrArmies() << " armie(s) ?" << std::endl;
                while(targetNbArmies <= 0 || targetNbArmies > nbArmies)
                {
                    std::cout << "How many armies do you want to add? ";
                    std::cin >> targetNbArmies;
                }
                int total = targetNbArmies + currentNode->getCountry().getNbrArmies();
                nbArmies -= targetNbArmies;
                std::cout << "Setting number of armies on " << currentNode->getCountry().getName() << " to " << total << std::endl;
                currentNode->getPointerToCountry()->setNbrArmies(total);
            }
        }
    }
    currentNode = nullptr;
    placeArmies(nbArmies);
}

void Player::attack(Graph& map, std::vector<Player*> &players)
{
    cout << this->getName() << ", do you wish to attack this turn? (y/n)";
    std::string willAttack;
    cin >> willAttack;
    if(willAttack == "n"){
        return;
    }

    //TODO: Reevaluate the countries you can attack after every attack

    std::map<Node *, Node *> canAttack = std::map<Node *, Node *>();
    std::list<Node *>::iterator nodeIterator;
    for (nodeIterator = this->nodes.begin(); nodeIterator != this->nodes.end(); nodeIterator++) {
        Node *currentNode = *nodeIterator;
        if (currentNode->getPointerToCountry()->getNbrArmies() >= 2) {
            for (auto const &node : currentNode->getAdjList()) {
                if (!containsNode(*node)) {
                    Node *toAttack;
                    for (int i = 0; i < map.getVectorOfNodes()->size(); i++) {
                        if (map.getVectorOfNodes()->at(i).getPointerToCountry()->getName()
                            == node->getPointerToCountry()->getName()) {
                            toAttack = &map.getVectorOfNodes()->at(i);
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
        cout << this->getName() << ", you can attack " << iterator->second->getPointerToCountry()->getName()
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
            if (players.at(i)->getName() == this->getName()) { //the player is this player
                continue;
            }
            for (auto const &node : players.at(i)->getNodes()) {
                if (node->getPointerToCountry()->getName() == iterator->second->getPointerToCountry()->getName()) {
                    defendingPlayer = &(*players.at(i));
                    break;
                }
            }
        }
        bool wonBattle = this->attack(*this, *defendingPlayer, *(iterator->first->getPointerToCountry()),
                                      *(iterator->second->getPointerToCountry()));
        if (wonBattle) {
            cout << this->getName() << ", you won!" << endl;

            //Add the conquered country to the winner's list and removing from the loser's list
            Node *n = (*iterator).second;
            defendingPlayer->removeNode(n);
            this->nodes.push_back(n);

            //Sending one army from the victorious country to the conquered country
            iterator->first->getPointerToCountry()->setNbrArmies(
                    iterator->first->getPointerToCountry()->getNbrArmies() - 1);
            iterator->second->getPointerToCountry()->setNbrArmies(1);

            cout << "=============================================" <<
                "Here are your countries after the battle." << endl;
            for (auto const &node : this->getNodes()) {
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
    cout << "That concludes all your attacks, " << this->getName() << "." << endl;
}

bool Player::attack(Player &attacker, Player &defender, Country &attackingCountry, Country &defendingCountry) {
    //TODO: Write the code that will encapsulate the attacking process according to the Risk rules
    /*
     The attacker and defender players choose the number of dice to roll for their attack/defense. The attacker
    is allowed 1 to 3 dice, with the maximum number of dice being the number of armies on the attacking
    country, minus one. The defender is allowed 1 to 2 dice, with the maximum number of dice being the
    number of armies on the defending country.
     The dice are rolled for each player and sorted, then compared pair-wise. For each pair, the player with
    the lowest roll loses one army. If the pair is equal, the attacker loses an army.
     If the attacked country runs out of armies, it has been defeated. The defending country now belongs to
    the attacking player. The attacker is allowed to move a number of armies from the attacking country to the
    attacked country, in the range [1 to (number of armies on attacking country -1)].
     */
    int rounds = 1;
    while(attackingCountry.getNbrArmies() > 2 && defendingCountry.getNbrArmies() > 0){
        cout << "Round " << rounds << "." << endl;
        int attackerDice = attackingCountry.getNbrArmies() >= 4 ? 3 : attackingCountry.getNbrArmies() - 1;
        int defenderDice = defendingCountry.getNbrArmies() >= 2 ? 2 : 1;

        //Getting vectors of dice rolls
        std::vector<int> attackerDiceRolls = attacker.dice->howManyDice(attackerDice);
        std::vector<int> defenderDiceRolls = defender.dice->howManyDice(defenderDice);

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

void Player::removeNode(Node* n)
{
    list<Node*>::const_iterator countryIterator;
    for (countryIterator = nodes.begin(); countryIterator != nodes.end(); ++countryIterator)
    {
        if(*countryIterator == n)
        {
            nodes.erase(countryIterator++);  // alternatively, i = items.erase(i);
        }
    }
}

bool Player::containsNode(Node &node){
    std::list<Node*>::iterator nodeIterator;
    for(nodeIterator = this->nodes.begin(); nodeIterator != this->nodes.end(); nodeIterator++){
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

void Player::fortify(Graph& map)
{
    cout << "========== Fortification ==========" << endl;
    std::string option;
    cout << this->getName() << ", Would you like to fortify? (y/n)";
    cin >> option;
    if(option == "n"){
        return;
    }

    string sourceStr;
    string destinationStr;
    int armNum;
    bool validInput = false;
    Node* sourceCtr = nullptr;
    Node* destCtr = nullptr;

    cout << this->getName() << ", here are the countries you own: " << endl;
    for(auto const &node : this->nodes){
        cout << node->getPointerToCountry()->getName() << " -- Armies: " << node->getPointerToCountry()->getNbrArmies() << endl;
    }

    //this while loop asks for source and loops if not owned
    do {
        std::cout << "Please enter the source country: ";
        getline(cin, sourceStr);

        list<Node*>::const_iterator sourceCountryIterator;
        for (sourceCountryIterator = nodes.begin(); sourceCountryIterator != nodes.end(); ++sourceCountryIterator)
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
        for(auto const &node : this->nodes){
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
        for (destinationCountryIterator = nodes.begin(); destinationCountryIterator != nodes.end(); ++destinationCountryIterator)
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
    sourceCtr->getPointerToCountry()->setNbrArmies(sourceCtr->getPointerToCountry()->getNbrArmies() - armNum);
    destCtr->getPointerToCountry()->setNbrArmies(destCtr->getPointerToCountry()->getNbrArmies() + armNum);
    std::cout << armNum << " armies have been moved from "<<sourceStr<<" to "<<destinationStr << std::endl;

    validInput=false;


}


bool Player::controlsAllCountriesInMap(Graph& map)
{
    for(int i = 0; i < map.getNbrCountries(); i++)
    {
        Node* n = &(*map.getVectorOfNodes())[i];
        if(std::find(nodes.begin(), nodes.end(), n) != nodes.end())
            continue;
        else
            return false;
    }
    return true;
}

list<string> Player::getsContinentsOwned(std::map<string, Graph>* continents) {
    list<string> continentsOwned;
    map<string, Graph>::reverse_iterator graphIterator;
    //We iterate through all the continents and add the ones that the player completely owns
    for (graphIterator = continents->rbegin(); graphIterator != continents->rend(); ++graphIterator) {
        bool continentOwned = true;
        Graph *current_continent = &(graphIterator->second);
        vector<Node> *nodesInContinent = current_continent->getVectorOfNodes();
        //we iterate through all the countries in the current continent and check if the player owns them all
        for (int i = 0; i < nodesInContinent->size(); i++) {
            bool countryOwned = false;
            list<Node *>::const_iterator countryIterator;
            //We iterate through all the countries the player owns and see if he owns the current country from the continent we are checking
            for (countryIterator = nodes.begin(); countryIterator != nodes.end(); ++countryIterator) {
                if ((*nodesInContinent)[i].getCountry() == (*countryIterator)->getCountry()) {
                    countryOwned = true;
                    break;
                }
            }
            //If the current country does not belong to the player, then the whole continent does not belong to them.
            if (!countryOwned) {
                //cout << "allo";
                continentOwned = false;
                break;
            }
        }
        if (continentOwned)
            continentsOwned.push_back(graphIterator->first);
    }
    return continentsOwned;
}

void Player::printNodes()
{
    list<Node*>::const_iterator countryIterator;
    cout << "Countries that belong to " << name << ":" << endl;
    for (countryIterator = nodes.begin(); countryIterator != nodes.end(); ++countryIterator)
    {
        Country c = (*countryIterator)->getCountry();
        cout << c.getName() << "; ";
    }
    cout << endl << endl;
}
