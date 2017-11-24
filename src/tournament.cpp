#include "../include/tournament.h"
bool wind = false;
#ifdef OS_WINDOW1S
wind = true;
#endif

static string trim_dots(const string& str)
{
    size_t first = str.find_first_not_of('.');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of('.');
    return str.substr(first, (last - first + 1));
}

//Checks the validity of the maps entered by the user
bool Tournament::check_validity_M(string M_in_string_form)
{
    bool valid_M = false;
    map_names = split(M_in_string_form, ',');
    //Allocating/reserving an array of size equal to the number of file names (since it is technically the number of maps)
    M.reserve(map_names.size());

    for (int i = 0; i < map_names.size(); i++) {
        map_names[i] = trim_dots(trim(map_names[i]));
        Parser *parser;
        cout << "Loading map located at " << MAPS_FOLDER << map_names[i] << endl;

        parser = new Parser(MAPS_FOLDER + map_names[i]);

        if (parser->mapIsValid() && parser->getGraph()->getNbrCountries() <= 80) {
            cout << "The map " << map_names[i] << " is valid.\n\n";
            valid_M = true;
        } else {
            cout << "We have detected that the map " << map_names[i] << " is invalid." << endl;
            cout << "Please enter a valid M." << endl;
            M.clear();
            return (valid_M = false);
        }
        M.push_back(parser);
    }
    if(M.size() < 1 || M.size() > MAX_GAMES_PER_MAP)
    {
        std::cout << "You entered no maps to be used, some invalid maps or you entered more than " << MAX_GAMES_PER_MAP << " maps." << std::endl;
        M.clear();
        return (valid_M = false);
    }
    else return valid_M;
}

//Checks the validity of the types of players entered by the user
bool Tournament::check_validity_P(string P_in_string_form)
{
    vector<string> line_data_players = split(P_in_string_form, ',');
    //Allocating/reserving an array of size equal to the number of file names (since it is technically the number of maps)
    //P = vector<Player*>(line_data_players.size());
    P.reserve(line_data_players.size());

    int number_aggressive_player = 1;
    int number_benevolent_player = 1;
    int number_random_player = 1;
    int number_cheater_player = 1;

    for(int i = 0; i < line_data_players.size(); i++)
    {
        line_data_players[i] = trim_dots(trim(tolower(line_data_players[i])));
        if(line_data_players[i] == "aggressive") {
            for(int j = 0; j < P.size(); j++) {
                if(P[j]->getName().find("Aggressive") != std::string::npos) {
                    number_aggressive_player++;
                    break;
                }
            }
            P.push_back(new Player("Aggressive " + std::to_string(number_aggressive_player), new AggressiveStrategy));
        }
        else if(line_data_players[i] == "benevolent") {
            for(int j = 0; j < P.size(); j++) {
                if(P[j]->getName().find("Benevolent") != std::string::npos) {
                    number_benevolent_player++;
                    break;
                }
            }
            P.push_back(new Player("Benevolent " + std::to_string(number_benevolent_player), new BenevolentStrategy));
        }
        else if(line_data_players[i] == "random")
        {
            for(int j = 0; j < P.size(); j++) {
                if(P[j]->getName().find("Random") != std::string::npos) {
                    number_random_player++;
                    break;
                }
            }
            P.push_back(new Player("Random " + std::to_string(number_random_player), new RandomStrategy));
        }
        else if(line_data_players[i] == "cheater")
        {
            for(int j = 0; j < P.size(); j++) {
                if(P[j]->getName().find("Cheater") != std::string::npos) {
                    number_cheater_player++;
                    break;
                }
            }
            P.push_back(new Player("Cheater " + std::to_string(number_cheater_player), new CheaterStrategy));
        } else
        {
            std::cout << line_data_players[i] << " is not a valid type of player. Please enter a proper P." << std::endl;
            P.clear();
            return false;
        }
    }
    if(P.size() < MIN_PLAYERS || P.size() > MAX_PLAYERS)
    {
        std::cout << "You entered less than " << MIN_PLAYERS << " or more than " << MAX_PLAYERS << " players." << std::endl;
        P.clear();
        return false;
    }
    else if(P.size() == line_data_players.size()) {
        cout << "The player types entered are valid. Here they are:" << endl;
        //Testing the type of strategy of each player:
        for(int i = 0; i < P.size(); i++)
        {
            cout << "\t" << P[i]->getName() << ": ";

            Strategy::StrategyType strategyType = P[i]->getStrategy()->getType();
            switch(strategyType)
            {
                case Strategy::ABSTRACT  : std::cout << "Abstract Strategy\n";   break;
                case Strategy::AGGRESSIVE: std::cout << "Aggressive Strategy\n";   break;
                case Strategy::BENEVOLENT: std::cout << "Benevolent Strategy\n";   break;
                case Strategy::HUMAN: std::cout << "Human Strategy\n";   break;
                case Strategy::CHEATER  : std::cout << "Cheater Strategy\n";   break;
                case Strategy::RANDOM: std::cout << "Random Strategy\n";   break;
            }
        }
        return true;
    }
    else {
        P.clear();
        return false;
    }
}
bool Tournament::check_validity_G()
{
    if(G < 1 || G > MAX_GAMES_PER_MAP)
    {
        std::cout << "You entered an invalid number of games to be played on each map." << std::endl;
        std::cout << "Please enter a proper G from 1 to " << MAX_GAMES_PER_MAP << "." << std::endl;
        return false;
    }
    else
        return true;
}
bool Tournament::check_validity_D()
{
    if(D < LOWER_MAX_TURNS_PER_GAME || D > UPPER_MAX_TURNS_PER_GAME)
    {
        std::cout << "You entered an invalid maximum number of turns for each game." << std::endl;
        std::cout << "Please enter a proper D from " << LOWER_MAX_TURNS_PER_GAME << " to " << UPPER_MAX_TURNS_PER_GAME << "." << std::endl;
        return false;
    }
    else
        return true;
}

Tournament::Tournament() {
    list<string> map_files_names;
    //Checking the operating system by using the windows boolean, determines how paths are set
    if(wind){ map_files_names = getNameOfFiles("..\\maps"); }
    else { map_files_names = getNameOfFiles("../maps"); }

    std::cout << "***************RISK TOURNAMENT***************" << std::endl;
    std::cout << "Here is the list of map files you can select from:" << std::endl;
    list<string>::const_iterator iterator;
    for (iterator = map_files_names.begin(); iterator != map_files_names.end(); ++iterator)
    {
        std::cout << "\t" << *iterator << std::endl;
    }
    std::cout << "You can select from 1 to " << MAX_MAPS << " maps from the list above." << std::endl;
    std::cout << "You can select from " << MIN_PLAYERS << " to " << MAX_PLAYERS << " different computer players strategies." << std::endl;
    std::cout << "You can select from 1 to " << MAX_GAMES_PER_MAP << " games to be played on each map." << std::endl;
    std::cout << "You can select the maximum number of turns per game from " << LOWER_MAX_TURNS_PER_GAME << " to " << UPPER_MAX_TURNS_PER_GAME << "." << std::endl;
    std::cout << "\t(Note: this is to minimize run completion time.)" << std::endl << std::endl;

    bool valid_M = false;
    bool valid_P = false;
    bool valid_G = false;
    bool valid_D = false;

    do {
        std::cout << "Enter the information according to the following format:" << std::endl;
        std::cout << "\tM: Map1, Map2, Map3" << std::endl;
        std::cout << "\tP: Aggressive, Benevolent, Random, Cheater." << std::endl;
        std::cout << "\tG: 4" << std::endl;
        std::cout << "\tD: 30" << std::endl << std::endl;

        string M_in_string_form;
        string P_in_string_form;

        valid_M = false;
        valid_P = false;
        valid_G = false;
        valid_D = false;

        /*
        cin.clear();
        std::cout << "M: ";
        std::getline(std::cin, M_in_string_form);
        std::cout << "P: ";
        std::getline(std::cin, P_in_string_form);
        std::cout << "G: ";
        cin >> G;
        cin.ignore();
        std::cout << "D: ";
        cin >> D;
        cin.ignore();
         */
        //Used for debugging: (makes everything faster)
        M_in_string_form = "World.map, New York.map, USA.map";
        P_in_string_form = "Aggressive, Aggressive, Benevolent, Cheater";
        G = 5;
        D = 20;

        valid_M = check_validity_M(M_in_string_form);
        if(!valid_M)
            continue;
        valid_P = check_validity_P(P_in_string_form);
        if(!valid_P)
            continue;
        valid_G = check_validity_G();
        if(!valid_G)
            continue;
        valid_D = check_validity_D();
        if(!valid_D)
            continue;
    }   while(!valid_M || !valid_P || !valid_G || !valid_D);

}

void Tournament::setup_games()
{
    for(int i = 0; i < M.size(); i++)
    {
        vector<Game*>* games_on_current_map = new vector<Game*>();
        games_on_current_map->reserve(G);
        for(int j = 0; j < G; j++)
        {
            (*games_on_current_map).push_back(new Game(M[i], P, D));
        }
        Graph* current_graph = M[i]->getGraph();
        games.insert(std::pair<Graph*, vector<Game*>*>(current_graph, games_on_current_map));
    }
}
void Tournament::play_games()
{
    cout << games.size() << endl;
    int k = 1;
    map<Graph*, vector<Game*>*>::reverse_iterator rit;
    for (rit = games.rbegin(); rit != games.rend(); ++rit)
    {
        vector<Game*>* current_games = rit->second;
        cout << current_games->size();
        for(int i = 0; i < current_games->size(); i++)
        {
            cout << "*************GAME NUMBER " << (i+1) << " ON MAP NUMBER " << k << endl;
            mainGameLoopTournament(*(*current_games)[i]);
        }
        k++;
    }
}

void Tournament::display_results()
{
    cout << "****************BIG RISK TOURNAMENT RESULTS****************" << endl;
    cout << "\t\t";
    for(int i = 1; i <= G; i++)
    {
        cout << "Game " << i << "         ";
    }
    cout << endl;
    int i = 0;
    map<Graph*, vector<Game*>*>::reverse_iterator rit;
    for (rit = games.rbegin(); rit != games.rend(); ++rit)
    {
        vector<Game*>* current_games = rit->second;
        printf("%-16s", map_names[i].c_str());
        for(int i = 0; i < current_games->size(); i++)
        {
            if((*current_games)[i]->winningPlayer)
                printf("%-15s", (*current_games)[i]->winningPlayer->getName().c_str());
            else cout << "DRAW           ";
        }
        cout << endl;
        i++;
    }
    cout << endl;
}