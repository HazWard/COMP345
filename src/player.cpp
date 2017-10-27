/*
    Filename: player.h 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Class representing a Risk Player
*/
#include <iostream>
#include <algorithm>
#include <map>
#include "../include/player.h"


// Constants
static const int MIN_NUMBER_OF_ARMIES = 3;
static const int MIN_NUMBER_OF_CARDS = 5;
static const int INFANTRY_BONUS = 1;
static const int CAVALRY_BONUS = 5;
static const int ARTILLERY_BONUS = 10;

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

void Player::reinforce(std::map<string, Graph>* map)
{
    // Perform actions to reinforce
    std::cout << "== REINFORCEMENT PHASE for " << this->name << " =="<< std::endl;
    unsigned long totalNbArmies = 0;
    if (this->nodes.size() >= MIN_NUMBER_OF_ARMIES)
    {
        totalNbArmies = this->nodes.size() / MIN_NUMBER_OF_ARMIES;
        std::list<std::string> continentsOwned =  getsContinentsOwned(map);

        for (unsigned int i = 0; i < continentsOwned.size(); i++) {
            totalNbArmies += 1; // Add 1 bonus point for each continent owned for now.
        }

        if(this->hand->getTotalCards() >= MIN_NUMBER_OF_CARDS)
        {
            totalNbArmies = (this->hand->exchange(Card::INFANTRY)) ? INFANTRY_BONUS + totalNbArmies : totalNbArmies;
            totalNbArmies = (this->hand->exchange(Card::ARTILLERY)) ? ARTILLERY_BONUS + totalNbArmies : totalNbArmies;
            totalNbArmies = (this->hand->exchange(Card::CAVALRY)) ? CAVALRY_BONUS + totalNbArmies : totalNbArmies;
        }
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
            std::cout << currentNode->getCountry().getNbrArmies() << " armie(s) ===" << std::endl;
            std::cout << "Do you want to add armies? (y/n) ";
            std::cin >> answer;
            if (answer == "y") {
                std::cout << "You already have " << currentNode->getCountry().getNbrArmies() << " armie(s) ?" << std::endl;
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
    /*
    Provide a group of C++ classes that implement the attack phase following the official rules of the game of Risk. In
    this phase, the player is allowed to declare a series of attacks to try to gain control of additional countries, and
    eventually control the entire map. The attack phase follows the following loop:
     The player decides if it will attack or not. If not, the attack phase is over.
     The player selects one of its countries to attack from, and one of the neighbors of this country to attack
    (i.e. the attacked country belongs to another player). The attacking country must have at least 2 armies
    on it.
     The attacker and defender players choose the number of dice to roll for their attack/defense. The attacker
    is allowed 1 to 3 dice, with the maximum number of dice being the number of armies on the attacking
    country, minus one. The defender is allowed 1 to 2 dice, with the maximum number of dice being the
    number of armies on the defending country.
     The dice are rolled for each player and sorted, then compared pair-wise. For each pair, the player with
    the lowest roll loses one army. If the pair is equal, the attacker loses an army.
     If the attacked country runs out of armies, it has been defeated. The defending country now belongs to
    the attacking player. The attacker is allowed to move a number of armies from the attacking country to the
    attacked country, in the range [1 to (number of armies on attacking country -1)].
     The player is allowed to initiate any number of attacks per turn, including 0.
    You must deliver a driver that demonstrates that 1) only valid attacks can be declared (i.e. valid attacker/attacked
    country); 2) only valid number of dice can be chosen by the attacker/defender; 3) given known dice values, that
    the right number of armies are deducted on the attacker/defender; 4) the attacker is allowed to initiate multiple
    attacks, until it declares that it does not want to attack anymore.
     */


    //TODO: Step 1, find the countries it can attack
//    1
    //TODO: Step 2, declare attacks

    //TODO: Step 3, perform the attacking, possibly using some static function to perform the attacking

    //TODO: Step 4, conclude attacks and test results


}

void Player::fortify(Graph& map)
{
    string sourceStr;
    string destinationStr;
    int armNum;
    bool validInput = false;
    Node* sourceCtr = nullptr;
    Node* destCtr = nullptr;

    //this while loop asks for source and loops if not owned
    do {
        std::cout << "Please enter the source country ";
        getline(cin, sourceStr);

        list<Node*>::const_iterator sourceCountryIterator;

        for (sourceCountryIterator = nodes.begin(); sourceCountryIterator != nodes.end(); ++sourceCountryIterator)
        {
            if (sourceStr==(*sourceCountryIterator)->getCountry().getName() && (*sourceCountryIterator)->getCountry().getNbrArmies() > 1) {
                sourceCtr = *sourceCountryIterator;
                validInput=true;
                break;
            }
        }
        if(!validInput)
            std::cout << "Source country is not owned please enter a country you own" << std::endl;
    }   while(!validInput);

    validInput=false;

    //this while loop asks for destination and loops if not owned or if not connected to source
    do {
        std::cout << "Please enter the destination country ";
        getline(cin, destinationStr);

        list<Node*>::const_iterator destinationCountryIterator;

     /*   for (destinationCountryIterator = nodes.begin(); destinationCountryIterator != nodes.end(); ++destinationCountryIterator)
        {
            if (destinationStr==(*destinationCountryIterator)->getCountry().getName() && map.areConnectedByEdge(*destinationCountryIterator, sourceCtr)) {
                destCtr = *destinationCountryIterator;
                validInput=true;
                break;
            }
        }
        */
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

bool Player::attack(Player *attacker, Player *defender, Country *attackingCountry, Country *defendingCountry) {
    //TODO: Write the code that will encapsulate the attacking process according to the Risk rules
    return false;
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