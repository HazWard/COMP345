#pragma once

#include "../include/observer.h"
#include "../include/views.h"

class StatObserverDecorator : public Observer {
    /**
     * Abstract Decorator type for the three StatObserver decorators.
     */
public:
    virtual void update() = 0;
    virtual void display() = 0;

private:
    StatObserver statObserver;

};

class DominationDecorator : public StatObserverDecorator {

public:
    DominationDecorator(StatObserver *so) : statObserver(so) {};
    void update();
    void display();

private:
    StatObserver *statObserver;
};

class PlayerHandDecorator : public StatObserverDecorator {

public:
    PlayerHandDecorator(StatObserver *so) : statObserver(so) {};
    void update();
    void display();

private:
    StatObserver *statObserver;
};

class ContinentDecorator : public StatObserverDecorator {

public:
    ContinentDecorator(StatObserver *so) : statObserver(so) {};
    void update();
    void display();

private:
    StatObserver *statObserver;
};