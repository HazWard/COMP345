//
// Created by Emilio Assuncao on 2017-11-06.
//
#include <iostream>
#include "../include/events.h"
#include "../include/views.h"

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
    //TODO: display other info here

    //display info deppending on phase
   if(typeid(getSubject()->event)== typeid(ReinforceEvent))
   {
       displayReinforceInfo();
   }
   else if(typeid(getSubject()->event)== typeid(AttackEvent))
   {
       displayAttackInfo();
   }
   else if(typeid(getSubject()->event)== typeid(FortifyEvent))
   {
       displayFortifyInfo();
   }
}

void PhaseObserver::displayReinforceInfo(){

}
void PhaseObserver::displayAttackInfo(){

}
void PhaseObserver::displayFortifyInfo(){
    std:: cout << getSubject()->event.armiesMoved << " armies are being moved from " << getSubject()->event.source->getPointerToCountry()->getName()
               << " to" << getSubject()->event.destination->getPointerToCountry()->getName() << std::endl;
}
