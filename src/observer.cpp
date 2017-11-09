//
// Created by Emilio Assuncao on 2017-11-06.
//

#include "../include/observer.h"

#include <iostream>
using namespace std;

Subject::Subject() {}

void Subject::attach(Observer *observer) {
    views.push_back(observer);
}

void Subject::detach(Observer *observer) {
    views.remove(observer);
}

void Subject::notify() {
    for(list<Observer*>::iterator iter = views.begin(); iter != views.end(); iter++){
        (*iter)->update();
    }
}

Observer::Observer() {}

Subject* Observer::getSubject() {
    return model;
}

void Observer::update(){}