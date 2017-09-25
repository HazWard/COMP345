#pragma once

#include <iostream>
#include <string>

using namespace std;

class Country
{
private:
	string name;
	int continent;
	int player;
	int nbrArmies;

public:
	Country();
	Country(string n, int c, int p, int nbrArm);
	bool operator==(Country& rhs)const;
	friend std::ostream& operator <<(std::ostream& stream, Country& c);
	string getName();
	int getContinent();
	int getPlayer();
	int getNbrArmies();
};