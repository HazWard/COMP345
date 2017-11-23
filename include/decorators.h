#pragma once

#include "../include/observer.h"
#include "../include/views.h"

class StatObserverDecorator : public Observer {
    /**
     * Abstract Decorator type for the three StatObserver decorators.
     */
public:
    virtual void update(int code) = 0;
    virtual void display() = 0;
    Observer* getStatObserver();

private:
    Observer *statObserver;

};

class DominationDecorator : public StatObserverDecorator {

public:
    DominationDecorator(Observer *so) : statObserver(so) {};
    void update(int code);
    void display();

private:
    Observer *statObserver;
};

class PlayerHandDecorator : public StatObserverDecorator {

public:
    PlayerHandDecorator(Observer *so) : statObserver(so) {};
    void update(int code);
    void display();

private:
    Observer *statObserver;
};

class ContinentDecorator : public StatObserverDecorator {

public:
    ContinentDecorator(Observer *so) : statObserver(so) {};
    void update(int code);
    void display();

private:
    Observer *statObserver;
};