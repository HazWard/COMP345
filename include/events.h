/*
 Created by hanna on 2017-11-08.
 event classes that represent game state for observers to observe
*/

#pragma once

#include "map.h"

class ReinforceEvent{
    public:
        ReinforceEvent(vector<int> bonus,vector<int> armiesPlaced,vector<Node*> cuntTreesReinforced);
        vector<int> bonus;
        vector<int> armiesPlaced;
        vector<Node*> cuntTreesReinforced;
};

class AttackEvent{
    public:
        AttackEvent(Node* attacker, Node* attacked, vector<int> attackerRolls, vector<int> attackedRolls);
        Node* attacker;
        Node* attaked;
        vector<int> attackerRolls;
        vector<int> attackedRolls;
};

class FortifyEvent{
    public:
        FortifyEvent(int armiesMoved, Node* source, Node* destination);
        int armiesMoved;
        Node* source;
        Node* destination;
};