//
// Created by Emilio Assuncao on 2017-11-06.
//
#pragma once

#include <iostream>
#include <list>
using namespace std;

class Observer {
    Subject *model;
public:
    virtual void update();
protected:
    Subject *getSubject();
};

class Subject {
    list<Observer*> views;
public:
    void attach(Observer *observer);
    void detach(Observer *observer);
    void notify();
};