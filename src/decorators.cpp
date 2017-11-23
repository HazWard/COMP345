#include <iostream>
#include "../include/decorators.h"
#include "../include/game.h"

void DominationDecorator::update(int code)
{
    this->statObserver->update(code);
    if(code == NEW_CONQUEST || code == CONTINENT_CONTROL) {
        this->display();
        // pausing system
        cout << '\n' << "Press Enter to continue";
        cin.ignore();
        cin.get();
    }
}

void DominationDecorator::display()
{
    Game* game = static_cast<Game*> (this->statObserver->model);
    int numberOfPlayers = game->getNbrPlayers();
    vector<Player*>* players = game->getArrayPlayers();

    for(int i = 0; i < numberOfPlayers; i++)
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
    this->statObserver->update(code);
    if(code == HAND_CHANGE) {
        this->display();
        // pausing system
        cout << '\n' << "Press Enter to continue";
        cin.ignore();
        cin.get();
    }
}

void PlayerHandDecorator::display()
{
    Game* game = static_cast <Game*> (this->statObserver->model);
    vector<Player*> *players1 = game->getArrayPlayers();
    for(int i=0; i<players1->size(); i++)
    {
        cout << "Player " << players1->at(i)->getName() << "'s hand:" << endl;
        players1->at(i)->getHand()->display();
    }
}

void ContinentDecorator::update(int code)
{
    this->statObserver->update(code);
    if(code == CONTINENT_CONTROL) {
        this->display();
        // pausing system
        cout << '\n' << "Press Enter to continue";
        cin.ignore();
        cin.get();
    }
}

void ContinentDecorator::display()
{
    Game* game = static_cast <Game*> (this->statObserver->model);
    vector<Player*> players1 = game->players;
    for(int i=0; i<players1.size(); i++)
    {
        vector<Continent*> *continentsOwned = players1[i]->getsContinentsOwned(game->getContinents());
        if (!(continentsOwned->size()== 0))
        {
            cout << "Player " << players1[i]->getName() << " controls the following continents:" << endl;
            for (int j = 0; i < continentsOwned->size(); j++)
            {
               cout << continentsOwned->at(i)->getName() << endl;
            }
        }
    }


}

Observer* StatObserverDecorator::getStatObserver() {
    return this->statObserver;
}
