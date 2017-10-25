#pragma once
#include "../include/map.h"
#include <map>

class Opener {
private:
	string fileName;
public:
	Opener(string file);
	vector<string> readLines();
};

class Parser {
private:
	//Private members of Parser
	vector<Node>* nodes;
	Graph* graph;
	map<string, Graph>* continents;
	bool error;
	//Helper methods
	bool graphIsConnected();
	bool continentsAreConnected();
public:
	//Constructors
	Parser();
	Parser(string fileName);
	//Destructors
	//~Parser();
	//Accessor methods
	Graph* getGraph();
	map<string, Graph>* getContinents();
	//Public methods
	bool mapIsValid();
	void displayContinents();
};
