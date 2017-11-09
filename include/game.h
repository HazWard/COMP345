#pragma once
#include "../include/reader.h"
#include "../include/player.h"
#include "../include/observer.h"

bool windows = false;
#ifdef OS_WINDOW1S
    windows = true;
#endif

class Game : public Subject {
private:
    //constants:
    const string DEFAULT_MAP_NAME = "World.map";
    const int DEFAULT_NUMBER_PLAYERS = 4;
    string DEFAULT_PLAYER_NAMES[6] = {"player A", "player B", "player C", "player D", "player E", "player F"};

    //private members:
    string mapName;
    int nbrPlayers;
    vector<Player *> arrayPlayers;
    Graph* mapCountries;
    vector<Continent*> continents;
    Deck mainDeck;
    //ReinforceEvent reinforceEvent;
    //AttackEvent attackEvent;
    //FortifyEvent fortifyEvent;

    //Helper methods:
    list<string> getNameOfFiles(const char *path);
    void getMapUser(list<string> listOfMapFiles);
    void getMapAutomatic();
    int getNbrPlayersUser();
    vector<Player *>* getPlayersUser(int np);
    vector<Player *>* getPlayersAutomatic(int np);
    bool verifyPlayerArmies(int nbrArmiesPerPlayer);
    bool armiesLeftToPlace(vector<int> nbrArmiesPlayers);
public:
    //Constructor:
    Game();
    //Mutator methods:
    void setMap(Graph* newMap);
    void setNbrPlayers(int nbrP);
    void setArrayPlayers(vector<Player *> &);
    //Accessor methods:
    string getMapName();
    int getNbrPlayers();
    vector<Player*>* getArrayPlayers();
    Graph* getMapCountries();
    Deck getMainDeck();
    vector<Continent*>* getContinents();
    //Public methods:
    void determinePlayerTurn();
    void assignCountriesToPlayers();
    void placeArmies();
    void performReinforce(std::vector<ReinforceResponse*>* responses);
    bool performAttack(AttackResponse *response);
    bool performFortify(FortifyResponse* response);
    void placeArmiesAutomatic();
};
