#pragma once

using namespace std;

#include <string>

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
	bool operator==(Country& rhs)const;
	bool operator==(Country rhs)const;
	friend std::ostream& operator <<(std::ostream& stream, Country& c);
	string getName();
	string getContinent();
	int getNbrArmies();
	void setNbrArmies(int na);
};

class Node
{
private:
	Country country;
	vector<Node*> adjList;
	bool visited;
public:
	Node();
	Node(Country c);
	Node(Country c, vector<Node*> adjList);
	Country getCountry();
	vector<Node*> getAdjList();
	bool isVisited();
	void addNode(Node& n);
	void setAdjList(vector<Node*> newAdjList);
	void setVisited(bool v);
	friend std::ostream& operator <<(std::ostream& stream, Node& n);
};

class Graph
{
private:
	int nbrVert;
	vector<Node> vectorOfNodes;
	void visitAdjacentNodes(vector<Node*> adjListNode);
	//void visitAdjacentNodesContinent(vector<Node*> adjListNode, string continent);
public:
	Graph();
	Graph(int v);
	Graph(int v, vector<Node>& arrayOfNodes);
	Graph(int v, Node* arrayOfNodes);
	//void addEdges(Node masterNode, Node listOfNodes[]);
	void addNode(Node &n);
	void addEdge(Node& n1, Node& n2);
	vector<Node>* getVectorOfNodes();
	bool isGraphConnected();
	friend std::ostream& operator <<(std::ostream& stream, Graph& g);
};
