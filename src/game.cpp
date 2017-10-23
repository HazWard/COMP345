#include "../include/game.h"
#include <dirent.h>
#include <iostream>
#include<list>
#include <Windows.h>
#include <random>
#include <algorithm>

/*
#include <cstdlib>
#include<string.h>
#include<fstream>
#include<dirent.h>
#include <vector>
#include <map>*/

using namespace std;

const string MAPS_FOLDER = "../maps/";

/*
Game::Game(): mapName(""), nbrPlayers(0), arrayPlayers(vector<Player*>()), mapCountries()
{ }

Game::Game(string mapName, int np, vector<Player*> pl, Graph m): mapName(mapName), nbrPlayers(np),
                                                                 arrayPlayers(pl), mapCountries(m)
{ }*/

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
static list<string> getNameOfFiles(const char *path)
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
static Graph* getMapUser(string* mapName, list<string> listOfMapFiles)
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
            *mapName = *it;
            cout << "You chose the map " << *mapName << endl;
            cout << "We will check if that map is a valid one." << endl;
            parse1 = new Parser(MAPS_FOLDER + *mapName);
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
    Graph *mapC = parse1->getGraph();
    delete parse1;
    return mapC;
}
static int getNbrPlayersUser()
{
    int nbrPlayers;
    do {
        cout << "How many players are playing the game? (2-6 players)";
        cin >> nbrPlayers;
        if(nbrPlayers < 0 || nbrPlayers > 7)
            cout << "Error: Invalid amount of players (only from 2 to 6)" << endl;
    } while(nbrPlayers < 0 || nbrPlayers > 7);
}
static vector<Player*>* getPlayersUser(int np)
{
    vector<Player*>* pl = new vector<Player*>;
    pl->reserve(np);
    for(int i = 0; i < np; i++)
    {
        pl->push_back(new Player());
    }
    return pl;
}
Game::Game()
{
    list<string> mapFiles = getNameOfFiles("..\\maps");
    this->mapCountries = *getMapUser(&this->mapName, mapFiles);
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
             << "the number of countries in the map (" + mapCountries.getNbrCountries()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
}
//ACCESSOR METHODS
string Game::getMapName() { return mapName; }

int Game::getNbrPlayers() { return nbrPlayers; }

vector<Player*> Game::getArrayPlayers() { return arrayPlayers; }

Graph Game::getMapCountries() { return mapCountries; }

Deck Game::getMainDeck() { return mainDeck; }

void Game::determinePlayerTurn() {
    vector<Player*> oldPlayerOrder = arrayPlayers;
    arrayPlayers.clear();
    vector<int> indicesOrder = vector<int>();
    for(int i = 0; i < nbrPlayers; i++)
    {
        bool newIndex = false;
        while(!newIndex) {
            random_device rd;
            srand(rd());
            int indexPlayer = rand() % (nbrPlayers - 1); //indexPlayer in the range 0 to nbrPlayers-1
            vector<int>::iterator it;
            it = find(indicesOrder.begin(), indicesOrder.end(), indexPlayer);
            if (it != indicesOrder.end()) {
                indicesOrder.push_back(indexPlayer);
                newIndex = true;
            }
            else newIndex = false;
        }
        arrayPlayers.push_back(oldPlayerOrder[newIndex]);
    }
}
/*
void Game::assignCountriesToPlayers()
{
    vector<Node>* listOfNodes = this->mapCountries.getVectorOfNodes();
    list<Node*> countriesToAssign;
    for(int i = 0; i < listOfNodes->size(); i++)
    {
        countriesToAssign.push_back(&(*listOfNodes)[i]);
    }
    for(int nbrCountriesAssigned = 0; nbrCountriesAssigned < countriesToAssign.size(); nbrCountriesAssigned++)
    {
        for(int i = 0; i < this->arrayPlayers.size(); i++)
        {
            int remainingNbrCountries = countriesToAssign.size();
            random_device rd;
            srand(rd());
            int indexRandCountry = rand() % (remainingNbrCountries - 1); //indexPlayer in the range 0 to nbrPlayers-1
            this->arrayPlayers[i]->addCountry();
        }
    }
}
*/
//Main for Part 2
int main()
{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
    Game riskGame;
    /*
    //Determine player order and print them to check that the order changed (randomly)
    vector<Player*> players = riskGame.getArrayPlayers();
    for(int i = 0; i < players.size(); i++)
    {
        cout << *players[i]. << endl;
    }
    riskGame.determinePlayerTurn();
    for(int i = 0; i < players.size(); i++)
    {
        cout << *players[i] << endl;
    }*/

}
//Main for Part 1
//int main()
//{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
//    Game riskGame;
//}
