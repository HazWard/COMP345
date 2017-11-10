/*
 Created by hanna on 2017-11-08.
 event classes that represent game state for observers to observe
*/

#pragma once

#include "../include/map.h"

class Event {
    public:
        virtual std::string printEvent() = 0;
};

class ReinforceEvent : public Event {
    public:
        ReinforceEvent();
        ReinforceEvent(vector<int> bonus,vector<int> armiesPlaced,vector<Node*> cuntTreesReinforced);
        vector<int> bonus;
        vector<int> armiesPlaced;
        vector<Node*> cuntTreesReinforced;
        std::string printEvent() = 0;
};

class AttackEvent : public Event{
    public:
        AttackEvent();
        AttackEvent(Node* attacker, Node* attacked, vector<int> attackerRolls, vector<int> attackedRolls);
        Node* attacker;
        Node* attaked;
        vector<int> attackerRolls;
        vector<int> attackedRolls;
        std::string printEvent();
};

class FortifyEvent : public Event{
    public:
        FortifyEvent();
        FortifyEvent(int armiesMoved, Node* source, Node* destination);
        int armiesMoved;
        Node* source;
        Node* destination;
        std::string printEvent();
};