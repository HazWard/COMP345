#pragma once

#include "../include/observer.h"
#include "../include/views.h"

class StatObserverDecorator : public Observer {
    /**
     * Abstract Decorator type for the three StatObserver decorators.
     */
public:
    StatObserverDecorator(Observer *so) : statObserver(so) {};
    virtual void update(int code) = 0;
    virtual void display() = 0;
    Observer* getStatObserver();

protected:
    Observer *statObserver;

};

class DominationDecorator : public StatObserverDecorator {

public:
    DominationDecorator(Observer *so) : StatObserverDecorator(so) {};
    void update(int code);
    void display();
};

class PlayerHandDecorator : public StatObserverDecorator {

public:
    PlayerHandDecorator(Observer *so) : StatObserverDecorator(so) {};
    void update(int code);
    void display();
};

class ContinentDecorator : public StatObserverDecorator {

public:
    ContinentDecorator(Observer *so) : StatObserverDecorator(so) {};
    void update(int code);
    void display();
};