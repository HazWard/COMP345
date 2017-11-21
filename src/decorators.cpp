#include <iostream>
#include "../include/decorators.h"
#include "../include/game.h"

void DominationDecorator::update(int code)
{
    this->display();
    // pausing system
    cin.ignore(INT_MAX);
    std::cout << '\n' << "Press Enter to continue";
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
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
    this->display();
    // pausing system
    cin.ignore(INT_MAX);
    std::cout << '\n' << "Press Enter to continue";
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
}

void PlayerHandDecorator::display()
{
    statObserver->display();
    //TODO Add player hand behaviour to stat observer


}

void ContinentDecorator::update(int code)
{
    this->display();
    // pausing system
    cin.ignore(INT_MAX);
    std::cout << '\n' << "Press Enter to continue";
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
}

void ContinentDecorator::display()
{
    statObserver->display();
    //TODO Add Continent behaviour to stat observer
}