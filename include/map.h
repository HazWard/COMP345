#pragma once

using namespace std;

class Country
{
private:
    string name;
    int nbrArmies;
public:
    Country();
    Country(string n);
    Country(string n, int nbrArm);
    bool operator==(Country& rhs)const;
    bool operator==(Country rhs)const;
    friend std::ostream& operator <<(std::ostream& stream, Country& c);
    string getName();
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

class Continent
{
private:
	string name;
    vector<Node*> nodes; //list of nodes within this continent
    int nbrNodes; //number of nodes in the current continent
public:
    Continent();
    Continent(string name);
    Continent(string name, const vector<Node*> &newNodes);
    Continent(string name, vector<Node*> newNodes, int nbrNodes);
    int getNbrNodes();
    vector<Node*>* getNodes();
    string getName();
    bool isContinentConnected();
    void visitAdjacentNodesContinent(int index);
    void setNbrNodes();
    void setNodes(vector<Node*> newNodes);
    void addNode(Node* n);
    friend std::ostream& operator <<(std::ostream& stream, Continent& c);
};

class Graph
{
private:
    int nbrVert;
    vector<Node> vectorOfNodes;
    void visitAdjacentNodes(vector<Node*> adjListNode);
    void visitAdjacentNodesContinent(vector<Node*> adjListNode, string continent);
public:
    Graph(int v);
    Graph(int v, vector<Node> arrayOfNodes);
    Graph(int v, Node* arrayOfNodes);
    //void addEdges(Node masterNode, Node listOfNodes[]);
    void addEdge(Node& n1, Node& n2);
    vector<Node> getVectorOfNodes();
    bool isGraphConnected();
    friend std::ostream& operator <<(std::ostream& stream, Graph& g);
};
