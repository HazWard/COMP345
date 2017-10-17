#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "../include/reader.h"
#include "../include/map.h"
using namespace std;

template<typename Out> //Needed for the split function
void split(const std::string &s, char delim, Out result) {
	/* 
	This function helps us split lines from map files, seperating by any given delimiter.
	*/
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

vector<string> split(const string &s, char delim) {
	/*
	Called by the Graph creator, returns a line of the map where things are seperated
	by the delimiter chosen. 
	*/
	vector<string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

//Constructor for the Opener class
Opener::Opener(string file) : fileName(file) { }

vector<string> Opener::readLines(){
	ifstream input;
	input.open(fileName.c_str());

	if(!input){
		cout << "File " << fileName << " could not be opened." << endl;
		throw new std::exception;
	}

	vector<string> lines;
	string line;

	while(getline(input, line)){
		if(line.empty()){
			continue;
		}
		//cout << line;
		lines.push_back(line);		
	}

	return lines;
}

//Constructor for the Parser, which will instantiate pointers to the map(graph) and a list of all the continents within it
Parser::Parser(string fileName){
	Opener mapFile(fileName);

	nodes = new vector<Node>;
	continents = new vector<Continent>; 
	
	vector<string> lines = mapFile.readLines();

	int territoryStart = -1;
	int continentStart = -1;
	
	for(int i = 0; i < lines.size(); i++)
	{
		if(lines[i].find("[Continents]") != std::string::npos)
		{
			continentStart = (i+1);
			break;
		}
	}
	//If we didn't find [Continents] within any lines, then the file is invalid
	if(continentStart == -1)
	{
		cout << "The file " << fileName << " is not a valid .map file. We cannot create a Parser object." << endl;
	}
	for(int i = continentStart; i < lines.size(); i++)
	{
		if(lines[i].find("[Territories]") != std::string::npos)
		{
			territoryStart = (i+1);
			break;
		}
		vector<string> lineData = split(lines[i], '='); //splitting current line on '='
		//continents->push_back(Continent(lineData[0]));
	}
	//If we didn't find [Territories] within any lines, then the file is invalid
	if(territoryStart == -1)
	{
		cout << "The file " << fileName << " is not a valid .map file. We cannot create a Parser object." << endl;
	}
	
	for(int i = territoryStart; i < lines.size(); i++)
	{
		vector<string> lineData = split(lines[i], ','); //splitting current line on ','
		nodes->push_back(Node(Country(lineData[0], 0)));
	}

	graph = new Graph(nodes->size(), *nodes);

	for(int i = territoryStart; i < lines.size(); i++){
		Node* currentNode;
		vector<string> lineData = split(lines[i], ','); //splitting current line on ','

		for(int k = 0; k < nodes->size(); k++){
			if((*nodes)[k].getCountry().getName() == lineData[0]){
				currentNode = &(*nodes)[k];
				break;
			}
		}

		for(int j = 0; j < continents->size(); j++){
			if(lineData[3] == (*continents)[j].getName()){
				(*continents)[j].addNode(currentNode);
				break;
			}
		}

		for(int j = 4; j < lineData.size(); j++){
			for(int k = 0; k < nodes->size(); k++){
				if((*nodes)[k].getCountry().getName() == lineData[j]){
					Node* add = &((*nodes)[k]);
					graph->addEdge(*currentNode, *add);
				}
			}
		}
	}
}

Graph* Parser::getGraph() { return graph; }
vector<Continent>* Parser::getContinents() { return continents; }

int main() {
/*We are going to try to open multiple files, and one of them is invalid (the 4th one), the others are valid maps so they should work properly.*/
	try
	{
		Parser parse1("World.map");
		Graph g1 = *(parse1.getGraph());
		vector<Continent> continents1 = *(parse1.getContinents());
		//We print the full content of the first map
		cout << g1;
	
		cout << "Is this graph (g) connected ? : ";
   		if (g1.isGraphConnected())
        	cout << "Yes, every node is connected to one another. It is a strongly connected graph.\n";
    	else
        	cout << "No, one or more nodes are not connected to all the others.\n";
	}
	catch(const std::exception& e)
	{
		cout << "The file World.map is not a valid .map file. We cannot create a Parser object." << endl;
	}

	try
	{
		Parser parse2("_49_ City Nights.map");
		Graph g2 = *(parse2.getGraph());
		vector<Continent> continents2 = *(parse2.getContinents());
		cout << "Is this graph (g2) connected ? : ";
	    if (g2.isGraphConnected())
	        cout << "Yes, every node is connected to one another. It is a strongly connected graph.\n";
	    else
	        cout << "No, one or more nodes are not connected to all the others.\n";
	}
	catch(const std::exception& e)
	{
		cout << "The file _49_ City Nights.map is not a valid .map file. We cannot create a Parser object." << endl;
	}
	
	try
	{
		Parser parse3("_H_Counterweight  World.map");
		Graph g3 = *(parse3.getGraph());
		vector<Continent> continents3 = *(parse3.getContinents());
		cout << "Is this graph (g3) connected ? : ";
	    if (g3.isGraphConnected())
	        cout << "Yes, every node is connected to one another. It is a strongly connected graph.\n";
	    else
	        cout << "No, one or more nodes are not connected to all the others.\n";
	}
	catch(const std::exception& e)
	{
		cout << "_H_Counterweight  World.map is not a valid .map file. We cannot create a Parser object." << endl;
	}
	try
	{
		Parser parse4("invalidMap.txt");
	}
	catch(const std::exception& e)
	{
		cout << "The file invalidMap.txt is not a valid .map file. We cannot create a Parser object." << endl;
	}
	try
	{
		Parser parse5("Bubble Plane.map");
		Graph g5 = *(parse5.getGraph());
		vector<Continent> continents5 = *(parse5.getContinents());
		cout << "Is this graph (g5) connected ? : ";
	    if (g5.isGraphConnected())
	        cout << "Yes, every node is connected to one another. It is a strongly connected graph.\n";
	    else
	        cout << "No, one or more nodes are not connected to all the others.\n";
	}
	catch(const std::exception& e)
	{
		cout << "Bubble Plane.map is not a valid .map file. We cannot create a Parser object." << endl;
	}
	
	return 0;
}
