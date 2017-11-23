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
    Node* country;
    bool exchangeOccured;
    ReinforceResponse(int n, Node *c, bool e) : nbArmies(n), country(c), exchangeOccured(e) {};
};

class AttackResponse
{
public:
    std::pair<Player*, Node*>* attacker;
    std::pair<Player*, Node*>* defender;
    bool isCheater;
    bool cheaterPhase;
    AttackResponse() = default;
    virtual ~AttackResponse() = default;
    AttackResponse(std::pair<Player*,Node*> *a, std::pair<Player*,Node*> *d, bool c, bool p) : attacker(a), defender(d), isCheater(c), cheaterPhase(p) {};
};

class CheaterAttackResponse : public AttackResponse
{
public:
    std::vector<AttackResponse*>*  attackResponses;
    ~CheaterAttackResponse() = default;
    CheaterAttackResponse(std::vector<AttackResponse*>* r) : attackResponses(r) {};
};

class FortifyResponse
{
public:
    int nbArmies;
    Node* sourceCountry;
    Node* destinationCountry;
    bool isCheater;
    FortifyResponse() = default;
    virtual ~FortifyResponse() = default;
    FortifyResponse(int n, Node *s, Node *d, bool c) : nbArmies(n), sourceCountry(s), destinationCountry(d), isCheater(c) {};
};
class CheaterFortifyResponse : public FortifyResponse
{
public:
    std::vector<FortifyResponse*>* fortifyResponses;
    ~CheaterFortifyResponse() = default;
    CheaterFortifyResponse(std::vector<FortifyResponse*>* r) : fortifyResponses(r) {};
};
