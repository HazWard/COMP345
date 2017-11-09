/* Assignment #1 Part: Dice
   Written By: Hagop Awakian
   Student ID: 27747632
   For COMP345 Section D
   Instructor: Dr. Joey Paquet
*/

#include <iostream>
#include <vector>
#include "../include/dice.h"
using namespace std;

Dice::Dice() : numTracker(6){
	std::random_device rd;
	std::mt19937 mt(rd());
} // contructor

int Dice::numGenerator()
{
	std::uniform_int_distribution<int> dist(1, 6);
	int ranNum = dist(mt);
	return ranNum; // return the random number generated
}

vector<int> Dice::getNumTracker()
{
	return numTracker; // getter for tracking the numbers obtained when rolling the dice
}

vector<int> Dice::howManyDice(int howManyDice)
{
	vector<int> diceVec(howManyDice); // create container
	int ranNum; //create variable to store the random number (for later use)

	for (int i = 0; i < diceVec.size(); i++)
	{
		ranNum = numGenerator(); // generates a random number
		diceVec[i] = ranNum; // inserts the random number generated in the vector (container)
		numTracker[ranNum - 1]++; // keep track of dice numbers
	}
	return diceVec; // returns the pointer to the vector (container)
}
