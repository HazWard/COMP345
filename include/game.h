#pragma once
#include "../include/reader.h"
#include "../include/player.h"

bool windows = false;
#ifdef OS_WINDOW1S
    windows = true;
#endif

class Game {
private:
    //private members:
    string mapName;
    int nbrPlayers;
    vector<Player *> arrayPlayers;
    Graph mapCountries;
    //TO DELETE map<string, Graph> continents;
    vector<Continent*> continents;
    Deck mainDeck;

    //Helper methods:
    list<string> getNameOfFiles(const char *path);

    void getMapUser(list<string> listOfMapFiles);

    int getNbrPlayersUser();

    vector<Player *> *getPlayersUser(int np);

    bool verifyPlayerArmiers(int nbrArmiesPerPlayer);

public:
    //Constructor:
    Game();
    //Destructor:
    //~Game();
    //Mutator methods:
    void setMap(Graph &newMap);

    void setNbrPlayers(int nbrP);

    void setArrayPlayers(vector<Player *> &);

    //Accessor methods:
    string getMapName();

    int getNbrPlayers();
    vector<Player*>* getArrayPlayers();
    Graph* getMapCountries();
    Deck getMainDeck();

    //TO DELETE map<string, Graph> *getContinents();
    vector<Continent*>* getContinents();


    //Public methods:
    void determinePlayerTurn();

    void assignCountriesToPlayers();

    void placeArmies();
};

