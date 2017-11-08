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


/*
 * IMPORTANT: For the sake of working in an MVC architecture, Player is the controller: ie it receives the commands
 * from the user, and feeds the model (game) with information about the game and any changes in the states.
 */

//TODO: Remove all print statements
//TODO: Capture all phase changing information in a standard way to feed the model (game)



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
Player::Player(string n) : name(n), hand(new Hand), nodes(std::list<Node*>()), dice(new Dice)
{
    this->setStrategy(new HumanStrategy());
}
Player::Player(string n, Strategy* s) : name(n), strategy(s) { }
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

Hand* Player::getHand()
{
    return this->hand;
}

void Player::setDice(Dice* targetDice)
{
    this->dice = targetDice;
}

Dice* Player::getDice()
{
    return this->dice;
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

/**
 * GAME PHASES USING STRATEGY OBJECT
*/

std::vector<ReinforceResponse*>* Player::reinforce(vector<Continent*> continents)
{
    return this->strategy->reinforce(this, continents);
}

AttackResponse* Player::attack(Graph &map, std::vector<Player *> &players)
{
    return this->strategy->attack(this, map, players);
}
//Temporary
// TODO: Fix attack method, please do so in Strategy
// void Player::attack(Graph& map, std::vector<Player*> &players){}
/*
void Player::attack(Graph& map, std::vector<Player*> &players)
{*/
    /**
     * This method is called by the model for each player when it is their turn to attack. First a decision is made on
     * whether or not the player will make ANY attacks. Then, if the player chooses to make attacks, the first possible
     * attack is generated. The user must then decide whether to go through with that attack. If so, this method delegates
     * the actual attacking _event_ to the overloaded attack() method which returns boolean (true for win, false for loss)
     */
/*
    cout << this->getName() << ", do you wish to attack this turn? (y/n)";
    std::string willAttack;
    cin >> willAttack;
    if(willAttack == "n"){
        return;
    }
    //TODO: Reevaluate the countries you can attack after every attack. Instead of a list, make it the first available attack.

    //TODO: Return information about what happened during this attack phase.

    //TODO: Once adjacency list contains pointers and not copies fix this method to reflect hat (complexity plz)

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
*/
/*
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
*/

FortifyResponse* Player::fortify(Graph &map)
{
    return this->strategy->fortify(this, map);
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

bool Player::controlsAllCountriesInMap(Graph& map)
{
    for(int i = 0; i < map.getNbrCountries(); i++)
    {
        Node* n = (*map.getVectorOfNodes())[i];
        if(std::find(nodes.begin(), nodes.end(), n) != nodes.end())
            continue;
        else
            return false;
    }
    return true;
}

vector<Continent*> Player::getsContinentsOwned(vector<Continent*> continents) {
    vector<Continent*> continentsOwned;
    //We iterate through all the continents and add the ones that the player completely owns
    for (int i = 0; i < continents.size(); i++) {
        bool continentOwned = true;
        vector<Node*> nodesInCurrentContinent = *(continents[i]->getNodesInContinent());
        //we iterate through all the countries in the current continent and check if the player owns them all
        for (int j = 0; j < nodesInCurrentContinent.size(); j++) {
            bool countryOwned = false;
            list<Node *>::const_iterator countryIterator;
            //We iterate through all the countries the player owns and see if he owns the current country from the continent we are checking
            for (countryIterator = nodes.begin(); countryIterator != nodes.end(); ++countryIterator) {
                if (nodesInCurrentContinent[j]->getCountry() == (*countryIterator)->getCountry()) {
                    countryOwned = true;
                    break;
                }
            }
            //If the current country does not belong to the player, then the whole continent does not belong to them.
            if (!countryOwned) {
                continentOwned = false;
                break;
            }
        }
        if (continentOwned)
            continentsOwned.push_back(continents[i]);
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
