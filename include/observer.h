//
// Created by Emilio Assuncao on 2017-11-06.
//
#pragma once

#include <list>
#include "../include/events.h"

//Forward declaration of Observer
class Observer;

class Subject {
    /**
     * Abstract class for subjects. In the Observer model, there is a one to many
     * relationship between inheritors of this class and inheritors of the Observer
     * class. The list _views_ contains all the observer objects to which this subject
     * will be sharing information.
     */
    std::list<Observer*> views;
public:
    Subject();
    void attach(Observer *observer);
    void detach(Observer *observer);
    void notify();
};

class Observer {
    /**
     * Abstract class for observers. Contains an instance of the model they are
     * observing. All classes that inherit from this class must define what they will
     * do when update() is invoked.
     */
    Subject *model;
public:
    Observer();
    virtual void update() {};
protected:
    Subject *getSubject();
};