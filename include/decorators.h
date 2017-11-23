#pragma once

#include "../include/observer.h"
#include "../include/views.h"

class StatObserverDecorator : public Observer {
    /**
     * Abstract Decorator type for the three StatObserver decorators.
     */
public:
    StatObserverDecorator(Observer *so) : statObserver(so), Observer(so->model) {};
    void update(int code) { this->statObserver->update(code); };
    Observer* getStatObserver();
private:
    Observer *statObserver;

};

class DominationDecorator : public StatObserverDecorator {

public:
    DominationDecorator(Observer *so) : StatObserverDecorator(so) {};
    void update(int code);
    void display();
    Observer* getStatObserver(){ return StatObserverDecorator::getStatObserver(); };
};

class PlayerHandDecorator : public StatObserverDecorator {

public:
    PlayerHandDecorator(Observer *so) : StatObserverDecorator(so) {};
    void update(int code);
    void display();
    Observer* getStatObserver(){ return StatObserverDecorator::getStatObserver(); };
};

class ContinentDecorator : public StatObserverDecorator {

public:
    ContinentDecorator(Observer *so) : StatObserverDecorator(so) {};
    void update(int code);
    void display();
    Observer* getStatObserver(){ return StatObserverDecorator::getStatObserver(); };
};