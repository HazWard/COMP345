#include <iostream>
#include "../include/decorators.h"
#include "../include/game.h"

void DominationDecorator::update(int code)
{
    if(code == NEW_CONQUEST || code == CONTINENT_CONTROL) {
        this->display();
        // pausing system
        cin.ignore(INT_MAX);
        std::cout << '\n' << "Press Enter to continue" << endl;
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    }
}

void DominationDecorator::display()
{
    statObserver->display();
    Game* game = static_cast <Game*> (model);
    vector<Player*>* players = game->getArrayPlayers();

    for(int i = 0; i < game->getNbrPlayers(); i++)
    {
        int thisPlayerPercentage = floor((players->at(i)->getNodes()->size()/ (double)game->getMapCountries()->getVectorOfNodes()->size() * 100));
        cout << players->at(i)->getName() << " owns " << thisPlayerPercentage << "% of the map : ";

        for(int j = 0; j <= thisPlayerPercentage; j++)
        {
            cout << "*";
        }
        cout << std::endl;
    }
}

void PlayerHandDecorator::update(int code)
{
    if(code == HAND_CHANGE) {
        this->display();
        // pausing system
        cin.ignore(INT_MAX);
        std::cout << '\n' << "Press Enter to continue" << endl;
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    }
}

void PlayerHandDecorator::display()
{
    statObserver->display();
    Game* game = static_cast <Game*> (model);
    vector<Player*> players1 = game->players;
    for(int i=0; i<players1.size(); i++)
    {
        cout << "Player " << players1[i]->getName() << "'s hand:" << endl;
        players1[i]->getHand()->display();
    }
}

void ContinentDecorator::update(int code)
{
    if(code == CONTINENT_CONTROL) {
        this->display();
        // pausing system
        cin.ignore(INT_MAX);
        std::cout << '\n' << "Press Enter to continue" << endl;
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    }
}

void ContinentDecorator::display()
{
    statObserver->display();
    Game* game = static_cast <Game*> (model);
    vector<Player*> players1 = game->players;
    for(int i=0; i<players1.size(); i++)
    {
        vector<Continent*> continentsOwned = players1[i]->getsContinentsOwned(*(game->getContinents()));
        if (!continentsOwned.size()== 0)
        {
            cout << "Player " << players1[i]->getName() << " controls the following continents:" << endl;
            for (int j = 0; i < continentsOwned.size(); j++)
            {
               cout << continentsOwned[i]->getName() << endl;
            }
        }
    }


}