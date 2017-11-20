
#ifndef COMP345_TOURNAMENT_H
#define COMP345_TOURNAMENT_H
#include "../include/game.h"
#include <iostream>


class Tournament
{
public:
    const int MAX_MAPS = 5;
    const int MIN_PLAYERS = 2;
    const int MAX_PLAYERS = 4;
    const int MAX_GAMES_PER_MAP = 5;
    const int LOWER_MAX_TURNS_PER_GAME = 10;
    const int UPPER_MAX_TURNS_PER_GAME = 50;

    vector<Parser*> M;
    vector<Player*> P;
    int G;
    int D;

    map<Graph, vector<Game*>> games;

    Tournament();
};

#endif //COMP345_TOURNAMENT_H
