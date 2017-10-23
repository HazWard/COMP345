#include "../include/game.h"
#include <dirent.h>
#include <iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>
#include <vector>
#include <map>
#include<list>
#include <Windows.h>

using namespace std;

const string MAPS_FOLDER = "../maps/";

Game::Game(): mapName(""), nbrPlayers(0), arrayPlayers(vector<Player*>()), map()
{ }
Game::Game(string mapName, int np, vector<Player*> pl, Graph m): mapName(mapName), nbrPlayers(np),
                                                                 arrayPlayers(pl), map(m)
{ }

void Game::setMap(Graph newMap)
{
    map = newMap;
}
void Game::setNbrPlayers(int nbrP)
{
    nbrPlayers = nbrP;
}
void Game::setArrayPlayers(vector<Player*> newArrayPl)
{

}

//Function to read all files from a given folder taken from:
//https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
static list<string> getNameOfFiles(const char *path)
{
    list<string> listOfMapFiles;
    struct dirent *entry;
    DIR *directory = opendir(path);
    //If we cannot open the directory, we simply return an empty list of names
    if (directory == NULL) 
    {
        return list<string>();
    }
    while ((entry = readdir(directory)) != NULL) 
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
    Game currentGame;
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
    while(!validIndexMap)
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
            Parser parse1(MAPS_FOLDER + *mapName);
            cout << "Is parse1 a valid map ? : ";
            if (parse1.mapIsValid()) {
                cout << "Yes, both the entire map as a whole and each continent are connected.\n";
                validIndexMap = true;
                return parse1.getGraph();
            }
            else {
                cout << "No, the graph and/or some of the continents are not strongly connected.\n";
                validIndexMap = false;
            }
        }
    }
}
static int getNbrPlayers()
{
    int nbrPlayers;
    while(nbrPlayers < 0 || nbrPlayers > 7) {
        cout << "How many players are playing the game? (2-6 players)";
        cin >> nbrPlayers;
        if(nbrPlayers < 0 || nbrPlayers > 7)
            cout << "Error: Invalid amount of players (only from 2 to 6)" << endl;
    }
}
static vector<Player*>* getPlayers(int np)
{
    vector<Player*>* pl;
    pl->reserve(np);
    for(int i = 0; i < np; i++)
    {
        pl->push_back(new Player());
    }
}
static Game beingGame(list<string> listOfMapFiles)
{
    string mapName;
    Graph mapOfTheGame = *getMap(&mapName, listOfMapFiles);
    int nbrPlayers = getNbrPlayers();
    vector<Player*> players = *(getPlayers(nbrPlayers));
    return Game(mapName, nbrPlayers, players, mapOfTheGame);
}

int main()
{
    list<string> mapFiles = getNameOfFiles("..\\maps");
    Game riskGame = beingGame(mapFiles);
}