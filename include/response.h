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