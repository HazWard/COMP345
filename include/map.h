#pragma once
#include <string>
#include <vector>

using namespace std;

//This class creates Country objects.
//A Country is defined as a territory in Risk
//(could be a country, could be a subset of a continent like North America, a province like Quebec, a territory like Yukon or a state like Virginia)
class Country
{
private:
    string name;
    string continent; //the continent the Country belongs to
    int nbrArmies;
public:
    Country();
    Country(string n);
    Country(string n, string cont, int nbrArm);
    bool operator==(Country &rhs) const;
    bool operator==(Country rhs) const;
    friend std::ostream &operator<<(std::ostream &stream, Country &c);
    string getName();
    string getContinent();
    int getNbrArmies();
    void setNbrArmies(int na);
};
//This class creates Node objects, that contain a Country and the list of nodes adjacent to it.
class Node
{
private:
    Country country;
    vector<Node*> adjList;
    bool visited;
public:
    Node();
    Node(Country c);
    Node(Country c, vector<Node *> adjList);
    Country getCountry();
    vector<Node*> getAdjList();
    Country * getPointerToCountry()const;
    bool isVisited();
    void addNode(Node *n);
    void setAdjList(vector<Node*> newAdjList);
    void setVisited(bool v);
    friend std::ostream &operator<<(std::ostream &stream, Node &n);
    friend bool operator<(const Node &lhs, const Node &rhs);
};

//This class creates a graph used as the map of our Risk game
class Graph
{
private:
	//Private members of the class
	int nbrVert;
	vector<Node> vectorOfNodes;
	//Helper method
	void visitAdjacentNodes(vector<Node*> adjListNode);
public:
	//Constructors
	Graph();
	Graph(int v, vector<Node>& arrayOfNodes);
	//Accessor methods (no mutator methods since the graph will not change after the initial creation
	vector<Node*>* getVectorOfNodes();
	int getNbrCountries();
	//Methods to add
	void addNode(Node &n);
	void addEdge(Node *n1, Node *n2);
    bool areConnectedByEdge(Node* n1, Node* n2);
	//bool areConnectedThroughOwned(int source, int destination);
	bool isGraphConnected();
	friend std::ostream& operator <<(std::ostream& stream, Graph& g);
};
class Continent
{
private:
	string name;
	int bonus;
	Graph continent;
	vector<Node*> nodesInContinent;
public:
	Continent();
	Continent(string n, int b);
	string getName() { return name; }
	int getBonus() { return bonus; }
	vector<Node*>* getNodesInContinent() { return &nodesInContinent; }
	void addNode(Node* n);
	friend std::ostream& operator <<(std::ostream& stream, Continent& c);
};