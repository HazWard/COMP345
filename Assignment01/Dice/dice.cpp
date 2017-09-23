#include <iostream>
#include <vector>
#include <random>
#include "dice.h"
using namespace std;

int Dice::numGenerator()
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(1, 6); // define the range
	int ranNum = distr(eng); // store the random number

	return ranNum; // return the random number generated
}

int* Dice::getNumTracker()
{
	return numTracker;
}

vector<int> Dice::howManyDice(int howManyDice)
{
	vector<int> diceVec(howManyDice); // create container
	int ranNum; //create variable for random number (for later use)

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
	Dice* dice1 = new Dice();
	Dice* dice2 = new Dice();

	int testNum = 5;

	vector<int> diceVecNums = dice1->howManyDice(testNum);



	for (int i = 0; i < diceVecNums.size(); i++)
	{
		cout << diceVecNums[i] << endl;
	}

	system("pause");
	return 0;
}