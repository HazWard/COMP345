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
}

void StatObserver::display() {
    //TODO: Implement how to display the information
}

PhaseObserver::PhaseObserver() :Observer() {}

void PhaseObserver::update() {
    this->display();
}

void PhaseObserver::display() {
    //TODO: display current phase and player turn

    //display info depending on phase
   if( dynamic_cast < ReinforceEvent* > ( dynamic_cast < Game* > (model)->currentEvent ))
   {
       displayReinforceInfo();
   }
   else if(dynamic_cast < AttackEvent* > ( dynamic_cast < Game* > (model)->currentEvent ))
   {
       displayAttackInfo();
   }
   else if(dynamic_cast < FortifyEvent* > ( dynamic_cast < Game* > (model)->currentEvent ))
   {
       displayFortifyInfo();
   }
}

void PhaseObserver::displayReinforceInfo(){
//TODO: Display reinforce info
}

void PhaseObserver::displayAttackInfo(){
//TODO: Display attack info
}
// Displays info when in fortify phase
void PhaseObserver::displayFortifyInfo(){
    std:: cout << dynamic_cast < FortifyEvent* > ( dynamic_cast < Game* > (model)->currentEvent )->armiesMoved << " armies are being moved from " <<
               dynamic_cast < FortifyEvent* > ( dynamic_cast < Game* > (model)->currentEvent )->source->getPointerToCountry()->getName()
               << " to" << dynamic_cast < FortifyEvent* > ( dynamic_cast < Game* > (model)->currentEvent )->destination->getPointerToCountry()->getName() << std::endl;
}
