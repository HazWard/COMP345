
#include <sstream>
#include <iostream>
#include <vector>
#include "../include/map.h"


using namespace std;

bool Country::operator == (Country& c)const
{
	/*
	Overloading the == operator for Country objects, equivalent of .equals() in Java
	*/
	if (this->name == c.name)
		return true;
	return false;
}

bool Country::operator==(Country c)const
{
	if (this->name == c.name)
		return true;
	return false;
}

std::ostream& operator<< (std::ostream& stream, Country& c)
{
	/*
	Overloading the << operator for use in printing Country objects, equivalent of toString in Java
	*/
	return stream << "\tInfo of country:" << endl <<
		"\tName: " << c.getName() << endl <<
		"\tContinent it belongs to: " << c.getContinent() << endl <<
		"\tNumber of armies: " << c.getNbrArmies() << endl;
}

//Default constructor, never used but necessary
Country::Country() : name(""), nbrArmies(0) {}

Country::Country(string n, string cont, int nbrArm) : name(n), continent(cont), nbrArmies(nbrArm) {}

//More lightweight default constructor, actually used for loading map files
Country::Country(string n) : name(n) {}

//-- ACCESSOR METHODS --
string Country::getName() { return name; }

string Country::getContinent() { return continent; }

int Country::getNbrArmies() { return nbrArmies; }

//-- MUTATOR METHODS --
void Country::setNbrArmies(int na) { nbrArmies = na;  }

//Default constructor for Node class
Node::Node() : country(), adjList(vector<Node*>()), visited(false) {}

//Partial parameterized Constructor
Node::Node(Country c) : country(c), adjList(vector<Node*>()), visited(false) {}

//Full parametrized Constructor, not used in current implementation
Node::Node(Country c, vector<Node*> al) : country(c), adjList(al), visited(false) {}

//-- ACCESSOR METHODS --
Country Node::getCountry() { return country; }

vector<Node*> Node::getAdjList() { return adjList; }

bool Node::isVisited() { return visited; }

//-- MUTATOR METHODS --
void Node::setAdjList(vector<Node*> newAdjList)
{
	if (!adjList.empty())
		adjList.clear();
	for (size_t i = 0; i < newAdjList.size(); i++)
	{
		adjList.push_back(newAdjList[i]);
	}
}

void Node::setVisited(bool v) { this->visited = v; }

void Node::addNode(Node& n)
{
	for (size_t i = 0; i < adjList.size(); i++)
	{
		//If the country is already in the adjacency list, we don't add it to avoid duplicates
		if (adjList[i]->getCountry() == n.getCountry())
			return;
	}
	this->adjList.push_back(&n);
}

std::ostream& operator << (std::ostream& stream, Node& n)
{
	/*
	Overloading the << operator for Node class, used to print out information about Nodes
	*/
	stream << "Info of the node:\n" << n.country;
	stream << "Adjacent countries: ";

	for (int i = 0; i < n.adjList.size(); i++)
	{
		stream << n.adjList[i]->getCountry().getName();
		if (i < n.adjList.size() - 1)
			stream << " -> ";
	}
	stream << "\n";
	return stream;
}

//Constructor for Graph 
Graph::Graph() : nbrVert(0) { }

Graph::Graph(int v) : nbrVert(v) { vectorOfNodes.reserve(v); }

Graph::Graph(int v, vector<Node>& nodes) : nbrVert(v)
{
	/*
	This constructor is passed a vector of nodes, and a number of nodes we are adding
	to the graph. If the size of the vector is not equal to the number of nodes being added,
	something went wrong and so the the vectorOfNodes object will not be set.
	*/
	try {
		if (v != nodes.size())
			throw new std::exception;
		vectorOfNodes = nodes;
	}
	catch (const std::exception& e) {
		cout << "The number of nodes in the node vector is not as expected." << endl;
	}
}

Graph::Graph(int v, Node* arrayOfNodes) : nbrVert(v)
{
	/*
	This constructor is passed a pointer to an array of nodes, and a number of nodes we are adding
	to the graph.
	CURRENTLY DEPRECATED DUE TO LIMITATIONS OF ARRAYS
	*/
	try {
		for (int i = 0; i < v; i++)
			vectorOfNodes.push_back(arrayOfNodes[i]);
	}
	catch (const std::exception& e) {
		cout << "The number of nodes in the node vector is not as expected." << endl;
	}
}

void Graph::addNode(Node& n)
{
	vectorOfNodes.push_back(n);
	nbrVert = vectorOfNodes.size();
}

void Graph::addEdge(Node& n1, Node& n2)
{
	/*
	Creating a new edge by connecting two nodes together.
	*/
	for (size_t i = 0; i < vectorOfNodes.size(); i++)
	{
		if (vectorOfNodes[i].getCountry() == n1.getCountry())
		{
			vectorOfNodes[i].addNode(n2);
		}
		if (vectorOfNodes[i].getCountry() == n2.getCountry())
		{
			vectorOfNodes[i].addNode(n1);
		}
	}
}

//--ACCESSOR METHODS--

//Not used in current implementation
vector<Node>* Graph::getVectorOfNodes() { return &vectorOfNodes; }

std::ostream& operator << (std::ostream& stream, Graph& g)
{
	/*
	Overloading << operator to print out Graph objects.
	*/
	string output;
	output.append("Info of this graph:\n");
	output.append("Number of vertices: " + to_string(g.nbrVert) + "\n");
	output.append("Array of nodes:\n");
	std::stringstream buffer;
	for (size_t i = 0; i < g.vectorOfNodes.size(); i++)
	{
		buffer << g.vectorOfNodes[i] << "\n";
	}
	return stream << output << buffer.rdbuf();
}

bool Graph::isGraphConnected()
{
	/*
	Important method for validating a Graph, checks if the Graph is strongly connected in the
	sense that any given node can reach all other nodes in the Graph. Uses visitAdjacentNodes.
	*/
	bool graphIsConnected = true;
    if(vectorOfNodes.empty())
        return false;
	vector<Node*> initialAdjListNode = vectorOfNodes[0].getAdjList();
	visitAdjacentNodes(initialAdjListNode);

	if (initialAdjListNode.empty())
		return false;

	for (size_t i = 0; i < vectorOfNodes.size(); i++)
	{
		//If any vector has not been visited, it means it is not connected to at least one node, so the graph is not strongly connected
		if (!vectorOfNodes[i].isVisited())
		{
			graphIsConnected = false;
			break;
		}
	}
	//Reinitialize the visited member of each node in our graph
	for (size_t i = 0; i < vectorOfNodes.size(); i++)
	{
		vectorOfNodes[i].setVisited(false);
	}
	return graphIsConnected;
}

void Graph::visitAdjacentNodes(vector<Node*> adjListNode)
{
	/*
	Recurive method that visits all the adjacent nodes of the initial passed adjListNode.
	It marks every visited Nodes visited.
	It recursuvely looks at every adjacent nodes of every visited Nodes.
	*/
	if (!adjListNode.empty())
	{
		for (size_t i = 0; i < adjListNode.size(); i++)
		{
			if (!adjListNode[i]->isVisited())
			{
				for (size_t j = 0; j < vectorOfNodes.size(); j++)
				{
					if (adjListNode[i]->getCountry() == vectorOfNodes[j].getCountry())
					{
						if (!vectorOfNodes[j].isVisited())
						{
							vectorOfNodes[j].setVisited(true);
							visitAdjacentNodes(vectorOfNodes[j].getAdjList());
						}
					}
				}
			}
		}
	}
}
