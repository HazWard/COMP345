#include "../include/game.h"
#include <dirent.h>
#include <iostream>
#include<list>
#include <random>
#include <algorithm>
#include <ctime>
#include <chrono>

using namespace std;

const string MAPS_FOLDER = "../maps/";

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
    while ((entry = readdir(directory)) != nullptr)
    {
        string currentFileName = entry->d_name;
        if(currentFileName != "." && currentFileName != "..") {
            listOfMapFiles.push_back(currentFileName);
        }
    }
    closedir(directory);
    return listOfMapFiles;
}
void Game::getMapUser(list<string> listOfMapFiles)
{
    cout << "Here is the list of available map files. Choose a map by entering the number associating with the one you want." << endl;
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
    Parser* parse1;
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
            parse1 = new Parser(MAPS_FOLDER + mapName);
            cout << "Is parse1 a valid map ? : ";
            if (parse1->mapIsValid()) {
                cout << "Yes, both the entire map as a whole and each continent are connected.\n";
                validIndexMap = true;
            }
            else {
                cout << "No, the graph and/or some of the continents are not strongly connected.\n";
                validIndexMap = false;
            }
        }
    } while(!validIndexMap);
    this->mapCountries = *parse1->getGraph();
    this->continents = *parse1->getContinents();
    delete parse1;
}
int Game::getNbrPlayersUser()
{
    int nbrPlayers;
    do {
        cout << "How many players are playing the game? (2-6 players)";
        cin >> nbrPlayers;
        if(nbrPlayers < 2 || nbrPlayers > 6)
            cout << "Error: Invalid amount of players (only from 2 to 6)" << endl;
    } while(nbrPlayers < 2 || nbrPlayers > 6);
}
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
Game::Game()
{
    list<string> mapFiles = getNameOfFiles("..\\maps");
	getMapUser(mapFiles);
    this->nbrPlayers = getNbrPlayersUser();
    this->arrayPlayers = *(getPlayersUser(nbrPlayers));
    this->mainDeck = Deck(mapCountries.getNbrCountries());
    if(nbrPlayers != arrayPlayers.size())
    {
        cout << "The number of players (" << nbrPlayers << " and "
             << "the number of players creater (" + arrayPlayers.size()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
    if(mainDeck.getNumberOfCards() != mapCountries.getNbrCountries())
    {
        cout << "The number of cards (" << mainDeck.getNumberOfCards() << " and "
             << "the number of nodes in the map (" + mapCountries.getNbrCountries()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
}
//ACCESSOR METHODS
string Game::getMapName() { return mapName; }

int Game::getNbrPlayers() { return nbrPlayers; }

vector<Player*>* Game::getArrayPlayers() { return &arrayPlayers; }

Graph Game::getMapCountries() { return mapCountries; }

Deck Game::getMainDeck() { return mainDeck; }

map<string, Graph>* Game::getContinents() { return &continents; };

void Game::determinePlayerTurn() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (arrayPlayers.begin(), arrayPlayers.end(), std::default_random_engine(seed));
    /*
    srand ( unsigned ( std::time(0) ) );
    random_shuffle ( arrayPlayers.begin(), arrayPlayers.end() );
     */
}
/*
 * //Old Player Turn:
vector<Player*> oldPlayerOrder = arrayPlayers;
arrayPlayers.clear();
vector<int> indicesOrder = vector<int>();

random_device rd;
srand(rd());
bool newIndex;
for(int i = 0; i < nbrPlayers; i++) {
    do {
        newIndex = true;
        int indexPlayer = rand() % (nbrPlayers); //indexPlayer in the range 0 to nbrPlayers-1
        for (int j = 0; j < indicesOrder.size(); j++) {
            if (indexPlayer == indicesOrder[j]) {
                newIndex = false;
                break;
            }
        }
        if (newIndex)
            indicesOrder.push_back(indexPlayer);
    } while (indicesOrder.size() != nbrPlayers);
}
for(int j = 0; j < nbrPlayers; j++) {
    arrayPlayers.push_back(oldPlayerOrder[indicesOrder[j]]);
    cout << arrayPlayers[j]->getName();
}
 */
//The following method has been provided by:
//http://rextester.com/HHTW39678
//http://www.cplusplus.com/forum/general/207328/
template <class T > void listShuffle( list<T> &L )
{
    mt19937 gen( chrono::system_clock::now().time_since_epoch().count() );
    vector<T> V( L.begin(), L.end() );
    shuffle( V.begin(), V.end(), gen );
    L.assign( V.begin(), V.end() );
}

void Game::assignCountriesToPlayers()
{
    vector<Node>* listOfNodes = this->mapCountries.getVectorOfNodes();
    list<Node*> nodesToAssign;
    for(int i = 0; i < listOfNodes->size(); i++)
        nodesToAssign.push_back(&(*listOfNodes)[i]);

    while(!nodesToAssign.empty()) {
            for (int i = 0; i < this->arrayPlayers.size(); i++) {
                listShuffle(nodesToAssign);
                if(nodesToAssign.front() != NULL) {
                    arrayPlayers[i]->addNode(nodesToAssign.front());
                    nodesToAssign.pop_front();
                }
            }
        }
}

void Game::placeArmies()
{
    int nbrArmiesPerPlayer = 0;
    switch(nbrPlayers)
    {
        case 2: nbrArmiesPerPlayer = 40; break;
        case 3: nbrArmiesPerPlayer = 35; break;
        case 4: nbrArmiesPerPlayer = 30; break;
        case 5: nbrArmiesPerPlayer = 25; break;
        case 6: nbrArmiesPerPlayer = 20; break;
    }
    for(int i = 0; i < nbrPlayers; i++)
    {
        int nbrArmiesPlayer = nbrArmiesPerPlayer;
        while(nbrArmiesPlayer > 0) {
            bool validCountryName = false;
            Country *c;
            do {
                cout << arrayPlayers[i]->getName() << " has " << nbrArmiesPlayer
                     << " armies to place. Please choose the name of the country you want to place some armies on."
                     << endl;
                int k = 1;

                for (auto const &node : arrayPlayers[i]->getNodes()) {
                    cout << k << ": " << node->getCountry().getName() << endl;
                    k++;
                }
                string chosenCountry;
                getline(cin, chosenCountry);

                for (auto const &node2 : arrayPlayers[i]->getNodes()) {
                    string name = node2->getCountry().getName();
                    if (name == chosenCountry) {
                        c = node2->getPointerToCountry();
                        validCountryName = true;
                        break;
                    }
                }
                if(!validCountryName)
                    cout << "You did not enter a valid country name from the list. Please make sure to enter it properly." << endl;
            } while(!validCountryName);
            int nbrArmiesToPut = 0;
            cout << "Please enter the number of armies to put on country " << c->getName() << ": ";
            cin >> nbrArmiesToPut;
            cin.ignore();

            if(nbrArmiesPlayer >= nbrArmiesToPut && nbrArmiesToPut >= 0) {
                nbrArmiesPlayer -= nbrArmiesToPut;
                c->setNbrArmies(c->getNbrArmies() + nbrArmiesToPut);
            } else cout << "There is not enough armies available to player " << arrayPlayers[i]->getName() << ", we cannot do this." << endl;
            if(nbrArmiesPlayer == 0) {
                cout << arrayPlayers[i]->getName()
                     << " has successfully placed all of their armies. We will go to the next player." << endl;
            }
        }
    }
    if(verifyPlayerArmiers(nbrArmiesPerPlayer))
        cout << endl << "All players have successfully placed their armies.\n";
    else {
        cout << "There was an error: Each player has not placed " << nbrArmiesPerPlayer << " armies.\n";
        exit(EXIT_FAILURE);
    }
}
bool Game::verifyPlayerArmiers(int nbrArmiesPerPlayer)
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

//Main for Part 2
int main()
{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
    Game riskGame;
    //Determine player order and print them to check that the order changed (randomly)
    vector<Player*>* players = riskGame.getArrayPlayers();

    map<string, Graph>* cont = riskGame.getContinents();

    map<string, Graph>::reverse_iterator rit;
    for (rit = (*cont).rbegin(); rit != (*cont).rend(); ++rit)
    {
        cout << rit->second;
    }
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
    for(int i = 0; i < riskGame.getNbrPlayers(); i++)
    {
        play[i]->printNodes();
    }
    riskGame.placeArmies();

    delete players;
    delete cont;

    return 0;
}
//Main for Part 1
//int main()
//{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
//    Game riskGame;
//}
