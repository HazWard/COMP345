/*
    Filename: player.cpp 
    Author: Starly Solon (40022595)
    Date of creation: September 14, 2017
    Description: Header file for Risk player
*/

// #include "cards.h"
// #include "dice.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
  public:
    Player(int[], int[]);
    int diceFactory(int);
    void reinforce();
    void attack();
    void fortify();
  private:
    int* countries[]; // TODO: Change to array of Country
	int* hand[]; // TODO: Change to array of Cards
};
#endif // PLAYER_H
