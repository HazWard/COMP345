//
// Created by Emilio Assuncao on 2017-11-06.
//
#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Observer {


};

class Subject {
    vector<Observer*> views;
    int value;

};


#endif //COMP345_OBSERVER_H
