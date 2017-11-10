/*
 Created by hanna on 2017-11-08.
 event classes that represent game state for observers to observe
*/

#pragma once

#include "../include/player.h"
#include "../include/map.h"

class Event {
public:
    virtual ~Event() = 0;
};

class ReinforceEvent : public Event {
    public:
        ReinforceEvent(vector<int> armiesPlaced, vector<Node*> countriesReinforced) :armiesPlaced(armiesPlaced), countriesReinforced(countriesReinforced) {};
        vector<int> armiesPlaced;
        vector<Node*> countriesReinforced;
};

class AttackEvent : public Event{
    public:
        AttackEvent(Player* attacker, Player* defender, Node* attacking, Node* defending, vector<int> *attackerRolls, vector<int> *defenderRolls, bool victory, int armiesMoved)
            : attacker(attacker), defender(defender), attacking(attacking), defending(defending), attackerRolls(attackerRolls), defenderRolls(defenderRolls), victory(victory), armiesMoved(armiesMoved) {};
        Player* attacker;
        Player* defender;
        Node* attacking;
        Node* defending;
        vector<int> *attackerRolls;
        vector<int> *defenderRolls;
        bool victory;
        int armiesMoved;
};

class FortifyEvent : public Event{
    public:
        FortifyEvent(int armiesMoved, Node* source, Node* destination) :armiesMoved(armiesMoved), source(source), destination(destination) {};
        int armiesMoved;
        Node* source;
        Node* destination;
};