#pragma once

#include "stdafx.h"
#include "Node.h"
#include <iostream>
#include <sstream>
#include <vector>

class Graph
{
private:
	int nbrVert;
	vector<Node> vectorOfNodes;
public:
	Graph(int v);
	Graph(int v, vector<Node> arrayOfNodes);
	Graph(int v, Node* arrayOfNodes);
	//void addEdges(Node masterNode, Node listOfNodes[]);
	void addEdge(Node& n1, Node& n2);
	vector<Node> getVectorOfNodes();
	friend std::ostream& operator <<(std::ostream& stream, Graph& g);
};
