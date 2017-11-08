#include "../include/game.h"
#include <dirent.h>
#include <iostream>
#include <list>
#include <random>

//Imports necessary on Windows G++ compilers
#include <algorithm>
#include <ctime>
#include <chrono>
#include <list>

using namespace std;

const string MAPS_FOLDER = "../maps/";

//Constructor for the Game class
//Uses the helper methods and checks that the number of players created is equal to the
//private number nbrPlayers initialized by the user.
//If they are not equal, that means the constructor failed to do its job and we exit the program.
Game::Game()
{
    list<string> mapFiles;

    //Checking the operating system by using the windows boolean, determines how paths are set
    if(windows){ mapFiles = getNameOfFiles("..\\maps"); }
    else { mapFiles = getNameOfFiles("../maps"); }

    getMapUser(mapFiles);
    this->nbrPlayers = getNbrPlayersUser();
    this->arrayPlayers = *(getPlayersUser(nbrPlayers));
    this->mainDeck = Deck(mapCountries.getNbrCountries());
    if(nbrPlayers != arrayPlayers.size())
    {
        cout << "The number of players (" << nbrPlayers << " and "
             << "the number of players created (" << arrayPlayers.size()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
    if(mainDeck.getNumberOfCards() != mapCountries.getNbrCountries())
    {
        cout << "The number of cards (" << mainDeck.getNumberOfCards() << " and "
             << "the number of nodes in the map (" << mapCountries.getNbrCountries()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
}

//ACCESSOR METHODS
string Game::getMapName() { return mapName; }

int Game::getNbrPlayers() { return nbrPlayers; }

vector<Player*>* Game::getArrayPlayers() { return &arrayPlayers; }

Graph* Game::getMapCountries() { return &mapCountries; }

Deck Game::getMainDeck() { return mainDeck; }

vector<Continent*>* Game::getContinents() { return &continents; };

//-- MUTATOR METHODS --
void Game::setMap(Graph& newMap)
{
    mapCountries = newMap;
}
void Game::setNbrPlayers(int nbrP)
{
    nbrPlayers = nbrP;
}
void Game::setArrayPlayers(vector<Player*>& newArrayPl)
{
    arrayPlayers = newArrayPl;
}

//Function to read all files from a given folder taken from:
//https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
list<string> Game::getNameOfFiles(const char *path)
{
    list<string> listOfMapFiles;
    struct dirent *entry;
    DIR *directory = opendir(path);
    //If we cannot open the directory, we simply return an empty list of names
    if (directory == nullptr)
    {
        return list<string>();
    }
    //We read the files in the given directory (MAPS_FOLDER) until there are no more directories to read.
    //That means we have read every file (presumably maps) inside that folder.
    while ((entry = readdir(directory)) != nullptr)
    {
        string currentFileName = entry->d_name;
        //This condition is to avoid adding the file names that will always be there and that
        //we will not ever care about.
        if(currentFileName != "." && currentFileName != "..") {
            listOfMapFiles.push_back(currentFileName);
        }
    }
    //We have to close the directory to avoid some memory leak
    closedir(directory);
    return listOfMapFiles;
}

void Game::getMapUser(list<string> listOfMapFiles) {
    /**
     * A method to ask the user to choose a map file among the list.
     * We will try to read the file and create a Graph from it, along with a map container of the continents it contains.
     * If the method mapIsValid of our Parser object returns false, that means that the current map file is not valid.
     * We will thus ask the user to enter another number until he chooses a valid map file.
     * When a valid map file is selected, we set mapCountries to the map and continents to the map container of continents read from the file.
     */
    cout << "Here is the list of available map files. Choose a map by entering the number associated with the one you want." << endl;
    int i = 0;
    int indexMapChosen = -1;
    list<string>::const_iterator iterator;
    for (iterator = listOfMapFiles.begin(); iterator != listOfMapFiles.end(); ++iterator)
    {
        cout << i+1 << ": " << *iterator << endl;
        i++;
    }
    cout << endl;
    bool validIndexMap = false;
    Parser* parser;
    do
    {
        cout << "Map chosen: ";
        cin >> indexMapChosen;
        indexMapChosen--;
        if (indexMapChosen >= listOfMapFiles.size() || indexMapChosen < 0)
        {
            cout << indexMapChosen + 1 << " is not a valid index. Please enter an index from 1 to "
                 << listOfMapFiles.size() << "." << endl;
            validIndexMap = false;
        }
        else
        {
            list<string>::iterator it = listOfMapFiles.begin();
            advance(it, indexMapChosen);
            mapName = *it;
            cout << "You chose the map " << mapName << endl;
            cout << "We will check if that map is a valid one." << endl;
            parser = new Parser(MAPS_FOLDER + mapName);
            cout << "Is parser a valid map ? : ";
            if (parser->mapIsValid()) {
                cout << "Yes, both the entire map as a whole and each continent are connected.\n";
                validIndexMap = true;
            }
            else if(parser->getGraph()->getNbrCountries() > 80){
                cout << "No" << endl << "We have detected that the number of countries in this Map is greater than 80." << endl
                                << "That is not supported in the current version of the game." << endl;
                validIndexMap = false;
            }
            else {
                cout << "No, the graph and/or some of the continents are not strongly connected.\n";
                validIndexMap = false;
            }
        }
    } while(!validIndexMap);
    this->mapCountries = *parser->getGraph();
    this->continents = *parser->getContinents();
    delete parser;
}

//Method that asks the user for the number of players
int Game::getNbrPlayersUser()
{
    int nbrPlayers;
    do {
        cout << "How many players are playing the game? (2-6 players)";
        cin >> nbrPlayers;
        if(nbrPlayers < 2 || nbrPlayers > 6)
            cout << "Error: Invalid amount of players (only from 2 to 6)" << endl;
    }
    while(nbrPlayers < 2 || nbrPlayers > 6);

    return nbrPlayers;
}
//Method that asks the user for the names of the players and creates player objects
vector<Player*>* Game::getPlayersUser(int np)
{
    vector<Player*>* pl = new vector<Player*>;
    pl->reserve(np);
    cin.ignore();
    string namePlayer;
    for(int i = 0; i < np; i++)
    {
        cout << "Enter the name of player " << (i+1) << ": ";
        getline(cin, namePlayer);
        pl->push_back(new Player(namePlayer));
    }
    return pl;
}

//Method used to
void Game::determinePlayerTurn() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (arrayPlayers.begin(), arrayPlayers.end(), std::default_random_engine(seed));
}

/*The following method has been provided by:
http://rextester.com/HHTW39678
http://www.cplusplus.com/forum/general/207328/*/
//This method is used to shuffle a list of any type randomly
template <class T > void listShuffle( list<T> &L )
{
    mt19937 gen( chrono::system_clock::now().time_since_epoch().count() );
    vector<T> V( L.begin(), L.end() );
    shuffle( V.begin(), V.end(), gen );
    L.assign( V.begin(), V.end() );
}

//Method used to assign countries one by one to the players in a round robin fashion
void Game::assignCountriesToPlayers()
{
    vector<Node*> listOfNodes = *(this->mapCountries.getVectorOfNodes());
    //We create a list from the vector of nodes of the map
    //We use a list since we will be removing a lot of elements and the
    //underlying data structure of a list is a linked list, which is better suited for insertion/removal of elements
    //then arrays (vectors use arrays as their underlying structure)
    list<Node*> nodesToAssign;
    for(int i = 0; i < listOfNodes.size(); i++)
        nodesToAssign.push_back(listOfNodes[i]);

    //As long as there are still nodes to be assigned to players, we shuffle the list and pop the first element from it
    while(!nodesToAssign.empty()) {
        //We assign countries to players one by one in a round robin fashion, until there are no more countries to assign
            for (int i = 0; i < this->arrayPlayers.size(); i++) {
                listShuffle(nodesToAssign);
                if(nodesToAssign.front() != nullptr) {
                    this->arrayPlayers[i]->addNode(nodesToAssign.front());
                    nodesToAssign.pop_front();
                }
            }
        }
}
//The 2 following functions are used in the method:
//TODO: use them in fortify
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
string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool Game::armiesLeftToPlace(vector<int> nbrArmiesPlayers){
    for(int i = 0; i < nbrArmiesPlayers.size(); i++)
    {
        if(nbrArmiesPlayers[i]  > 0)
            return true;
    }
    return false;
}

//This method is used to place armies on the players' countries.
void Game::placeArmies()
{
    //the number of armies each player has to place varies depending on the number of players
    int nbrArmiesPerPlayer = 0;
    switch(nbrPlayers) {
        case 2:
            nbrArmiesPerPlayer = 40;
            break;
        case 3:
            nbrArmiesPerPlayer = 35;
            break;
        case 4:
            nbrArmiesPerPlayer = 30;
            break;
        case 5:
            nbrArmiesPerPlayer = 25;
            break;
        case 6:
            nbrArmiesPerPlayer = 20;
            break;
        default:
            exit(EXIT_FAILURE);
    }

    //Each player starts with the nbrArmiesPerPlayer for the current number of players playing
    //The index of this vector is the same as the index in the player array
    //Each player will have to place nbrArmiesPerPlayer number of armies.
    vector<int> nbrArmiesPlayers(arrayPlayers.size(), nbrArmiesPerPlayer);

    //We are going to place 1 army per owbed territory in a round robin fashion. This process is automated.
    cout << "Placing 1 army per owned Territory for each player..." << endl;
    bool allNodesHaveOneArmy = false;
    int i = 0, k = 0;
    while(!allNodesHaveOneArmy) {
        for (int i = 0; i < nbrPlayers; i++) {
            int j = 0;
            for(auto const &node : arrayPlayers[i]->getNodes()){
                if(j == k) {
                    node->getPointerToCountry()->setNbrArmies(1);
                    nbrArmiesPlayers[i]--;
                    break;
                }
                j++;
            }
        }
        k++;
        allNodesHaveOneArmy = true;
        for(int i = 0; i < nbrPlayers; i++)
        {
            if(k < arrayPlayers[i]->getNodes().size()) {
                allNodesHaveOneArmy = false;
                break;
            }
        }
    }

    //Now, we ask for the player to add the remaining armies that were not automatically added, one by one.
    //He only has to select a valid index associated with the country.
    while(armiesLeftToPlace(nbrArmiesPlayers)) {
        for(int i = 0; i < nbrPlayers; i++)
        {
            Country *c;
            bool validCountryIndex = false;

            if(nbrArmiesPlayers[i] <= 0)
                continue;

            cout << "\n---------------Player: " << arrayPlayers[i]->getName() << " --------------" << endl;
            int k = 1;
            for (auto const &node : arrayPlayers[i]->getNodes()) {
                cout << k << ": " << node->getCountry().getName() << endl;
                k++;
            }
            cout << arrayPlayers[i]->getName() << " has " << nbrArmiesPlayers[i]
                 << " armies to place. Please choose the index of the country you want to place 1 army on."
                 << endl;
            do{
                int chosenCountryInd;
                cin >> chosenCountryInd;
                k = 1;
                for (auto const &node2 : arrayPlayers[i]->getNodes()) {
                    if(k == chosenCountryInd)
                    {
                        c = node2->getPointerToCountry();
                        validCountryIndex = true;
                        break;
                    }
                    k++;
                }
                if(!validCountryIndex)
                    cout << "You did not enter a valid country index from the list. Please make sure to enter a number between 1 and " << arrayPlayers[i]->getNodes().size() << ".\n";
            }   while(!validCountryIndex);

            //ONE ARMY AT A TIME: (comment this out for SUBMISSION)
            //c->setNbrArmies(c->getNbrArmies() + 1);
            //nbrArmiesPlayers[i]--;

            //SPEED UP VERSION FOR TESTING: (comment this out for TESTING)
            int nbrArmiesToPlace = 0;
            cout << "Please enter the number of armies to place on " << c->getName() << ": ";
            cin >> nbrArmiesToPlace;
            cin.ignore();
            if(nbrArmiesToPlace <= nbrArmiesPlayers[i]) {
                nbrArmiesPlayers[i] -= nbrArmiesToPlace;
                c->setNbrArmies(c->getNbrArmies() + nbrArmiesToPlace);
            }
            else
            {
                cout << "The player has " << nbrArmiesPlayers[i] << " armies left to place. ";
                cout << "You cannot place " << nbrArmiesToPlace << " armies on " << c->getName() << ".\n\n";
            }
        }
    }
    if(verifyPlayerArmies(nbrArmiesPerPlayer))
        cout << endl << "All players have successfully placed their armies.\n";
    else {
        cout << "There was an error: Each player has not placed " << nbrArmiesPerPlayer << " armies.\n";
        exit(EXIT_FAILURE);
    }
}
//Private method used to verify that each player successfully placed exactly the right number of armies to be placed.
bool Game::verifyPlayerArmies(int nbrArmiesPerPlayer)
{
    for(int i = 0; i < nbrPlayers; i++)
    {
        int nbrArmies = 0;
        for (auto const& node : arrayPlayers[i]->getNodes()) {
            nbrArmies += node->getCountry().getNbrArmies();
        }
        if(nbrArmies != nbrArmiesPerPlayer)
            return false;
    }
    return true;
}

/**
 * Helper method to perform attacking phase
 */
bool Game::performAttack(Player &attacker, Player &defender, Country &attackingCountry, Country &defendingCountry) {
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

//Main for Part 1
void gameStartDriver()
{
    // The constructor verifies that the map loaded is valid.
    // Invalid maps are rejected without the program crashing.
    // Also, we check that the right number of players is created inside the constructor as well.
    Game riskGame;
}


// Main for Part 2
void startupPhaseDriver()
{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
    Game riskGame;
    //Determine player order and print them to check that the order changed (randomly)
    vector<Player*>* players = riskGame.getArrayPlayers();

    vector<Continent*> continents = *(riskGame.getContinents());

    cout << "Player order before we randomize the order:" << endl;
    for(int i = 0; i < players->size(); i++)
    {
        cout << (*players)[i]->getName() << " ";
    }
    cout << endl;

    riskGame.determinePlayerTurn();

    cout << "Player order after we randomize the order:" << endl;
    for(int i = 0; i < players->size(); i++)
    {
        cout << (*players)[i]->getName() << " ";
    }
    cout << endl << endl;

    riskGame.assignCountriesToPlayers();

    vector<Player*> play = *(riskGame.getArrayPlayers());

    //Displaying all the countries in the graph
    for(int i = 0; i < continents.size(); i++)
    {
        cout << *(continents[i]);
    }

    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        play[i]->printNodes();
    }
    riskGame.placeArmies();

    cout << "TESTING";
    for(auto &node : *riskGame.getMapCountries()->getVectorOfNodes()){
        cout << *node << endl;
    }
}

//Main for Part 3

void mainGameLoopDriver()
{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
    Game riskGame;
    //Determine player order and print them to check that the order changed (randomly)
    vector<Player*>* players = riskGame.getArrayPlayers();

    vector<Continent*> continents = *(riskGame.getContinents());

    cout << "Player order before we randomize the order:" << endl;
    for(int i = 0; i < players->size(); i++)
    {
        cout << (*players)[i]->getName() << " ";
    }
    cout << endl;

    riskGame.determinePlayerTurn();

    cout << "Player order after we randomize the order:" << endl;
    for(int i = 0; i < players->size(); i++)
    {
        cout << (*players)[i]->getName() << " ";
    }
    cout << endl << endl;

    riskGame.assignCountriesToPlayers();

    vector<Player*> play = *(riskGame.getArrayPlayers());

    //Displaying all the countries in the graph
    for(int i = 0; i < continents.size(); i++)
    {
        cout << *(continents[i]);
    }

    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        play[i]->printNodes();
    }
    riskGame.placeArmies();

    //Boolean is false until a player wins. this is the breaking condition of the main game loop
    bool playerWins = false;

    //We keep track of the winning player
    Player* winningPlayer;

    //Main game loop
    while(!playerWins)
    {
        for(int i = 0; i < players->size(); i++)
        {
            // Each player gets to reinforce, attack and fortify
            // TODO: Add Game method to perform concrete changes for each phase
            std::vector<ReinforceResponse> reinforceChanges = (*players)[i]->reinforce(continents);
            AttackResponse attackChanges = (*players)[i]->attack(*riskGame.getMapCountries(), play);
            FortifyResponse fortifyChanges = (*players)[i]->fortify(*riskGame.getMapCountries());

            //After each player's turn, we check if one player owns all the countries in the map
            if((*players)[i]->controlsAllCountriesInMap(*riskGame.getMapCountries())) {
                playerWins = true;
                winningPlayer = (*players)[i];
                break;
            }
        }
    }
    cout << winningPlayer->getName() << " won the game of risk! Congratulations!!!" << endl;
}

// Main for Part 4
void reinforceDriver()
{
    Game riskGame;
    vector<Player*>* players = riskGame.getArrayPlayers();

    vector<Continent*> continents = *(riskGame.getContinents());
    map<string, Graph>::reverse_iterator countryIterator;

    riskGame.determinePlayerTurn();
    riskGame.assignCountriesToPlayers();

    vector<Player*> play = *(riskGame.getArrayPlayers());

    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        play[i]->printNodes();
    }
    riskGame.placeArmies();

    bool playerWins = false;
    while(!playerWins)
    {
        for(int i = 0; i < players->size(); i++)
        {
            (*players)[i]->reinforce(continents);
        }
        playerWins = true; // Force quit for Demo
    }

    // Print state of map to see number of armies
    for (auto &node : *riskGame.getMapCountries()->getVectorOfNodes())
    {
        std::cout << *node << std::endl;
    }
}

// Main for Part 5
void attackDriver()
{
    Game riskGame;
    vector<Player*>* players = riskGame.getArrayPlayers();

    vector<Continent*>* continents = riskGame.getContinents();

    riskGame.determinePlayerTurn();
    riskGame.assignCountriesToPlayers();

    vector<Player*> play = *(riskGame.getArrayPlayers());

    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        play[i]->printNodes();
    }
    riskGame.placeArmies();

    bool playerWins = false;
    while(!playerWins)
    {
        for(int i = 0; i < players->size(); i++)
        {
            (*players)[i]->attack(*riskGame.getMapCountries(), *riskGame.getArrayPlayers());
        }
        playerWins = true; // Force quit for Demo
    }

    // Print state of map to see number of armies
    for (auto &node : *riskGame.getMapCountries()->getVectorOfNodes())
    {
        std::cout << node << std::endl;
    }
}

// Main for Part 6
void fortifyDriver()
{
    /*The constructor verifies that the map loaded is valid.
     Invalid maps are rejected without the program crashing.
     Also, we check that the right number of players is created inside the constructor as well.*/
    Game riskGame;
    //Determine player order and print them to check that the order changed (randomly)
    vector<Player*>* players = riskGame.getArrayPlayers();

    vector<Continent*> continents = *(riskGame.getContinents());

    cout << "Player order before we randomize the order:" << endl;
    for(int i = 0; i < players->size(); i++)
    {
        cout << (*players)[i]->getName() << " ";
    }
    cout << endl;

    riskGame.determinePlayerTurn();

    cout << "Player order after we randomize the order:" << endl;
    for(int i = 0; i < players->size(); i++)
    {
        cout << (*players)[i]->getName() << " ";
    }
    cout << endl << endl;

    riskGame.assignCountriesToPlayers();

    vector<Player*> play = *(riskGame.getArrayPlayers());

    //Displaying all the countries in the graph
    for(int i = 0; i < continents.size(); i++)
    {
        cout << *(continents[i]);
    }

    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        play[i]->printNodes();
    }
    riskGame.placeArmies();

    cout << "TESTING";
    for(auto &node : *riskGame.getMapCountries()->getVectorOfNodes()){
        cout << *node << endl;
    }

    //testing fortify
    for (int i = 0; i < players->size(); i++) {
        (*players)[i]->fortify(*riskGame.getMapCountries());
    }
}

int main()
{
    int part = 0;
    while(part >= 0)
    {
        std::cout << "Please choose a part to run: ";
        std::cin >> part;
        switch(part) {
            case 1 : std::cout << "== PART 1 == ";
                gameStartDriver(); // Driver for Part 1
                break;

            case 2 : std::cout << "== PART 2 == ";
                startupPhaseDriver(); // Driver for Part 2
                break;
            case 3 : std::cout << "== PART 3 == ";
                mainGameLoopDriver(); // Driver for Part 3
                break;
            case 4 : std::cout << "== PART 4 == ";
                reinforceDriver(); // Driver for Part 4
                break;
            case 5 : std::cout << "== PART 5 == ";
                attackDriver(); // Driver for Part 5
                break;
            case 6 : std::cout << "== PART 6 == ";
                fortifyDriver(); // Driver for Part 6
                break;
            default:
                part = -1;
                break;
        }
    }
}