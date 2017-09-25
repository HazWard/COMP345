#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>

#include "Graph.h"
#include "Country.h"
#include "Node.h"

#include <vector>

using namespace std;

int main()
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
		/*
	g.addEdge(provinces[0], provinces[1]);
	g.addEdge(provinces[0], provinces[3]);
	g.addEdge(provinces[1], provinces[4]);*/
	cout << g.getVectorOfNodes()[0].getAdjList();

	cout << g;

	return 0;
}