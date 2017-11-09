//
// Created by Emilio Assuncao on 2017-11-06.
//

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
   if(typeid(model.currentEvent)== typeid(reinforceEvent))
       this.displayReinforceInfo();
    else
    if(typeid(model.currentEvent)== typeid(attackEvent))
        this.displayAttackInfo();
    else
    if(typeid(model.currentEvent)== typeid(fortifyEvent))
        this.displayFortifyInfo();
}

void PhaseObserver::displayReinforceInfo(){

}
void PhaseObserver::displayAttackInfo(){

}
void PhaseObserver::displayFortifyInfo(){
    std:: cout << model.currentEvent.armiesMoved << " armies are being moved from " << model.currentEvent.source->getPointerToCountry()->getName()
               << " to" << model.currentEvent.destination->getPointerToCountry()->getName() << std::endl;
}

