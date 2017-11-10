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
   if( dynamic_cast < FortifyEvent* > ( model->currentEvent )->type == 0 )
   {
       displayReinforceInfo();
   }
   else if(dynamic_cast < FortifyEvent* > ( model->currentEvent )->type == 1)
   {
       displayAttackInfo();
   }
   else if(dynamic_cast < FortifyEvent* > ( model->currentEvent )->type == 2)
   {
       displayFortifyInfo();
   }
}

void PhaseObserver::displayReinforceInfo(){

}

void PhaseObserver::displayAttackInfo(){

}

void PhaseObserver::displayFortifyInfo(){
    std:: cout << dynamic_cast < FortifyEvent* > ( model->currentEvent )->armiesMoved << " armies are being moved from " <<
               dynamic_cast < FortifyEvent* > ( model->currentEvent )->source->getPointerToCountry()->getName()
               << " to" << dynamic_cast < FortifyEvent* > ( model->currentEvent )->destination->getPointerToCountry()->getName() << std::endl;
}
