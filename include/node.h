#pragma once

#include "Country.h"
#include <vector>
#include <sstream>

class Node
{
private:
	Country country;
	vector<Node*> adjList;
public:
	Node();
	Node(Country c);
	Node(Country c, vector<Node*> adjList);
	Country getCountry();
	vector<Node*> getAdjList();
	void addNode(Node& n);
	void setAdjList(Node listOfNodes[]);
	friend std::ostream& operator <<(std::ostream& stream, Node& n);
};