//
// Created by Emilio Assuncao on 2017-11-06.
//

#include "../include/views.h"

#import <iostream>
using namespace std;

StatObserver::StatObserver() {}

void StatObserver::update() {
    this->display();
}

void StatObserver::display() {
    //TODO: Implement how to display the information
}

PhaseObserver::PhaseObserver() {}

void PhaseObserver::update() {
    this->display();
}

void PhaseObserver::display() {
    //TODO: Implement how to display the information
}
