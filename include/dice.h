#include <vector>
using namespace std;

#pragma once

class Dice
{
	private:
		int numTracker[6];
	public:
		int numGenerator();
		vector<int> howManyDice(int howManyDice);
		int* getNumTracker();
};