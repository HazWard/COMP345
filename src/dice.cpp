/* Assignment #1 Part: Dice
   Written By: Hagop Awakian
   Student ID: 27747632
   For COMP345 Section D
   Instructor: Dr. Joey Paquet
*/

#include <iostream>
#include <vector>
#include <random>
#include "../include/dice.h"
using namespace std;

Dice::Dice() : numTracker(6){} // contructor

int Dice::numGenerator()
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(1, 6); // define the range
	int ranNum = distr(eng); // store the random number

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
