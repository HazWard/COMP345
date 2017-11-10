/*
 Created by hanna on 2017-11-08.
 event classes that represent game state for observers to observe
*/

#pragma once

#include "../include/player.h"
#include "../include/map.h"

class Event {
    public:
        virtual std::string printEvent() = 0;
};

class ReinforceEvent : public Event {
    public:
        ReinforceEvent();
        ReinforceEvent(vector<int> armiesPlaced,vector<Node*> countriesReinforced);
        vector<int> bonus;
        vector<int> armiesPlaced;
        vector<Node*> countriesReinforced;
        std::string printEvent() = 0;
        const type = 0;
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
        FortifyEvent();
        FortifyEvent(int armiesMoved, Node* source, Node* destination);
        int armiesMoved;
        Node* source;
        Node* destination;
        std::string printEvent();
        const type = 2;
};