//
// Created by Emilio Assuncao on 2017-11-06.
//
#include <iostream>
#include "../include/events.h"
#include "../include/views.h"
#include "../include/game.h"
using namespace std;

StatObserver::StatObserver() :Observer() {}

void StatObserver::update(int code) {
    if(code == NEW_TURN) {
        this->display();
        // Pausing System: COMMENT OUT FOR PART 3
        /*
        cout << '\n' << "Press Enter to continue";
        cin.ignore();
        cin.get();*/
    }
}

void StatObserver::display()
{
    Game* game = static_cast <Game*> (model);
    cout << "================== Current Turn: " << game->currentTurn << " ==================" << endl;
}

PhaseObserver::PhaseObserver() :Observer() {}

void PhaseObserver::update(int code) {
    if(code != NEW_TURN) {
        this->display();
        // Pausing System COMMENT OUT FOR PART 3
        cout << '\n' << "Press Enter to continue";
        cin.ignore();
        cin.get();
    }
}

void PhaseObserver::display() {

    cout << endl;
    std::cout << "--------Phase Observer--------"<< std::endl;
   if(dynamic_cast <ReinforceEvent*>((static_cast < Game* > (model))->currentEvent ))
   {
       std::cout << "We are in the Reinforcement phase" << " of the player: "<< (static_cast < Game* > (model))->currentPlayer->getName() <<std::endl;
       displayReinforceInfo();
   }

   else if(dynamic_cast <AttackEvent*> ( (static_cast < Game* > (model))->currentEvent ))
   {
       std::cout << "We are in the Attacking phase" << " of the player: "<< (static_cast < Game* > (model))->currentPlayer->getName() <<std::endl;
       displayAttackInfo();
   }

   else if(dynamic_cast <FortifyEvent*> ( (static_cast < Game* > (model))->currentEvent ))
   {
       std::cout << "We are in the Fortification phase" << " of the player: "<< (static_cast < Game* > (model))->currentPlayer->getName() <<std::endl;
       displayFortifyInfo();
   }
    std::cout << "------------------------------"<< std::endl;
}

void PhaseObserver::displayReinforceInfo(){
    ReinforceEvent* event = dynamic_cast < ReinforceEvent* > ( static_cast < Game* > (model)->currentEvent );
    if(event->countriesReinforced.size() == 0)
    {
        cout << "No reinforcements were made." << std::endl;
        return;
    }
    std::cout << "These reinforcements are being processed: " << std::endl;
    for (unsigned int i = 0; i < event->countriesReinforced.size(); i++)
    {
        std::cout << "==> " <<event->armiesPlaced.at(i) << " armies are being added to " << event->countriesReinforced.at(i)->getPointerToCountry()->getName() << std::endl;
    }
}

void PhaseObserver::displayAttackInfo(){
    AttackEvent* event = dynamic_cast< AttackEvent* > ( static_cast < Game* > (model)->currentEvent );
    std::cout << event->attacker->getName() << " is using the country " << event->attacking->getCountry().getName() <<
     " to attack " << event->defender->getName() << "'s country " << event->defending->getCountry().getName()
              << ".\n";
    if (event->victory){
        std::cout<<"Attack was successful\n";
    }
    else
        std::cout<<"Attack failed\n";
}

void PhaseObserver::displayFortifyInfo(){
    FortifyEvent* event = dynamic_cast < FortifyEvent* > (static_cast < Game* > (model)->currentEvent);
    if(event->armiesMoved == 0) {
        cout << "No fortifications were made." << std::endl;
        return;
    }
    std:: cout << event->armiesMoved << " armies are being moved from " << event->source->getPointerToCountry()->getName()
               << " to " << event->destination->getPointerToCountry()->getName() << std::endl;
}
