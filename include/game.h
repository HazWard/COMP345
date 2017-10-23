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
private:
    string mapName;
    int nbrPlayers;
    vector<Player*> arrayPlayers;
    Graph mapCountries;
    Deck mainDeck;
};

