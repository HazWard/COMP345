#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <functional>
#include "../include/reader.h"

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
		return std::vector<string>();
	}

	vector<string> lines;
	string line;

	while (getline(input, line)) {
        line.erase(
                std::remove_if(line.begin(), line.end(),
                               [](char c) -> bool
                               {
                                   return std::isspace<char>(c, std::locale::classic());
                               }),
                line.end());
		if (line.empty()) {
			continue;
		}
		//cout << line;
		lines.push_back(line);
	}

	return lines;
}

Parser::Parser(){}

//Constructor for the Parser, which will instantiate pointers to the map(graph) and a list of all the continents within it
Parser::Parser(string fileName) {
	Opener mapFile(fileName);

	nodes = new vector<Node>;
	continents = new vector<Continent*>;

	tempContinents = new map<string,Graph>;

    vector<string> lines = mapFile.readLines();

	error = lines.empty();
	if (!error) {
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
				nodes->reserve(lines.size() - territoryStart); //We now know the maximum number of countries inside the graph so we allocate the appropriate memory to it
				break;
			}
			vector<string> lineData = split(lines[i], '='); //splitting current line on '='
			string nameContinent = lineData[0];
			int bonusContinent = stoi(lineData[1]);
			continents->push_back(new Continent(nameContinent, bonusContinent));

			tempContinents->insert(std::pair<string, Graph>(lineData[0], Graph()));
		}
		//If we didn't find [Territories] within any lines, then the file is invalid
		if (territoryStart == -1)
		{
			cout << "The file " << fileName << " is not a valid .map file. We cannot create a Parser object." << endl;
		}

		//We add every node from the file to our vector of nodes
		for (int i = territoryStart; i < lines.size(); i++)
		{
			vector<string> lineData = split(lines[i], ','); //splitting current line on ','
			nodes->push_back(Node(Country(lineData[0], lineData[3], 0)));

			//Adding this territory to the continents
			map<string, Graph>::reverse_iterator rit;
			for (rit = tempContinents->rbegin(); rit != tempContinents->rend(); ++rit)
			{
				if (lineData[3] == rit->first)
				{
					Graph* graph_current_cont = &(rit->second);
					Node temp = Node(Country(lineData[0], lineData[3], 0));
					graph_current_cont->addNode(temp);
				}
			}
		}

		//we create the graph from the number of nodes and the content of the vector of nodes.
		graph = new Graph(nodes->size(), *nodes);

		//We have to assign each node to a continent, and create each node's adjacency list.
		//That requires to loop againj through the territories inside the file.
		for (int i = territoryStart; i < lines.size(); i++) {
			Node *currentNode = new Node();
			vector<string> lineData = split(lines[i], ','); //splitting current line on ','

			//Getting a pointer to the node on the current line. We will create its adjacency list.
			for (int k = 0; k < (*graph->getVectorOfNodes()).size(); k++) {
				if ((*graph->getVectorOfNodes())[k]->getCountry().getName() == lineData[0]) {
					currentNode = (*graph->getVectorOfNodes())[k];
					break;
				}
			}
			if (!currentNode->getCountry().getName().empty()) {
				Graph* tempCont = nullptr;
				string tempContName;
				map<string, Graph>::reverse_iterator rit;
				for (rit = tempContinents->rbegin(); rit != tempContinents->rend(); ++rit)
				{
					if (currentNode->getCountry().getContinent() == rit->first)
					{
						tempCont = &(rit->second);
						tempContName = rit->first;
					}
				}

				bool currentContinentIsValid = (tempCont != nullptr && !tempContName.empty());
				for (int j = 4; j < lineData.size(); j++) {
					for (int k = 0; k < (*graph->getVectorOfNodes()).size(); k++) {
						if ((*graph->getVectorOfNodes())[k]->getCountry().getName() == lineData[j]) {
							Node *add = (*graph->getVectorOfNodes())[k];
							graph->addEdge(currentNode, add);
						}
					}
					if (currentContinentIsValid)
					{
						vector<Node*>* nodesInCurrentContinent = tempCont->getVectorOfNodes();
						for (int k = 0; k < nodesInCurrentContinent->size(); k++)
						{
							if ((*nodesInCurrentContinent)[k]->getCountry().getName() == lineData[j])
							{
								Node* add = (*nodesInCurrentContinent)[k];
								tempCont->addEdge(currentNode, add);
							}
						}
					}
				}
				//Getting a pointer to the continent of the node on the current line. We will add it to that continent's vector of pointers to nodes.
				Continent *currentContinent = nullptr;
				for (int j = 0; j < (*graph->getVectorOfNodes()).size(); j++) {
					if (currentNode->getCountry().getContinent() == (*continents)[j]->getName()) {
						currentContinent = (*continents)[j];
						break;
					}
				}
				if (currentContinent != nullptr) {
					//We add the current node to the corresponding continent
					currentContinent->addNode(currentNode);
				}
			}
		}
	}
}

//-- ACCESSOR METHODS --
Graph* Parser::getGraph() {
	return graph;
}

vector<Continent*>* Parser::getContinents() { return continents; }

//Both the following methods are helper methods that are useful to determine whether or not the map is valid
//This method checks if the graph is strongly connected
bool Parser::graphIsConnected()
{
	return graph->isGraphConnected();
}
//This method checks if the continents within the graph are all strongly connected
bool Parser::continentsAreConnected()
{
	map<string, Graph>::reverse_iterator rit;
	for (rit = tempContinents->rbegin(); rit != tempContinents->rend(); ++rit)
	{
		//If at least one continent is not completely connected, it will return false
		if (rit->second.isGraphConnected() == false)
			return false;
	}
	//if all tests of connected continents worked, that means that all contients are indeed connected.
	return true;
}
/*The definition of a valid map is a map where all the nodes(countries) are strongly connected
 and where all the continents within the map are also strongly connected.
Therefore, the following method returns whether or not the map is valid,
 while also checking for any errors encountered during parsing.*/
bool Parser::mapIsValid()
{
	return !error && graphIsConnected() && continentsAreConnected();
}
