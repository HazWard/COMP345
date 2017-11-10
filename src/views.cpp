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

void StatObserver::display() {

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

}

void PhaseObserver::displayAttackInfo(){
    std::cout << dynamic_cast< AttackEvent* > ( static_cast < Game* > (model)->currentEvent )->attacker->getName() <<
     " is attacking " << dynamic_cast< AttackEvent* > ( static_cast < Game* > (model)->currentEvent )->defender->getName()
              << ".\n The defender has ";
}
// Displays info when in fortify phase
void PhaseObserver::displayFortifyInfo(){
    std:: cout << dynamic_cast < FortifyEvent* > ( static_cast < Game* > (model)->currentEvent )->armiesMoved << " armies are being moved from " <<
               dynamic_cast < FortifyEvent* > ( static_cast < Game* > (model)->currentEvent )->source->getPointerToCountry()->getName()
               << " to" << dynamic_cast < FortifyEvent* > ( static_cast < Game* > (model)->currentEvent )->destination->getPointerToCountry()->getName() << std::endl;
}
