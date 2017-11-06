//
// Created by Emilio Assuncao on 2017-11-06.
//

#include "../include/observer.h"

#include <vector>
#include <iostream>
using namespace std;




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