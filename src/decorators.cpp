#pragma once;

#include "../include/decorators.h"

void DominationDecorator::display(){
    statObserver.display();
    //TODO Add domination behaviour to stat observer
}

void PlayerHandDecorator::display(){
    statObserver.display();
    //TODO Add player hand behaviour to stat observer
}

void ContinentDecorator::display(){
    statObserver.display();
    //TODO Add Continent behaviour to stat observer
}