#include "../include/game.h"
#include <dirent.h>
#include <iostream>
#include<list>
#include <Windows.h>
/*
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
static Graph* getMap(string* mapName, list<string> listOfMapFiles)
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
static int getNbrPlayers()
{
    int nbrPlayers;
    do {
        cout << "How many players are playing the game? (2-6 players)";
        cin >> nbrPlayers;
        if(nbrPlayers < 0 || nbrPlayers > 7)
            cout << "Error: Invalid amount of players (only from 2 to 6)" << endl;
    } while(nbrPlayers < 0 || nbrPlayers > 7);
}
static vector<Player*>* getPlayers(int np)
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
    this->mapCountries = *getMap(&this->mapName, mapFiles);
    this->nbrPlayers = getNbrPlayers();
    this->arrayPlayers = *(getPlayers(nbrPlayers));
    if(nbrPlayers != arrayPlayers.size())
    {
        cout << "The number of players (" << nbrPlayers << " and "
             << "the number of players creater (" + arrayPlayers.size()
             << "is not equivalent. We will exit the program." << endl;
        exit (EXIT_FAILURE);
    }
}

//Main for Part 1
int main()
{
    /*The constructor verifies that the map loaded is valid.
    Invalid maps are rejected without the program crashing.
    Also, we check that the right number of players is created inside the constructor as well.*/
    Game riskGame;
}