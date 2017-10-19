
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>
#include "../include/reader.h"
#include "../include/map.h"

using namespace std;

const string MAPS_FOLDER = "../maps/";

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

vector<string> Opener::readLines() {
	ifstream input;
	input.open(fileName.c_str());

	if (!input) {
		cout << "File " << fileName << " could not be opened." << endl;
		throw new std::exception;
	}

	vector<string> lines;
	string line;

	while (getline(input, line)) {
		if (line.empty()) {
			continue;
		}
		//cout << line;
		lines.push_back(line);
	}

	return lines;
}

//Constructor for the Parser, which will instantiate pointers to the map(graph) and a list of all the continents within it
Parser::Parser(string fileName) {
	Opener mapFile(fileName);

	nodes = new vector<Node>;
	continents = new map<string,Graph>;

	vector<string> lines = mapFile.readLines();

	int territoryStart = -1;
	int continentStart = -1;

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i].find("[Continents]") != std::string::npos)
		{
			continentStart = (i + 1);
			break;
		}
	}
	//If we didn't find [Continents] within any lines, then the file is invalid
	if (continentStart == -1)
	{
		cout << "The file " << fileName << " is not a valid .map file. We cannot create a Parser object." << endl;
	}
	for (int i = continentStart; i < lines.size(); i++)
	{
		if (lines[i].find("[Territories]") != std::string::npos)
		{
			territoryStart = (i + 1);
			break;
		}
		vector<string> lineData = split(lines[i], '='); //splitting current line on '='
		continents->insert(pair<string, Graph>(lineData[0], Graph(0)));
	}
	//If we didn't find [Territories] within any lines, then the file is invalid
	if (territoryStart == -1)
	{
		cout << "The file " << fileName << " is not a valid .map file. We cannot create a Parser object." << endl;
	}

	for (int i = territoryStart; i < lines.size(); i++)
	{
		vector<string> lineData = split(lines[i], ','); //splitting current line on ','
		nodes->push_back(Node(Country(lineData[0], lineData[3], 0)));

		map<string, Graph>::reverse_iterator rit;
		for (rit = continents->rbegin(); rit != continents->rend(); ++rit)
		{
			if (lineData[3] == rit->first)
			{
				Graph* graph_current_cont = &(rit->second);
				Node temp = Node(Country(lineData[0], lineData[3], 0));
				graph_current_cont->addNode(temp);
			}
		}
	}

	graph = new Graph(nodes->size(), *nodes);

	for (int i = territoryStart; i < lines.size(); i++) {
		Node* currentNode = new Node;
		vector<string> lineData = split(lines[i], ','); //splitting current line on ','

		for (int k = 0; k < nodes->size(); k++) {
			if ((*nodes)[k].getCountry().getName() == lineData[0]) {
				currentNode = &(*nodes)[k];
				break;
			}
		}
		if (currentNode->getCountry().getName() != "")
		{
			Graph* currentContinent = NULL;
			string currentContinentName = "";
			map<string, Graph>::reverse_iterator rit;
			for (rit = continents->rbegin(); rit != continents->rend(); ++rit)
			{
				if (currentNode->getCountry().getContinent() == rit->first)
				{
					currentContinent = &(rit->second);
					currentContinentName = rit->first;
				}
			}

			bool currentContinentIsValid = currentContinent != NULL && currentContinentName != "";
			for (int j = 4; j < lineData.size(); j++) 
			{
				for (int k = 0; k < nodes->size(); k++) 
				{
					if ((*nodes)[k].getCountry().getName() == lineData[j]) 
					{
						Node* add = &((*nodes)[k]);
						graph->addEdge(*currentNode, *add);
					}
				}
				if (currentContinentIsValid)
				{
					vector<Node>* nodesInCurrentContinent = currentContinent->getVectorOfNodes();
					for (int k = 0; k < nodesInCurrentContinent->size(); k++)
					{
						if ((*nodesInCurrentContinent)[k].getCountry().getName() == lineData[j])
						{
							Node* add = &((*nodesInCurrentContinent)[k]);
							currentContinent->addEdge(*currentNode, *add);
						}
					}
				}
			}
		}
	}
}

Graph* Parser::getGraph() { return graph; }
map<string, Graph>* Parser::getContinents() { return continents; }

bool Parser::graphIsConnected()
{
	if (graph->isGraphConnected())
		return true;
	else return false;
}
bool Parser::continentsAreConnected()
{
	map<string, Graph>::reverse_iterator rit;
	for (rit = continents->rbegin(); rit != continents->rend(); ++rit)
	{
		//If at least one continent is not completely connected, it will return false
		if (rit->second.isGraphConnected() == false)
			return false;
	}
	//if all tests of connected continents worked, that means that all contients are indeed connected.
	return true;
}
bool Parser::mapIsValid()
{
	if (graphIsConnected() && continentsAreConnected())
		return true;
	else return false;
}

int main() {

	/*We are going to try to open multiple files, and one of them is invalid (the 4th one), the others are valid maps so they should work properly.*/
	try
	{
		Parser parse1(MAPS_FOLDER + "World.map");
		/*
		Graph g1 = *(parse1.getGraph());
		map<string, Graph>* continents = parse1.getContinents();

		map<string, Graph>::reverse_iterator rit;
		for (rit = continents->rbegin(); rit != continents->rend(); ++rit)
		{
			cout << rit->first << endl;
			cout << rit->second;
		}

		//We print the full content of the first map
		cout << g1;
		*/

		cout << "Is parse1 a valid map ? : ";
		if (parse1.mapIsValid())
			cout << "Yes, both the entire map as a whole and each continent are connected.\n";
		else
			cout << "No, the graph and/or some of the continents are not strongly connected.\n";
	}
	catch (const std::exception& e)
	{
		cout << "The file World.map is not a valid .map file. We cannot create a Parser object." << endl;
	}
	try
	{
		Parser parse2(MAPS_FOLDER + "_49_ City Nights.map");
		Graph g2 = *(parse2.getGraph());
		map<string, Graph>* continents2 = parse2.getContinents();
		cout << "Is parse2 a valid map ? : ";
		if (parse2.mapIsValid())
			cout << "Yes, both the entire map as a whole and each continent are connected.\n";
		else
			cout << "No, the graph and/or some of the continents are not strongly connected.\n";
	}
	catch (const std::exception& e)
	{
		cout << "The file _49_ City Nights.map is not a valid .map file. We cannot create a Parser object." << endl;
	}

	try
	{
		Parser parse3(MAPS_FOLDER + "_H_Counterweight  World.map");
		Graph g3 = *(parse3.getGraph());
		map<string, Graph>* continents3 = parse3.getContinents();
		cout << "Is parse3 a valid map ? : ";
		if (parse3.mapIsValid())
			cout << "Yes, both the entire map as a whole and each continent are connected.\n";
		else
			cout << "No, the graph and/or some of the continents are not strongly connected.\n";
	}
	catch (const std::exception& e)
	{
		cout << "_H_Counterweight  World.map is not a valid .map file. We cannot create a Parser object." << endl;
	}
	try
	{
		Parser parse4(MAPS_FOLDER + "invalidMap.txt");
		cout << "Is parse4 a valid map ? : ";
		/*ERROR?
		if (parse4.mapIsValid())
			cout << "Yes, both the entire map as a whole and each continent are connected.\n";
		else
			cout << "No, the graph and/or some of the continents are not strongly connected.\n";*/
	}
	catch (const std::exception& e)
	{
		cout << "The file invalidMap.txt is not a valid .map file. We cannot create a Parser object." << endl;
	}
	try
	{
		Parser parse5(MAPS_FOLDER + "Bubble Plane.map");
		Graph g5 = *(parse5.getGraph());
		map<string, Graph>* continents5 = parse5.getContinents();
		cout << "Is parse5 a valid map ? : ";
		cout << parse5.continentsAreConnected() << parse5.graphIsConnected();
		if (parse5.mapIsValid())
			cout << "Yes, both the entire map as a whole and each continent are connected.\n";
		else
			cout << "No, the graph and/or some of the continents are not strongly connected.\n";
	}
	catch (const std::exception& e)
	{
		cout << "Bubble Plane.map is not a valid .map file. We cannot create a Parser object." << endl;
	}
	
	return 0;
}
