/* Assignment #1 Part: Dice
   Written By: Hagop Awakian
   Student ID: 27747632
   For COMP345 Section D
   Instructor: Dr. Joey Paquet
*/

#include <vector>
using namespace std;

#pragma once

class Dice
{
	private:
		vector<int> numTracker;
	public:
		Dice();
		int numGenerator();
		vector<int> howManyDice(int howManyDice);
		vector<int> getNumTracker();
};