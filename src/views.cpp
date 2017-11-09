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
    //TODO: Implement how to display the information
    model.
}
