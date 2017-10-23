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
	vector<Node>* nodes;
	Graph* graph;
	map<string, Graph>* continents;
	bool error;
public:
	Parser();
	Parser(string fileName);
	Graph* getGraph();
	map<string, Graph>* getContinents();
	bool graphIsConnected();
	bool continentsAreConnected();
	bool mapIsValid();
	void displayContinents();
};
