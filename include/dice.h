/* Assignment #1 Part: Dice
   Written By: Hagop Awakian
   Student ID: 27747632
   For COMP345 Section D
   Instructor: Dr. Joey Paquet
*/

#pragma once

#include <vector>
#include <random>
using namespace std;

class Dice
{
private:
	vector<int> numTracker;
	std::mt19937 mt;
public:
	Dice();
	int numGenerator();
	vector<int> howManyDice(int howManyDice);
	vector<int> getNumTracker();
};