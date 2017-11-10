//
// Created by Emilio Assuncao on 2017-11-06.
//
#include <iostream>
#include "../include/events.h"
#include "../include/views.h"
#include "../include/game.h"
using namespace std;

StatObserver::StatObserver() :Observer() {}

void StatObserver::update() {
    this->display();
    // pausing system
    cout << '\n' << "Press a key to continue...";
    cin;
}

void StatObserver::display()
{
    cout << "-------------Game Statistics-------------";
    Game* game = static_cast <Game*> (model);
    vector<Player*>* players = game->getArrayPlayers();

    for(int i = 0; i < game->getNbrPlayers(); i++)
    {
        cout << game->players[i]->getName() << " : ";
        for(int j = 0; j <= floor ((players->at(i)->getNodes()->size()/game->getMapCountries()->getVectorOfNodes()->size()) * 10); j++)
        {
            cout << "*";
        }
    }
}

PhaseObserver::PhaseObserver() :Observer() {}

void PhaseObserver::update() {
    this->display();
    // pausing system
    cout << '\n' << "Press a key to continue...";
    cin;
}

void PhaseObserver::display() {
    //TODO: display current phase and player turn

    //display info depending on phase
   if(dynamic_cast <ReinforceEvent*>((static_cast < Game* > (model))->currentEvent ))
   {
       displayReinforceInfo();
   }
   else if(dynamic_cast <AttackEvent*> ( (static_cast < Game* > (model))->currentEvent ))
   {
       displayAttackInfo();
   }
   else if(dynamic_cast <FortifyEvent*> ( (static_cast < Game* > (model))->currentEvent ))
   {
       displayFortifyInfo();
   }
}

void PhaseObserver::displayReinforceInfo(){
    ReinforceEvent* event = dynamic_cast < ReinforceEvent* > ( static_cast < Game* > (model)->currentEvent );
    std::cout << "These reinforcements are being processed: " << std::endl;
    for (unsigned int i = 0; i < event->countriesReinforced.size(); i++)
    {
        std::cout << "==> " <<event->armiesPlaced.at(i) << " armies are being added to " << event->countriesReinforced.at(i) << std::endl;
    }
}

void PhaseObserver::displayAttackInfo(){
    std::cout << dynamic_cast< AttackEvent* > ( static_cast < Game* > (model)->currentEvent )->attacker->getName() <<
     " is attacking " << dynamic_cast< AttackEvent* > ( static_cast < Game* > (model)->currentEvent )->defender->getName()
              << ".\n";
}
// Displays info when in fortify phase
void PhaseObserver::displayFortifyInfo(){
    FortifyEvent* event = dynamic_cast < FortifyEvent* > ( static_cast < Game* > (model)->currentEvent );
    std:: cout << event->armiesMoved << " armies are being moved from " << event->source->getPointerToCountry()->getName()
               << " to" << event->destination->getPointerToCountry()->getName() << std::endl;
}
