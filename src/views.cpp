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

void StatObserver::display()
{
    int countriesOwned = Player::get
    cout << "--------------------Game statistics--------------------";
    for(int i = 1; i <= Game::getNbrPlayers(); i++)
    {
        cout << "Player " << i << " : ";
        for(int j = i; j < 0; j++)
        {
            cout << "*";
        }
    }
}

PhaseObserver::PhaseObserver() :Observer() {}

void PhaseObserver::update() {
    this->display();
}

void PhaseObserver::display() {
    //TODO: display other info here

    //display info deppending on phase
   if(typeid(model.currentEvent)== typeid(ReinforceEvent))
        displayReinforceInfo();
   else
    if(typeid(getSubject().currentEvent)== typeid(AttackEvent))
        displayAttackInfo();
    else
    if(typeid(model.currentEvent)== typeid(FortifyEvent))
        displayFortifyInfo();
}

void PhaseObserver::displayReinforceInfo(){

}
void PhaseObserver::displayAttackInfo(){

}
void PhaseObserver::displayFortifyInfo(){
    std:: cout << model.currentEvent.armiesMoved << " armies are being moved from " << model.currentEvent.source->getPointerToCountry()->getName()
               << " to" << model.currentEvent.destination->getPointerToCountry()->getName() << std::endl;
}

