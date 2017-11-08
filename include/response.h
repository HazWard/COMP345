/**
 * Class representing a response
 * - Information required to perform the given
 *   phase for a Player object
 */

#pragma once

#include "../include/map.h"
#include "../include/player.h"

class Player; // Forward Declaration

class ReinforceResponse
{
public:
    int nbArmies;
    Node* country;
};

class AttackResponse
{
public:
    std::pair<Player*, Node*> attacker;
    std::pair<Player*, Node*> defender;
};

class FortifyResponse
{
public:
    int nbArmies;
    Node* sourceCountry;
    Node* destinationCountry;
};