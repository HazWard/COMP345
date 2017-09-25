#include "stdafx.h"
#include <iostream>

#include "Country.h"

using namespace std;


//We define 2 countries to be equal if they have the same name
bool Country::operator==(Country& c)const 
{
	if (this->name == c.name)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& stream, Country& c)
{
	return stream << "\tInfo of country:" << endl <<
		"\tName: " << c.getName() << endl << "\tContinent: " << c.getContinent() << endl <<
		"\tPlayer: " << c.getPlayer() << endl << "\tNumber of armies: " << c.getNbrArmies() << endl;
}


Country::Country() : name(""), continent(0), player(0), nbrArmies(0)
{

}
Country::Country(string n, int c, int p, int nbrArm)
{
	this->name = n;
	this->continent = c;
	this->player = p;
	this->nbrArmies = nbrArm;
}
string Country::getName()
{
	return name;
}
int Country::getContinent()
{
	return continent;
}
int Country::getPlayer()
{
	return player;
}
int Country::getNbrArmies()
{
	return nbrArmies;
}