/**
 * Class representing a response
 * - Information required to perform the given
 *   phase for a Player object
 */

#pragma once

#include "../include/map.h"
#include "../include/player.h"

class Player; // Forward Declaration

/** Response Summary (PLEASE READ THIS!)
 * This set of classes represent all the possible changes a Player's game phases can produce.
 * The is then interpreted by the Game to produce the actual changes to the map.
 */

class ReinforceResponse
{
public:
    int nbArmies;
    Country* country;
    ReinforceResponse(int n, Country *c) : nbArmies(n), country(c) {};
};

class AttackResponse
{
public:
    std::pair<Player*, Node*>* attacker;
    std::pair<Player*, Node*>* defender;
    AttackResponse(std::pair<Player*,Node*> *a, std::pair<Player*,Node*> *d) : attacker(a), defender(d) {};
};

class FortifyResponse
{
public:
    int nbArmies;
    Node* sourceCountry;
    Node* destinationCountry;
    FortifyResponse(int n, Node *s, Node *d) : nbArmies(n), sourceCountry(s), destinationCountry(d) {};
};