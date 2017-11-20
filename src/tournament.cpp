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
        std::cout << *iterator << std::endl;
    }
    std::cout << "You can select from 1 to " << MAX_MAPS << " maps from the list above." << std::endl;
    std::cout << "You can select from " << MIN_PLAYERS << " to " << MAX_PLAYERS << " different computer players strategies." << std::endl;
    std::cout << "You can select from 1 to " << MAX_GAMES_PER_MAP << " games to be played on each map." << std::endl;
    std::cout << "You can select the maximum number of turns per game from " << LOWER_MAX_TURNS_PER_GAME << " to " << UPPER_MAX_TURNS_PER_GAME << "." << std::endl;
    std::cout << "\t(Note: this is to minimum run completion time.)" << std::endl << std::endl;

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

        vector<string> line_data_maps = split(M_in_string_form, ',');
        //Allocating/reserving an array of size equal to the number of file names (since it is technically the number of maps)
        M = vector<Parser*>(line_data_maps.size());

        for (int i = 0; i < line_data_maps.size(); i++) {
            line_data_maps[i] = trim_dots(trim(line_data_maps[i]));
            Parser *parser;
            cout << "Loading map located at " << MAPS_FOLDER << line_data_maps[i] << endl;

            parser = new Parser(MAPS_FOLDER + line_data_maps[i]);

            if (parser->mapIsValid()) {
                cout << "The map " << line_data_maps[i] << " is valid.\n\n";
                valid_M = true;
            } else if (parser->getGraph()->getNbrCountries() > 80) {
                cout << "We have detected that the number of countries in this Map is greater than 80." << endl
                     << "That is not supported in the current version of the game." << endl;
                cout << "Please enter a valid M." << endl;
                valid_M = false;
                M.clear();
                break;
            } else {
                cout << "The graph and/or some of the continents are not strongly connected.\n";
                cout << "Please enter a valid M." << endl;
                valid_M = false;
                M.clear();
                break;
            }
            M.push_back(parser);
        }
        if(M.size() < 1 || M.size() > MAX_GAMES_PER_MAP)
        {
            std::cout << "You entered no valid maps to be used, or you entered more than " << MAX_GAMES_PER_MAP << " maps." << std::endl;
            valid_M = false;
        }

        vector<string> line_data_players = split(P_in_string_form, ',');
        //Allocating/reserving an array of size equal to the number of file names (since it is technically the number of maps)
        //P = vector<Player*>(line_data_players.size());
        P.reserve(line_data_players.size());
        for(int i = 0; i < line_data_players.size(); i++)
        {
            line_data_players[i] = trim_dots(trim(line_data_players[i]));
            if(line_data_players[i] == "Aggressive") {
                int number_aggressive_player = 1;
                for(int j = 0; j < P.size(); j++) {
                    if(P[j]->getName().find("Aggressive") != std::string::npos)
                        number_aggressive_player++;
                }
                P.push_back(new Player("Aggressive " + number_aggressive_player, new AggressiveStrategy));
            }
            else if(line_data_players[i] == "Benevolent") {
                int number_benevolent_player = 1;
                for(int j = 0; j < P.size(); j++) {
                    if(P[j]->getName().find("Benevolent") != std::string::npos)
                        number_benevolent_player++;
                }
                P.push_back(new Player("Benevolent " + number_benevolent_player, new BenevolentStrategy));
            }
            else if(line_data_players[i] == "Random")
            {
                int number_random_player = 1;
                for(int j = 0; j < P.size(); j++) {
                    if(P[j]->getName().find("Random") != std::string::npos)
                        number_random_player++;
                }
                P.push_back(new Player("Random " + number_random_player, new RandomStrategy));
            }
            else if(line_data_players[i] == "Cheater")
            {
                int number_cheater_player = 1;
                for(int j = 0; j < P.size(); j++) {
                    if(P[j]->getName().find("Cheater") != std::string::npos)
                        number_cheater_player++;
                }
                P.push_back(new Player("Cheater " + number_cheater_player, new CheaterStrategy));
            } else
            {
                std::cout << line_data_players[i] << " is not a valid type of player. Please enter a proper P." << std::endl;
                valid_P = false;
                break;
            }
        }
        if(P.size() < MIN_PLAYERS || P.size() > MAX_PLAYERS)
        {
            std::cout << "You entered less than " << MIN_PLAYERS << " or more than " << MAX_PLAYERS << "." << std::endl;
            valid_P = false;
        }
        else if(P.size() == line_data_players.size())
            valid_P = true;

        if(G < 1 || G > MAX_GAMES_PER_MAP)
        {
            std::cout << "You entered an invalid number of games to be played on each map." << std::endl;
            std::cout << "Please enter a proper G from 1 to " << MAX_GAMES_PER_MAP << "." << std::endl;
            valid_G = false;
        }
        else
            valid_G = true;

        if(D < LOWER_MAX_TURNS_PER_GAME || D > UPPER_MAX_TURNS_PER_GAME)
        {
            std::cout << "You entered an invalid maximum number of turns for each game." << std::endl;
            std::cout << "Please enter a proper D from " << LOWER_MAX_TURNS_PER_GAME << " to " << UPPER_MAX_TURNS_PER_GAME << "." << std::endl;
            valid_D = false;
        }
        else
            valid_D = true;

    }   while(!valid_M || !valid_P || !valid_G || !valid_D);

}

int main()
{
    Tournament t;
    if(wind)
        system("pause");
    else {
        std::cout << "Press any key to continue . . ." << std::endl;
        std::getchar();
    }

    return 0;
}