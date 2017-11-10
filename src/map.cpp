
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include "../include/map.h"


using namespace std;

bool Country::operator == (Country& c)const
{
	//Overloading the == operator for Country objects, equivalent of .equals() in Java
	return (this->name == c.name);
}

bool Country::operator==(Country c)const
{
    //Overloading the == operator for Country objects, equivalent of .equals() in Java
	return this->name == c.name;
}

/*
std::ostream& operator<< (std::ostream& stream, Country& c)
{

	//Overloading the << operator for use in printing Country objects, equivalent of toString in Java

	return stream << "\tInfo of country:" << endl <<
		"\tName: " << c.getName() << endl <<
		"\tContinent it belongs to: " << c.getContinent() << endl <<
		"\tNumber of armies: " << c.getNbrArmies() << endl;
}*/

//The << operator is overloaded for Country to be able to print Country objects
std::ostream& operator<< (std::ostream& stream, Country c)
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

//Full parameterized constructor
Country::Country(string n, string cont, int nbrArm) : name(n), continent(cont), nbrArmies(nbrArm) {}

//More lightweight default constructor, actually used for loading map files
Country::Country(string n) : name(n) {}

//-- ACCESSOR METHODS --
string Country::getName() { return name; }

string Country::getContinent() { return continent; }

int Country::getNbrArmies() { return nbrArmies; }

//-- MUTATOR METHODS --
void Country::setNbrArmies(int na) { this->nbrArmies = na; }

//Default constructor for Node class
Node::Node() : country(), adjList(vector<Node *>()), visited(false) {}

//Partial parameterized Constructor
Node::Node(Country c) : country(c), adjList(vector<Node *>()), visited(false) {}

//Full parametrized Constructor, not used in current implementation
Node::Node(Country c, vector<Node *> al) : country(c), adjList(al), visited(false) {}

//-- ACCESSOR METHODS --
Country Node::getCountry() { return country; }

vector<Node *> Node::getAdjList() { return adjList; }

bool Node::isVisited() { return visited; }

Country* Node::getPointerToCountry() const { return const_cast<Country*>(&country); }

//-- MUTATOR METHODS --
void Node::setAdjList(vector<Node *> newAdjList) {
    if (!adjList.empty())
        adjList.clear();
    for (size_t i = 0; i < newAdjList.size(); i++) {
        adjList.push_back(newAdjList[i]);
    }
}

void Node::setVisited(bool v) { this->visited = v; }

//Method used to add a pointer to a node to the ajacency list of this node
void Node::addNode(Node *n)
{
	for (size_t i = 0; i < adjList.size(); i++)
	{
		//If the country is already in the adjacency list, we don't add it to avoid duplicates
		if (adjList[i]->getCountry() == n->getCountry())
			return;
	}
	this->adjList.push_back(n);
}

bool operator<(const Node &lhs, const Node &rhs){
	return lhs.getPointerToCountry()->getNbrArmies() < rhs.getPointerToCountry()->getNbrArmies();
}

//The << operator is overloaded for Country to be able to print Country objects
std::ostream& operator << (std::ostream& stream, Node& n)
{
	/*
	Overloading the << operator for Node class, used to print out information about Nodes
	*/
	stream << "Info of the node:\n" << n.country;
	stream << "Adjacent nodes: ";

    for (int i = 0; i < n.adjList.size(); i++) {
        stream << n.adjList[i]->getCountry().getName();
        if (i < n.adjList.size() - 1)
            stream << " -> ";
    }
    stream << "\n";
    return stream;
}

//Default constructor for Graph
Graph::Graph() : nbrVert(0) { }

//Parameterized constructor for Graph.
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

//Method to add a node to the graph. Used when we use the default constructor for Graph and add nodes to it
void Graph::addNode(Node& n)
{
	vectorOfNodes.push_back(n);
	nbrVert = vectorOfNodes.size();
}

//Method to add an edge between two nodes in the graph.
//It uses the method addNode() of each node that are to be connected.
void Graph::addEdge(Node *n1, Node *n2)
{
	/*
	Creating a new edge by connecting two nodes together.
	*/
	for (size_t i = 0; i < vectorOfNodes.size(); i++)
	{
		if (vectorOfNodes[i].getPointerToCountry()->getName() == n1->getPointerToCountry()->getName())
		{
			vectorOfNodes[i].addNode(n2);
		}
		if (vectorOfNodes[i].getPointerToCountry()->getName() == n2->getPointerToCountry()->getName())
		{
			vectorOfNodes[i].addNode(n1);
		}
	}
}

//--ACCESSOR METHODS--
vector<Node*>* Graph::getVectorOfNodes()
{
	vector<Node*>* nodes = new vector<Node*>();
	for(int i = 0; i < nbrVert; i++)
	{
		nodes->push_back(&vectorOfNodes[i]);
	}
	return nodes;
}

int Graph::getNbrCountries() { return nbrVert; };

//The << operator is overloaded for Graph to be able to print Graph objects
//it will print each node and its adjacency list to have a better representation of the Graph
std::ostream& operator << (std::ostream& stream, Graph& g)
{
	/*
	Overloading << operator to print out Graph objects.
	*/
	stream << "Info of this graph:\n";
	stream << "Number of vertices: " << g.nbrVert << endl;
	stream << "Array of nodes:\n";
	for (size_t i = 0; i < g.vectorOfNodes.size(); i++)
	{
		stream << g.vectorOfNodes[i] << "\n";
	}
	return stream;
}

//Method used to check whether or not the Graph is a connected Graph.
//If the recursive helper method visitAdjacentNodes() fails to mark all the nodes as visited,
//then some nodes are not accessible from the initial node, meaning the Graph is not strongly connected and we return false.
//A map has to be strongly connected so this method has to return true.
//Else, the map is invalid.
bool Graph::isGraphConnected()
{
	/*
	Important method for validating a Graph, checks if the Graph is strongly connected in the
	sense that any given node can reach all other nodes in the Graph. Uses visitAdjacentNodes.
	*/
	bool graphIsConnected = true;
    if(vectorOfNodes.empty())
        return false;
    vector<Node *> initialAdjListNode = vectorOfNodes[0].getAdjList();

    if (initialAdjListNode.empty())
        return false;

	visitAdjacentNodes(initialAdjListNode);

    for (size_t i = 0; i < vectorOfNodes.size(); i++) {
        //If any vector has not been visited, it means it is not connected to at least one node, so the graph is not strongly connected
        if (!vectorOfNodes[i].isVisited()) {
            graphIsConnected = false;
            break;
        }
    }
    //Reinitialize the visited member of each node in our graph
    for (size_t i = 0; i < vectorOfNodes.size(); i++) {
        vectorOfNodes[i].setVisited(false);
    }
    return graphIsConnected;
}

//Recursive helper method used to visit all the nodes accessible from the initial node and mark them as visited.
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

bool Graph::areConnectedByEdge(Node* n1, Node* n2) {
    for (int i = 0; i < n1->getAdjList().size(); i++) {
        if (n1->getAdjList()[i] == n2)
            return true;
    }
    return false;
}

Continent::Continent(): name(""), bonus(0), nodesInContinent(vector<Node*>())
{ }
Continent::Continent(string n, int b): name(n), bonus(b), nodesInContinent(vector<Node*>())
{ }
void Continent::addNode(Node* n)
{
	nodesInContinent.push_back(n);
}

std::ostream& operator << (std::ostream& stream, Continent& c)
{
	/*
	Overloading << operator to print out Continent objects.
	*/
	stream << c.getName() << ":\n";
	stream << "Bonus: " << c.getBonus() << endl;
	stream << "Number of countries: " << c.getNodesInContinent()->size() << endl;
	stream << "List of countries:\n";
	for (size_t i = 0; i < c.getNodesInContinent()->size(); i++) {
		stream << "\t" << (*c.getNodesInContinent())[i]->getCountry().getName() << endl;
	}
	stream << endl;
	return stream;
}