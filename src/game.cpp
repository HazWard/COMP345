#include "../include/game.h"
#include <dirent.h>
#include "../include/events.h"
#include "../include/views.h"
#include "../include/decorators.h"
#include "../include/tournament.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <cstdio>
#include <set>

using namespace std;

bool windows = false;
#ifdef OS_WINDOW1S
windows = true;
#endif

//forward declarations
static bool reinforcementsMade(std::vector<ReinforceResponse*>* responses);

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
    this->arrayPlayers = *(getPlayersAutomatic(nbrPlayers));
    determinePlayerTurn();
    this->mainDeck = Deck(mapCountries->getNbrCountries());
    if(nbrPlayers != arrayPlayers.size())
    {
        cout << "The number of players (" << nbrPlayers << " and "
             << "the number of players created (" << arrayPlayers.size()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
    if(mainDeck.getNumberOfCards() != mapCountries->getNbrCountries())
    {
        cout << "The number of cards (" << mainDeck.getNumberOfCards() << " and "
             << "the number of nodes in the map (" << mapCountries->getNbrCountries()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
}

Game::Game(Parser* map, vector<Player*> pl, int maximum_turns)
{
    this->arrayPlayers.clear();
    this->mapName = "some_map";
    this->mapCountries = map->getGraph();
    //TODO: might cause a problem we shall see (content of pointer)
    this->continents = *(map->getContinents());
    this->arrayPlayers.reserve(pl.size());
    for(int i = 0; i < pl.size(); i++)
        this->arrayPlayers.push_back(pl[i]);
    for(int i = 0; i < arrayPlayers.size(); i++)
    {
        arrayPlayers[i]->getNodes()->clear();
    }
    this->nbrPlayers = pl.size();
    determinePlayerTurn();
    this->mainDeck = Deck(mapCountries->getNbrCountries());
    this->max_turns = maximum_turns;
    if(nbrPlayers != arrayPlayers.size())
    {
        cout << "The number of players (" << nbrPlayers << ") and "
             << "the number of players created (" << arrayPlayers.size()
             << ") is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
    if(mainDeck.getNumberOfCards() != mapCountries->getNbrCountries())
    {
        cout << "The number of cards (" << mainDeck.getNumberOfCards() << " and "
             << "the number of nodes in the map (" << mapCountries->getNbrCountries()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
}

//ACCESSOR METHODS
string Game::getMapName() { return mapName; }

int Game::getNbrPlayers() { return nbrPlayers; }

vector<Player*>* Game::getArrayPlayers() { return &arrayPlayers; }

Graph* Game::getMapCountries() { return mapCountries; }

Deck* Game::getMainDeck() { return &mainDeck; }

vector<Continent*>* Game::getContinents() { return &continents; };

//-- MUTATOR METHODS --
void Game::setMap(Graph* newMap)
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
list<string> getNameOfFiles(const char *path)
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

void Game::getMapAutomatic()
{
    /**
    A method to get the default map (World.map) and load it after verifying if it is indeed valid.
 */
    mapName = DEFAULT_MAP_NAME;
    Parser* parser;
    cout << "Loading map located at " << MAPS_FOLDER << mapName << endl;

    parser = new Parser(MAPS_FOLDER + mapName);

    if (parser->mapIsValid()) {
        cout << "The map " << mapName << " is valid.\n\n";
    }
    else if(parser->getGraph()->getNbrCountries() > 80){
        cout << "We have detected that the number of countries in this Map is greater than 80." << endl
             << "That is not supported in the current version of the game." << endl
             << "We will exit the program." << endl;
        exit(EXIT_FAILURE);
    }
    else {
        cout << "The graph and/or some of the continents are not strongly connected.\n";
        cout << "We will exit the program." << endl;
        exit(EXIT_FAILURE);
    }
    this->mapCountries = parser->getGraph();
    this->continents = *parser->getContinents();
    delete parser;
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
        cout << "\t" << i+1 << ": " << *iterator << endl;
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

    this->mapCountries = parser->getGraph();
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

//Method that sets up the players
vector<Player*>* Game::getPlayersAutomatic(int np)
{
    vector<Player*>* pl = new vector<Player*>;
    pl->reserve(np);
    for(int i = 0; i < np; i++)
    {
        pl->push_back(new Player(DEFAULT_PLAYER_NAMES[i]));
    }
    return pl;
}

//Method used to determine the order of player turn which is random
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
    vector<Node*> listOfNodes = *(this->mapCountries->getVectorOfNodes());
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
            for(auto const &node : *(arrayPlayers[i]->getNodes())){
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
            if(k < arrayPlayers[i]->getNodes()->size()) {
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
            for (auto const &node : *(arrayPlayers[i]->getNodes())) {
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
                for (auto const &node2 : *(arrayPlayers[i]->getNodes())) {
                    if(k == chosenCountryInd)
                    {
                        c = node2->getPointerToCountry();
                        validCountryIndex = true;
                        break;
                    }
                    k++;
                }
                if(!validCountryIndex)
                    cout << "You did not enter a valid country index from the list. Please make sure to enter a number between 1 and " << arrayPlayers[i]->getNodes()->size() << ".\n";
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

//This method is used to place armies on the players' countries.
void Game::placeArmiesAutomatic()
{
    std::random_device rd;
    std::mt19937 mt(rd());
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

    for (int i = 0; i < nbrPlayers; i++)
    {
        cout << nbrPlayers << " ";
        cout << (*(arrayPlayers[i]->getNodes())).size() << endl;
    }

    //We are going to place 1 army per owbed territory in a round robin fashion. This process is automated.
    cout << "Placing 1 army per owned Territory for each player..." << endl;
    bool allNodesHaveOneArmy = false;
    int i = 0, k = 0;
    while(!allNodesHaveOneArmy) {
        for (int i = 0; i < nbrPlayers; i++) {
            int j = 0;
            for(auto const &node : *(arrayPlayers[i]->getNodes())){
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
            if(k < arrayPlayers[i]->getNodes()->size()) {
                allNodesHaveOneArmy = false;
                break;
            }
        }
    }
    for(int i = 0; i < nbrArmiesPlayers.size(); i++)
        cout << nbrArmiesPlayers[i] << endl;

    //Now, we ask for the player to add the remaining armies that were not automatically added, one by one.
    //He only has to select a valid index associated with the country.
    while(armiesLeftToPlace(nbrArmiesPlayers)) {
        for(int i = 0; i < nbrPlayers; i++)
        {
            Country *c;
            bool validCountryIndex = false;

            if(nbrArmiesPlayers[i] <= 0)
                continue;
            std::uniform_int_distribution<int> dist(0, arrayPlayers[i]->getNodes()->size()-1);
            do{
                int chosenCountryInd = dist(mt); // store the random number into the chosen country index

                int k = 0;
                for (auto const &node2 : *(arrayPlayers[i]->getNodes())) {
                    if(k == chosenCountryInd)
                    {
                        c = node2->getPointerToCountry();
                        validCountryIndex = true;
                        break;
                    }
                    k++;
                }
            }   while(!validCountryIndex);

            c->setNbrArmies(c->getNbrArmies() + 1);
            nbrArmiesPlayers[i]--;
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
        for (auto const& node : *(arrayPlayers[i]->getNodes())) {
            nbrArmies += node->getCountry().getNbrArmies();
        }
        cout << nbrArmies << "   " << nbrArmiesPerPlayer << endl;
        if(nbrArmies != nbrArmiesPerPlayer)
            return false;
    }
    return true;
}

void Game::chooseGameScenario(vector<Player*>* players)
{
    cout << "Please choose a game scenario: " << endl;
    cout << "Scenario 1:" << endl;
    for(int i = 0; i < nbrPlayers; i++)
    {
        switch(i)
        {
            case 0: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n"; break;
            case 1: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 2: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 3: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n"; break;
            case 4: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 5: cout << "\t" << (*players)[i]->getName() << " is human.\n";
        }
    }
    cout << "Scenario 2:" << endl;
    for(int i = 0; i < nbrPlayers; i++)
    {
        switch(i)
        {
            case 0: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 1: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 2: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 3: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 4: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 5: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n";
        }
    }
    cout << "Scenario 3:" << endl;
    for(int i = 0; i < nbrPlayers; i++)
    {
        switch(i)
        {
            case 0: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 1: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 2: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 3: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 4: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 5: cout << "\t" << (*players)[i]->getName() << " is human.\n";
        }
    }
    cout << "Scenario 4:" << endl;
    for(int i = 0; i < nbrPlayers; i++)
    {
        switch(i)
        {
            case 0: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 1: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 2: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 3: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 4: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 5: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n";
        }
    }
    cout << "Scenario 5:" << endl;
    for(int i = 0; i < nbrPlayers; i++)
    {
        switch(i)
        {
            case 0: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 1: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n"; break;
            case 2: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 3: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n"; break;
            case 4: cout << "\t" << (*players)[i]->getName() << " is aggressive.\n"; break;
            case 5: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n";
        }
    }
    cout << "Scenario 6:" << endl;
    for(int i = 0; i < nbrPlayers; i++)
    {
        switch(i)
        {
            case 0: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 1: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n"; break;
            case 2: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 3: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n"; break;
            case 4: cout << "\t" << (*players)[i]->getName() << " is human.\n"; break;
            case 5: cout << "\t" << (*players)[i]->getName() << " is benevolent.\n"; break;
        }
    }

    int scenarioChosen;
    bool validScenario = false;
    while(!validScenario) {
        cout << "\n What shall you choose? ";
        cin >> scenarioChosen;
        if(scenarioChosen <= 0 || scenarioChosen >= 8)
        {
            cout << "Please enter a scenario from 1 to 7." << endl;
            validScenario = false;
        } else validScenario = true;
    }
    switch(scenarioChosen)
    {
        case 1:
        {
            for(int i = 0; i < nbrPlayers; i++)
            {
                switch(i)
                {
                    case 0: (*players)[i]->setStrategy(new BenevolentStrategy); break;
                    case 1: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 2: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 3: (*players)[i]->setStrategy(new BenevolentStrategy); break;
                    case 4: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 5: (*players)[i]->setStrategy(new HumanStrategy);
                }
            }
        } break;
        case 2:
        {
            for(int i = 0; i < nbrPlayers; i++)
            {
                switch(i)
                {
                    case 0: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 1: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 2: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 3: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 4: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 5: (*players)[i]->setStrategy(new AggressiveStrategy);
                }
            }
        } break;
        case 3:
        {
            for(int i = 0; i < nbrPlayers; i++)
            {
                switch(i)
                {
                    case 0: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 1: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 2: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 3: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 4: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 5: (*players)[i]->setStrategy(new HumanStrategy);
                }
            }
        } break;
        case 4:
        {
            for(int i = 0; i < nbrPlayers; i++)
            {
                switch(i)
                {
                    case 0: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 1: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 2: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 3: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 4: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 5: (*players)[i]->setStrategy(new AggressiveStrategy);
                }
            }
        } break;
        case 5:
        {
            for(int i = 0; i < nbrPlayers; i++)
            {
                switch(i)
                {
                    case 0: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 1: (*players)[i]->setStrategy(new BenevolentStrategy); break;
                    case 2: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 3: (*players)[i]->setStrategy(new BenevolentStrategy); break;
                    case 4: (*players)[i]->setStrategy(new AggressiveStrategy); break;
                    case 5: (*players)[i]->setStrategy(new BenevolentStrategy);
                }
            }
        } break;
        case 6:
        {
            for(int i = 0; i < nbrPlayers; i++)
            {
                switch(i)
                {
                    case 0: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 1: (*players)[i]->setStrategy(new BenevolentStrategy); break;
                    case 2: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 3: (*players)[i]->setStrategy(new BenevolentStrategy); break;
                    case 4: (*players)[i]->setStrategy(new HumanStrategy); break;
                    case 5: (*players)[i]->setStrategy(new BenevolentStrategy);
                }
            }
        }
        /*
        case 7:
        {
            for(int i = 0; i < nbrPlayers; i++)
            {
                switch(i)
                {
                    case 0: (*players)[i]->setStrategy(new CheaterStrategy()); break;
                    case 1: (*players)[i]->setStrategy(new BenevolentStrategy()); break;
                    case 2: (*players)[i]->setStrategy(new HumanStrategy()); break;
                    case 3: (*players)[i]->setStrategy(new BenevolentStrategy()); break;
                    case 4: (*players)[i]->setStrategy(new HumanStrategy()); break;
                    case 5: (*players)[i]->setStrategy(new BenevolentStrategy());
                }
            }
        }*/
}
    //Testing the type of strategy of each player:
    for(int i = 0; i < nbrPlayers; i++)
    {
        cout << (*players)[i]->getName() << ": ";

        Strategy::StrategyType strategyType = (*players)[i]->getStrategy()->getType();
        switch(strategyType)
        {
            case Strategy::ABSTRACT  : std::cout << "Abstract Strategy";   break;
            case Strategy::AGGRESSIVE: std::cout << "Aggressive Strategy";   break;
            case Strategy::BENEVOLENT: std::cout << "Benevolent Strategy";   break;
            case Strategy::HUMAN: std::cout << "Human Strategy";   break;
            case Strategy::CHEATER  : std::cout << "Cheater Strategy";   break;
            case Strategy::RANDOM: std::cout << "Random Strategy";   break;
        }
    }
    cout << endl << endl;
}

//empty the list of nodes of each player, and set the number of armies of every country in the graph to 0
void Game::reinitialize_game() {
    for (int i = 0; i < this->arrayPlayers.size(); i++)
    {
        this->arrayPlayers[i]->getNodes()->clear();
    }
    for(int i = 0; i < mapCountries->getVectorOfNodes()->size(); i++)
    {
        (*mapCountries->getVectorOfNodes())[i]->getPointerToCountry()->setNbrArmies(0);
    }
}

void part1()
{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
    Game riskGame;

    riskGame.max_turns = riskGame.DEFAULT_MAX_TURNS;
    //Determine player order and print them to check that the order changed (randomly)
    vector<Continent*> continents = *(riskGame.getContinents());

    riskGame.assignCountriesToPlayers();

    vector<Player*>* players = riskGame.getArrayPlayers();

    //Displaying all the continents in the graph
    for(int i = 0; i < continents.size(); i++)
    {
        cout << *(continents[i]);
    }

    riskGame.placeArmiesAutomatic();

    //Setting player strategy according to our 5 scenarios
    riskGame.chooseGameScenario(players);

    //Boolean is false until a player wins. this is the breaking condition of the main game loop
    bool playerWins = false;

    riskGame.currentTurn = 1;

    //creating observers
    Observer *phaseObserver = new PhaseObserver(static_cast<Subject*>(&riskGame));
    Observer *statObserver = new StatObserver(static_cast<Subject*>(&riskGame));
    //Unsure whether the last three objects should be taking the pointer to statObserver as their parameter instead of creating new instances

    //Attaching observers
    riskGame.attach(phaseObserver);
    riskGame.attach(statObserver);

    int decorators = 0;
    bool dominationDecorator = false;
    bool handsDecorator = false;
    bool continentDecorator = false;

    //Main game loop
    while(!playerWins)
    {
        //Adding and removing decorators based on user input
        while(decorators >= 1) {
            cout << "Do you wish to remove decorators from StatObserver?(y/n)" << endl;
            std::string answer;
            cin >> answer;

            if (answer == "n")
                break;

            answer.clear();

            if (dominationDecorator) {
                cout << "Domination Decorator?(y/n)" << endl;
                cin >> answer;
                if (answer == "y") {
                    decorators--;
                    riskGame.detach(statObserver);
                    dominationDecorator = false;
                    statObserver = new StatObserver(static_cast<Subject *>(&riskGame));
                    if (handsDecorator)
                        statObserver = new PlayerHandDecorator(statObserver);
                    if (continentDecorator)
                        statObserver = new ContinentDecorator(statObserver);
                    riskGame.attach(statObserver);
                }
                answer.clear();
            }
            if (handsDecorator) {
                cout << "Player Hand Decorator?(y/n)" << endl;
                cin >> answer;
                if (answer == "y") {
                    decorators--;
                    handsDecorator = false;
                    riskGame.detach(statObserver);
                    statObserver = new StatObserver(static_cast<Subject *>(&riskGame));
                    if (dominationDecorator)
                        statObserver = new DominationDecorator(statObserver);
                    if (continentDecorator)
                        statObserver = new ContinentDecorator(statObserver);
                    riskGame.attach(statObserver);
                }
                answer.clear();
            }
            if (continentDecorator) {
                cout << "Continents Decorator?(y/n)" << endl;
                cin >> answer;
                if (answer == "y") {
                    decorators--;
                    continentDecorator = false;
                    riskGame.detach(statObserver);
                    statObserver = new StatObserver(static_cast<Subject *>(&riskGame));
                    if (dominationDecorator)
                        statObserver = new DominationDecorator(statObserver);
                    if (handsDecorator)
                        statObserver = new PlayerHandDecorator(statObserver);
                    riskGame.attach(statObserver);
                }
            }
        }
        while(decorators < 3) {
            cout << "Do you wish to add decorators to StatObserver?(y/n)" << endl;
            std::string answer;
            cin >> answer;

            if (answer == "n")
                break;

            answer.clear();

            if (!dominationDecorator) {
                cout << "Domination Decorator?(y/n)" << endl;
                cin >> answer;
                if (answer == "y") {
                    decorators++; dominationDecorator = true;
                    riskGame.detach(statObserver);
                    statObserver = new DominationDecorator(statObserver);
                    riskGame.attach(statObserver);
                }
                answer.clear();
            }
            if (!handsDecorator) {
                cout << "Player Hand Decorator?(y/n)" << endl;
                cin >> answer;
                if (answer == "y") {
                    decorators++; handsDecorator = true;
                    riskGame.detach(statObserver);
                    statObserver = new PlayerHandDecorator(statObserver);
                    riskGame.attach(statObserver);
                }
                answer.clear();
            }
            if (!continentDecorator) {
                cout << "Continents Decorator?(y/n)" << endl;
                cin >> answer;
                if (answer == "y") {
                    decorators++; continentDecorator = true;
                    riskGame.detach(statObserver);
                    statObserver = new ContinentDecorator(statObserver);
                    riskGame.attach(statObserver);
                }
                answer.clear();
            }
        }

        riskGame.notify(NEW_TURN);
        for(int i = 0; i < players->size(); i++)
        {
            cout << "***************** " << players->at(i)->getName() << "'s turn *****************" << std::endl;
            //monitor current player
            riskGame.currentPlayer = players->at(i);
            // Each player gets to reinforce, attack and fortify
            std::vector<ReinforceResponse*>* reinforceResponse = (*players)[i]->reinforce(continents);

            if(reinforcementsMade(reinforceResponse)){
                riskGame.performReinforce(reinforceResponse);
                if(reinforceResponse->at(0)->exchangeOccured)
                    riskGame.notify(HAND_CHANGE);
                else
                    riskGame.notify(0);
            }
            delete reinforceResponse;

            bool conqueredTerritory = false;
            AttackResponse *attackResponse;
            do{
                attackResponse = players->at(i)->attack(players);
                if(attackResponse){
                    //Counting how many continents the attacker and defender have before the attack is done
                    int attackerCont = attackResponse->attacker->first->getsContinentsOwned(riskGame.getContinents())->size();
                    int defenderCont = attackResponse->defender->first->getsContinentsOwned(riskGame.getContinents())->size();

                    //Performing the attack
                    bool conquest = riskGame.performAttack(attackResponse);

                    //checks if the number of continents owned by either of the player has changed as a result of the attack
                    if(conquest) {
                        conqueredTerritory = true;
                        int attackerContAfter = attackResponse->attacker->first->getsContinentsOwned(riskGame.getContinents())->size();
                        int defenderContAfter = attackResponse->defender->first->getsContinentsOwned(riskGame.getContinents())->size();
                        if((attackerCont != attackerContAfter) || (defenderCont != defenderContAfter))
                            riskGame.notify(CONTINENT_CONTROL);
                        else
                            riskGame.notify(NEW_CONQUEST);
                    }
                    else
                        riskGame.notify(0);
                }
            }while(attackResponse);
            delete attackResponse;

            if(conqueredTerritory){
                players->at(i)->getHand()->draw(riskGame.getMainDeck()->draw());
                riskGame.notify(HAND_CHANGE);
            }

            FortifyResponse *fortifyResponse = players->at(i)->fortify(*riskGame.getMapCountries());
            if(fortifyResponse){
                riskGame.performFortify(fortifyResponse);
                riskGame.notify(0);
            }
            delete fortifyResponse;

            //After each player's turn, we check if one player owns all the countries in the map
            if(riskGame.currentPlayer->controlsAllCountriesInMap(*riskGame.getMapCountries()))
            {
                playerWins = true;
                riskGame.winningPlayer = riskGame.currentPlayer;
                break;
            }
        }
        //If we reach the max number of turns, the game is over with a draw
        if(riskGame.currentTurn++ == riskGame.max_turns)
        {
            playerWins = true;
            riskGame.winningPlayer = nullptr;
        }
    }
    cout << "===== GAME RESULTS =====" << endl;
    for(auto &node : *riskGame.getMapCountries()->getVectorOfNodes()){
        cout << *node << endl;
    }
    if(riskGame.winningPlayer)
        cout << riskGame.winningPlayer->getName() << " won the game of risk! Congratulations!!!" << endl;
    else {
        cout << "The game of risk reached the maximum number of turns (" << riskGame.max_turns << ")." << endl;
        cout << "It ended up with a DRAW." << endl;
    }
}

void part2(Game& riskGame)
{
    riskGame.determinePlayerTurn();
    //Determine player order and print them to check that the order changed (randomly)
    vector<Player*>* players = riskGame.getArrayPlayers();

    vector<Continent*> continents = *(riskGame.getContinents());

    riskGame.assignCountriesToPlayers();

    players = riskGame.getArrayPlayers();

    //Displaying all the continents in the graph
    for(int i = 0; i < continents.size(); i++)
    {
        cout << *(continents[i]);
    }

    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        players->at(i)->printNodes();
    }
    riskGame.placeArmiesAutomatic();

    //Boolean is false until a player wins. this is the breaking condition of the main game loop
    bool playerWins = false;

    riskGame.currentTurn = 1;

    //creating observers
    Observer *phaseObserver = new PhaseObserver(static_cast<Subject*>(&riskGame));
    Observer *statObserver = new StatObserver(static_cast<Subject*>(&riskGame));
    Observer *domObserver = new DominationDecorator(new StatObserver(static_cast<Subject*>(&riskGame)));
    Observer *handObserver = new PlayerHandDecorator(new StatObserver(static_cast<Subject*>(&riskGame)));
    Observer *continentObserver = new ContinentDecorator(new StatObserver(static_cast<Subject*>(&riskGame)));
    //Unsure whether the last three objects should be taking the pointer to statObserver as their parameter instead of creating new instances

    //Attaching observers
    riskGame.attach(phaseObserver);
    riskGame.attach(statObserver);
    riskGame.attach(domObserver);
    riskGame.attach(handObserver);
    riskGame.attach(continentObserver);

    //Main game loop
    while(!playerWins) {
        riskGame.cheaterPlayed = false;
        for (int i = 0; i < players->size(); i++) {
            riskGame.notify(NEW_TURN);
            cout << "***************** " << players->at(i)->getName() << "'s turn *****************" << std::endl;
            //monitor current player
            riskGame.currentPlayer = players->at(i);
            // Each player gets to reinforce, attack and fortify
            std::vector<ReinforceResponse *> *reinforceResponse = (*players)[i]->reinforce(continents);

            if (reinforcementsMade(reinforceResponse)) {
                riskGame.performReinforce(reinforceResponse);
                //TODO: Send the code HAND_CHANGE to notify() when the player's hand has changed as a result of reinforce
                riskGame.notify(0);
            }
            delete reinforceResponse;

            AttackResponse *attackResponse;
            do {
                if(riskGame.cheaterPlayed)
                {
                    attackResponse = nullptr;
                }
                else
                {
                    attackResponse = players->at(i)->attack(players);
                    if (attackResponse) {
                        //Counting how many continents the attacker and defender have before the attack is done
                        CheaterAttackResponse* actualResponse = dynamic_cast<CheaterAttackResponse*>(attackResponse);
                        int attackerCont = players->at(i)->getsContinentsOwned(riskGame.getContinents())->size();
                        int defenderCont = 0;

                        if(!actualResponse)
                        {
                            attackerCont = attackResponse->attacker->first->getsContinentsOwned(
                                    riskGame.getContinents())->size();
                            defenderCont = attackResponse->defender->first->getsContinentsOwned(
                                    riskGame.getContinents())->size();
                        }
                        else
                        {
                            riskGame.cheaterPlayed = true;
                        }

                        //Performing the attack
                        bool conquest = riskGame.performAttack(attackResponse);

                        //checks if the number of continents owned by either of the player has changed as a result of the attack
                        if (conquest) {

                            int attackerContAfter = players->at(i)->getsContinentsOwned(riskGame.getContinents())->size();
                            int defenderContAfter = 0;
                            if(actualResponse)
                            {
                                attackerCont = attackResponse->attacker->first->getsContinentsOwned(
                                        riskGame.getContinents())->size();
                                defenderCont = attackResponse->defender->first->getsContinentsOwned(
                                        riskGame.getContinents())->size();
                            }
                            if ((attackerCont != attackerContAfter) || (defenderCont != defenderContAfter))
                                riskGame.notify(CONTINENT_CONTROL);
                            else
                                riskGame.notify(NEW_CONQUEST);
                        } else
                            riskGame.notify(0);
                    }
                }
            } while (attackResponse);
            delete attackResponse;

            FortifyResponse *fortifyResponse = players->at(i)->fortify(*riskGame.getMapCountries());
            if (fortifyResponse) {
                riskGame.performFortify(fortifyResponse);
                riskGame.notify(0);
            }
            delete fortifyResponse;
        }
        //After each player's turn, we check if one player owns all the countries in the map
        //If we reach the max number of turns, the game is over with a draw
        if (riskGame.currentTurn++ == riskGame.max_turns) {
            playerWins = true;
            riskGame.winningPlayer = nullptr;
        }
    }
    cout << "\n\n===== GAME RESULTS =====" << endl;
    for(auto &node : *riskGame.getMapCountries()->getVectorOfNodes()){
        cout << *node << endl;
    }
    if(riskGame.winningPlayer)
        cout << riskGame.winningPlayer->getName() << " won the game of risk! Congratulations!!!" << endl;
    else {
        cout << "The game of risk reached the maximum number of turns (" << riskGame.max_turns << ")." << endl;
        cout << "It ended up with a DRAW." << endl << endl;
    }
    riskGame.reinitialize_game();
}

void mainGameLoopTournament(Game& riskGame)
{
    /*
    for (int i = 0; i < riskGame.getArrayPlayers()->size(); i++) {
        (*riskGame.getArrayPlayers())[i]->getNodes()->clear();
        cout << (*riskGame.getArrayPlayers())[i]->getNodes()->size() << endl;
    }*/
    riskGame.determinePlayerTurn();
    //Determine player order and print them to check that the order changed (randomly)
    vector<Player*>* players = riskGame.getArrayPlayers();

    vector<Continent*> continents = *(riskGame.getContinents());

    riskGame.assignCountriesToPlayers();

    players = riskGame.getArrayPlayers();

    //Displaying all the continents in the graph
    for(int i = 0; i < continents.size(); i++)
    {
        cout << *(continents[i]);
    }

    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        players->at(i)->printNodes();
    }
    riskGame.placeArmiesAutomatic();

    //Boolean is false until a player wins. this is the breaking condition of the main game loop
    bool playerWins = false;

    riskGame.currentTurn = 1;

    //creating observers
    Observer *phaseObserver = new PhaseObserver(static_cast<Subject*>(&riskGame));
    Observer *statObserver = new StatObserver(static_cast<Subject*>(&riskGame));
    Observer *domObserver = new DominationDecorator(new StatObserver(static_cast<Subject*>(&riskGame)));
    Observer *handObserver = new PlayerHandDecorator(new StatObserver(static_cast<Subject*>(&riskGame)));
    Observer *continentObserver = new ContinentDecorator(new StatObserver(static_cast<Subject*>(&riskGame)));
    //Unsure whether the last three objects should be taking the pointer to statObserver as their parameter instead of creating new instances

    //Attaching observers
    riskGame.attach(phaseObserver);
    riskGame.attach(statObserver);
    riskGame.attach(domObserver);
    riskGame.attach(handObserver);
    riskGame.attach(continentObserver);

    //Main game loop
    while(!playerWins) {
        riskGame.cheaterPlayed = false;
        for (int i = 0; i < players->size(); i++) {
            riskGame.notify(NEW_TURN);
            cout << "***************** " << players->at(i)->getName() << "'s turn *****************" << std::endl;
            //monitor current player
            riskGame.currentPlayer = players->at(i);
            // Each player gets to reinforce, attack and fortify
            std::vector<ReinforceResponse *> *reinforceResponse = (*players)[i]->reinforce(continents);

            if (reinforcementsMade(reinforceResponse)) {
                riskGame.performReinforce(reinforceResponse);
                //TODO: Send the code HAND_CHANGE to notify() when the player's hand has changed as a result of reinforce
                riskGame.notify(0);
            }
            delete reinforceResponse;

            AttackResponse *attackResponse;
            do {
                if(riskGame.cheaterPlayed)
                {
                    attackResponse = nullptr;
                }
                else
                {
                    attackResponse = players->at(i)->attack(players);
                    if (attackResponse) {
                        //Counting how many continents the attacker and defender have before the attack is done
                        CheaterAttackResponse* actualResponse = dynamic_cast<CheaterAttackResponse*>(attackResponse);
                        int attackerCont = players->at(i)->getsContinentsOwned(riskGame.getContinents())->size();
                        int defenderCont = 0;

                        if(!actualResponse)
                        {
                            attackerCont = attackResponse->attacker->first->getsContinentsOwned(
                                    riskGame.getContinents())->size();
                            defenderCont = attackResponse->defender->first->getsContinentsOwned(
                                    riskGame.getContinents())->size();
                        }
                        else
                        {
                            riskGame.cheaterPlayed = true;
                        }

                        //Performing the attack
                        bool conquest = riskGame.performAttack(attackResponse);

                        //checks if the number of continents owned by either of the player has changed as a result of the attack
                        if (conquest) {

                            int attackerContAfter = players->at(i)->getsContinentsOwned(riskGame.getContinents())->size();
                            int defenderContAfter = 0;
                            if(actualResponse)
                            {
                                attackerCont = attackResponse->attacker->first->getsContinentsOwned(
                                        riskGame.getContinents())->size();
                                defenderCont = attackResponse->defender->first->getsContinentsOwned(
                                        riskGame.getContinents())->size();
                            }
                            if ((attackerCont != attackerContAfter) || (defenderCont != defenderContAfter))
                                riskGame.notify(CONTINENT_CONTROL);
                            else
                                riskGame.notify(NEW_CONQUEST);
                        } else
                            riskGame.notify(0);
                    }
                }
            } while (attackResponse);
            delete attackResponse;

            FortifyResponse *fortifyResponse = players->at(i)->fortify(*riskGame.getMapCountries());
            if (fortifyResponse) {
                riskGame.performFortify(fortifyResponse);
                riskGame.notify(0);
            }
            delete fortifyResponse;
        }
        //After each player's turn, we check if one player owns all the countries in the map
        //If we reach the max number of turns, the game is over with a draw
        if (riskGame.currentTurn++ == riskGame.max_turns) {
            playerWins = true;
            riskGame.winningPlayer = nullptr;
        }
    }
    cout << "\n\n===== GAME RESULTS =====" << endl;
    for(auto &node : *riskGame.getMapCountries()->getVectorOfNodes()){
        cout << *node << endl;
    }
    if(riskGame.winningPlayer)
        cout << riskGame.winningPlayer->getName() << " won the game of risk! Congratulations!!!" << endl;
    else {
        cout << "The game of risk reached the maximum number of turns (" << riskGame.max_turns << ")." << endl;
        cout << "It ended up with a DRAW." << endl << endl;
    }
    riskGame.reinitialize_game();
}

static bool reinforcementsMade(std::vector<ReinforceResponse*>* responses)
{
    //If we can go through this loop, that means some reinforcements were made.
    for(auto &response : *responses)
    {
        return true;
    }
    return false;
}

void Game::performReinforce(std::vector<ReinforceResponse*>* responses)
{
    int tempTotal;
    std::vector<Node*> countriesReinforces;
    std::vector<int> armiesPlaced;
    for(auto &response : *responses) {
        tempTotal = response->country->getPointerToCountry()->getNbrArmies() + response->nbArmies;
        response->country->getPointerToCountry()->setNbrArmies(tempTotal);
        countriesReinforces.push_back(response->country);
        armiesPlaced.push_back(response->nbArmies);
    }
    if (!this->currentEvent)
    {
        delete this->currentEvent;
    }
    this->currentEvent = new ReinforceEvent(armiesPlaced, countriesReinforces);
}

/**
 * Helper method to perform attacking phase
 * returns true when a battle resulted in a new conquest for a player, and false otherwise
 */
bool Game::performAttack(AttackResponse* response) {
    if(!response)
        return true;

    CheaterAttackResponse* actualResponse = dynamic_cast<CheaterAttackResponse*>(response);
    if (actualResponse)
    {
        for (int i = 0; i < actualResponse->attackResponses->size(); ++i)
        {
            this->performAttack(actualResponse->attackResponses->at(i));
        }
    }
    else
    {
        bool victory = false;
        int rounds = 1;
        std::vector<int> *totalAttackerRolls = new std::vector<int>();
        std::vector<int> *totalDefenderRolls = new std::vector<int>();
        bool battleOver = false;
        Node* attackingCountry = response->attacker->second;
        Node* defendingCountry = response->defender->second;
        Player* attackingPlayer = response->attacker->first;
        Player* defendingPlayer = response->defender->first;

        if(attackingCountry->getPointerToCountry() == defendingCountry->getPointerToCountry())
            return false;

        while(attackingCountry->getPointerToCountry()->getNbrArmies() >= 2 && defendingCountry->getPointerToCountry()->getNbrArmies() > 0 && !battleOver){
            int attackerDice = attackingCountry->getPointerToCountry()->getNbrArmies() >= 4 ? 3 : attackingCountry->getPointerToCountry()->getNbrArmies() - 1;
            int defenderDice = defendingCountry->getPointerToCountry()->getNbrArmies() >= 2 ? 2 : 1;

            //Getting vectors of dice rolls
            std::vector<int> attackerDiceRolls = response->attacker->first->getDice()->howManyDice(attackerDice);
            std::vector<int> defenderDiceRolls = response->defender->first->getDice()->howManyDice(defenderDice);

            //Adding those dice rolls to the total dice rolls
            totalAttackerRolls->insert(std::end(*totalAttackerRolls), std::begin(attackerDiceRolls), std::end(attackerDiceRolls));
            totalDefenderRolls->insert(std::end(*totalDefenderRolls), std::begin(defenderDiceRolls), std::end(defenderDiceRolls));

            //Sorting the dice roll vectors in descending order
            std::sort(attackerDiceRolls.begin(), attackerDiceRolls.end(), std::greater<int>());
            std::sort(defenderDiceRolls.begin(), defenderDiceRolls.end(), std::greater<int>());

            //iterating through the dice rolls, until run our of descending dice
            for(int i = 0; i < std::min(defenderDiceRolls.size(), attackerDiceRolls.size()); i++){
                if(defenderDiceRolls[i] >= attackerDiceRolls[i]){
                    attackingCountry->getPointerToCountry()->setNbrArmies(attackingCountry->getPointerToCountry()->getNbrArmies() - 1);
                }
                else{
                    defendingCountry->getPointerToCountry()->setNbrArmies(defendingCountry->getPointerToCountry()->getNbrArmies() - 1);
                }
                if(defendingCountry->getPointerToCountry()->getNbrArmies() == 0){
                    victory = true;
                    battleOver = true;
                }
                else if(attackingCountry->getPointerToCountry()->getNbrArmies() == 1){
                    victory = false;
                    battleOver = true;
                }
            }
            rounds++;
        }

        int armiesMoved = 0;

        // Use to identify this attack from a cheater
        if (response->cheaterPhase)
        {
            victory = true;
        }

        if(victory){// If the attacker won, the country changes hands and he moves armies
            attackingPlayer->addNode(defendingCountry);
            defendingPlayer->removeNode(defendingCountry);

            armiesMoved = attackingCountry->getPointerToCountry()->getNbrArmies() - 1;

            attackingCountry->getPointerToCountry()->setNbrArmies(1);
            defendingCountry->getPointerToCountry()->setNbrArmies(armiesMoved);
        }

        if (!this->currentEvent)
        {
            delete this->currentEvent;
        }
        this->currentEvent = new AttackEvent(response->attacker->first, response->defender->first, response->attacker->second,
                                             response->defender->second, totalAttackerRolls, totalDefenderRolls, victory, armiesMoved);

        attackingCountry = nullptr;
        attackingPlayer = nullptr;
        defendingCountry = nullptr;
        defendingPlayer = nullptr;
        delete totalDefenderRolls;
        delete totalAttackerRolls;

        return victory; //returns true if victory false otherwise
    }
}

void Game::performFortify(FortifyResponse* response) {
    if (response->isCheater)
    {
        CheaterFortifyResponse* actualResponse = dynamic_cast<CheaterFortifyResponse*>(response);
        for (int i = 0; i < actualResponse->fortifyResponses->size(); ++i)
        {
            this->performFortify(actualResponse->fortifyResponses->at(i));
        }
    }
    else
    {
        //Apply changes
        string sourceStr = response->sourceCountry->getPointerToCountry()->getName();
        string destinationStr = response->destinationCountry->getPointerToCountry()->getName();
        response->sourceCountry->getPointerToCountry()->setNbrArmies(response->sourceCountry->getPointerToCountry()->getNbrArmies() - response->nbArmies);
        response->destinationCountry->getPointerToCountry()->setNbrArmies(response->destinationCountry->getPointerToCountry()->getNbrArmies() + response->nbArmies);

        //update currentEvent and return it
        if (!this->currentEvent)
        {
            delete this->currentEvent;
        }
        this->currentEvent = new FortifyEvent(response->nbArmies,response->sourceCountry,response->destinationCountry);
    }
}

Event* Game::getCurrentEvent(){
    return this->currentEvent;
}

int main()
{
    cout << "Choose a part to execute: " << endl;
    std::string answer;
    cin >> answer;

    if(answer == "1")
        part1();
    else if(answer == "2"){
        Parser* gameMap = new Parser("../maps/World.map");
        std::vector<Player*> players = std::vector<Player*>();
        for (int i = 0; i < 4; ++i)
        {
            players.push_back(new Player(to_string(i + 1)));
        }
        players.at(0)->setStrategy(new AggressiveStrategy);
        players.at(1)->setStrategy(new BenevolentStrategy);
        players.at(2)->setStrategy(new CheaterStrategy);
        players.at(3)->setStrategy(new RandomStrategy);

        Game* riskGame = new Game(gameMap, players, 50);
        part2(*riskGame);
    }
    else if(answer == "3"){
        //mainGameLoopDriver();
        Tournament t;

        t.setup_games();
        t.play_games();
        t.display_results();

        if(windows)
            system("pause");
        else {
            std::cout << "Press any key to continue . . ." << std::endl;
            std::getchar();
        }
    }
    else{
        cout << "Invalid choice." << endl;
    }
}