#pragma once
#include "../include/reader.h"
#include "../include/player.h"
#include "../include/observer.h"
#include "../include/events.h"
#include <chrono>

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

    //Helper methods:
    void getMapUser(list<string> listOfMapFiles);
    void getMapAutomatic();
    int getNbrPlayersUser();
    vector<Player *>* getPlayersUser(int np);
    vector<Player *>* getPlayersAutomatic(int np);
    Event* getCurrentEvent();
    bool verifyPlayerArmies(int nbrArmiesPerPlayer);
    bool armiesLeftToPlace(vector<int> nbrArmiesPlayers);
public:
    //public members:
    Player* winningPlayer;
    int max_turns;
    //public constant:
    const int DEFAULT_MAX_TURNS = 50;
    //Constructor:
    Game();
    Game(Parser* map, vector<Player*> pl, int maximum_turns);
    //Mutator methods:
    void setMap(Graph* newMap);
    void setNbrPlayers(int nbrP);
    void setArrayPlayers(vector<Player *> &);
    //Accessor methods:
    string getMapName();
    int getNbrPlayers();
    vector<Player*>* getArrayPlayers();
    Graph* getMapCountries();
    Deck* getMainDeck();
    vector<Continent*>* getContinents();
    //Public methods:
    void determinePlayerTurn();
    void assignCountriesToPlayers();
    void placeArmies();
    void performReinforce(std::vector<ReinforceResponse*>* responses);
    bool performAttack(AttackResponse *response);
    void performFortify(Player* player,FortifyResponse* response);
    void placeArmiesAutomatic();
    Event* currentEvent;
    vector <Player*> players;
    int currentTurn;
    Player* currentPlayer;
    void chooseGameScenario(vector<Player*>* players);
    void reinitialize_game();
};

const string MAPS_FOLDER = "../maps/";
list<string> getNameOfFiles(const char *path);
void mainGameLoopDriver();
void mainGameLoopTournament(Game& riskGame);