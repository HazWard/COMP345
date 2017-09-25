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

int main()
{	
	Dice* dice1 = new Dice(); // create dice object 1
	Dice* dice2 = new Dice(); // create dice object 2

	cout << "========Testing the quantity of dice requested========" << endl << endl;
	cout << "Dice #1: Please enter the number of dice you would like to use: ";
	int numOfDice;
	cin >> numOfDice;
	vector<int> diceVecNums1 = dice1->howManyDice(numOfDice);
	cout << "The number of dice requested by dice #1 is: " << diceVecNums1.size() << endl << endl;

	cout << "Dice #2: Please enter the number of dice you would like to use: ";
	cin >> numOfDice;
	vector<int> diceVecNums2 = dice2->howManyDice(numOfDice);
	cout << "The number of dice requested by dice #2 is: " << diceVecNums2.size() << endl << endl;

	cout << "========Testing that the container returns the correct values========" << endl << endl;
	cout << "Values stored for Dice #1: " << endl;

	for (int i = 0; i < diceVecNums1.size(); i++)
	{
		cout << diceVecNums1[i] << endl;
	}

	cout << endl << "Values stored for Dice #2: " << endl;

	for (int i = 0; i < diceVecNums2.size(); i++)
	{
		cout << diceVecNums2[i] << endl;
	}
	
	cout << endl << "========Testing that every dice rolling facility maintains a percentage of all individual values rolled.========" << endl << endl;	
	vector<int> diceVec1 = dice1->getNumTracker();

	for (int i = 0; i < diceVec1.size(); i++)
	{
		cout << "Number of " << i + 1 << " in dice #1: " << diceVec1[i] << endl;
	}

	vector<int> diceVec2 = dice2->getNumTracker();
	cout << endl;

	for (int i = 0; i < diceVec2.size(); i++)
	{
		cout << "Number of " << i + 1 << " in dice #2: " << diceVec2[i] << endl;
	}

	system("pause");
	return 0;
}
