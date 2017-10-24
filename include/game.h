#include "../include/reader.h"
#include "../include/player.h"
#pragma once

class Game
{
public:
    Game();
    void setMap(Graph& newMap);
    void setNbrPlayers(int nbrP);
    void setArrayPlayers(vector<Player*>&);
    void determinePlayerTurn();
    void assignCountriesToPlayers();
    string getMapName();
    int getNbrPlayers();
    vector<Player*>* getArrayPlayers();
    Graph getMapCountries();
    Deck getMainDeck();
private:
    string mapName;
    int nbrPlayers;
    vector<Player*> arrayPlayers;
    Graph mapCountries;
    Deck mainDeck;
};

