//
// Created by Emilio Assuncao on 2017-11-06.
//
#pragma once

#include "../include/observer.h"

class PhaseObserver : public Observer{
    /**
     * Concrete observer that will act as the view for what is going on when it comes to
     * players and their reinforce, attack and fortify phase.
     * TODO: Decide how this observer will gather the information and display it
     */
public:
    PhaseObserver();
    PhaseObserver(Subject *subject) : Observer(subject) {};
    void update(int code);
    void display();
    void displayReinforceInfo();
    void displayAttackInfo();
    void displayFortifyInfo();
};

class StatObserver : public Observer{
    /**
     * Concrete observer that will act as a view for what is going on when it comes
     * to the winning conditions of the game (battles and map domination).
     * TODO: Write implementation 
     */
public:
    StatObserver();
    StatObserver(Subject *subject) : Observer(subject) {};
    void update(int code);
    void display();
};