//
// Created by hanna on 2017-11-20.
//
#pragma once;

#include "../include/observer.h"
#include "../include/views.h"

class Decorator : public Observer{

public:
    void update();
    void display();

private:
    StatObserver statObserver;

};

class DominationDecorator : public Decorator{

public:
    void update();
    void display();

private:
    StatObserver statObserver;
};

class PlayerHandDecorator : public Decorator{

public:
    void update();
    void display();

private:
    StatObserver statObserver;
};

class CuntinentDecorator : public Decorator{

public:
    void update();
    void display();

private:
    StatObserver statObserver;
};