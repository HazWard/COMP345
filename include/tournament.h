
#ifndef COMP345_TOURNAMENT_H
#define COMP345_TOURNAMENT_H
#include "../include/game.h"
#include <iostream>


class Tournament
{
private:
    bool check_validity_M(string M_in_string_form);
    bool check_validity_P(string P_in_string_form);
    bool check_validity_G();
    bool check_validity_D();
public:
    const int MAX_MAPS = 5;
    const int MIN_PLAYERS = 2;
    const int MAX_PLAYERS = 4;
    const int MAX_GAMES_PER_MAP = 5;
    const int LOWER_MAX_TURNS_PER_GAME = 10;
    const int UPPER_MAX_TURNS_PER_GAME = 50;

    vector<Parser*> M;
    vector<Player*> P;
    size_t G;
    size_t D;

    map<Graph*, vector<Game*>*> games;

    Tournament();
    void setup_games();
    void play_games();
    void display_results();
};

#endif //COMP345_TOURNAMENT_H
