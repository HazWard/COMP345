#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "../include/graph.h"
#include "../include/country.h"
#include "../include/node.h"
#include "../include/player.h"
#include "../include/dice.h"
#include "../include/cards.h"

using namespace std;

// Class Runners
void cardsRunner()
{
	//Constants used to define card types, used in lieu of an enum
	const int INFANTRY = 0;
	const int CAVALRY = 1;
	const int ARTILLERY = 2;

    Deck *d = new Deck(9);
    d->display();
    Hand *h = new Hand();
    h->draw(d->draw());
    h->display();
    h->draw(d->draw());
    h->draw(d->draw());
    h->display();
    cout << (h->exchange(INFANTRY) == 1 ? "True" : "False") << " " << (h->exchange(INFANTRY) == 1 ? "True" : "False") << endl;
}

void diceRunner()
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
}


void mapRunner()
{
	const int NUM_PROV = 13;
	Node provinces[NUM_PROV];
	provinces[0] = Node(Country("Quebec", 1, 1, 1));
	provinces[1] = Node(Country("New Brunswick", 1, 1, 1));
	provinces[2] = Node(Country("Prince Edward Island", 1, 1, 1));
	provinces[3] = Node(Country("Nova Scotia", 1, 1, 1));
	provinces[4] = Node(Country("Ontario", 1, 1, 1));
	provinces[5] = Node(Country("Manitoba", 1, 1, 1));
	provinces[6] = Node(Country("Saskatchewan", 1, 1, 1));
	provinces[7] = Node(Country("Alberta", 1, 1, 1));
	provinces[8] = Node(Country("British Columbia", 1, 1, 1));
	provinces[9] = Node(Country("Yukon Territory", 1, 1, 1));
	provinces[10] = Node(Country("Northwest Territories", 1, 1, 1));
	provinces[11] = Node(Country("Nunavut", 1, 1, 1));
	provinces[12] = Node(Country("Newfoundland and Labrador", 1, 1, 1));

	Graph g(NUM_PROV, provinces);
	Node adjListQC[] = { provinces[1], provinces[4], provinces[12] };
	g.getVectorOfNodes()[0].setAdjList(adjListQC);
	
	// g.addEdge(provinces[0], provinces[1]);
	// g.addEdge(provinces[0], provinces[3]);
	// g.addEdge(provinces[1], provinces[4]);
	cout << g.getVectorOfNodes()[0].getAdjList();

	cout << g;
}

void playerRunner()
{
    Hand* testHand = new Hand();
    int arr[] = {1, 2, 3, 4};
    std::list<int> testList (arr, arr + sizeof(arr) / sizeof(int) );
    Player* user = new Player(testHand, &testList);
	
	cout << "==== Perform dice rolls ====" << endl;
	cout << "With 1 die: " << user->roll(1) << endl;
	cout << "With 2 die: " << user->roll(2) << endl;
	cout << "With 3 die: " << user->roll(3) << endl;
	cout << endl;
	
	cout << "==== Perform Reinforce procedure ====" << endl;
	user->reinforce();
	cout << endl;

	cout << "==== Perform Attack procedure ====" << endl;
	user->attack();
	cout << endl;

	cout << "==== Perform Fortify procedure ====" << endl;
	user->fortify();
	cout << endl;
}

// Driver Runner
int main() 
{
	cout << "=============== PART 1 (MAP) - Driver ===============" << endl;
	mapRunner();
	cout << endl;
	
	/*
	cout << "=============== PART 3 (DICE)- Driver ===============" << endl;
	diceRunner();
	cout << endl;

	cout << "=============== PART 4 (PLAYER)- Driver ===============" << endl;
	playerRunner();
	cout << endl;

	cout << "=============== PART 5 (CARDS)- Driver ===============" << endl;
	cardsRunner();
	cout << endl;
	*/
	return 0;
}