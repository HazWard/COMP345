#include <iostream>
#include "../include/graph.h"

using namespace std;

Graph::Graph(int v) : nbrVert(v)
{
	vectorOfNodes.reserve(v);
}

Graph::Graph(int v, vector<Node> nodes) : nbrVert(v)
{
	if (v == nodes.size())
	{
		//vectorOfNodes.reserve(v);
		for (int i = 0; i < nodes.size(); i++)
			vectorOfNodes = nodes;
			//vectorOfNodes.push_back(nodes[i]);
	}
	else
		cout << "error" << endl;
}
Graph::Graph(int v, Node* arrayOfNodes) : nbrVert(v)
{
	for (int i = 0; i < v; i++)
		vectorOfNodes.push_back(arrayOfNodes[i]);
}
void Graph::addEdge(Node& n1, Node& n2)
{
	for (int i = 0; i < vectorOfNodes.size(); i++)
	{
		if (vectorOfNodes[i].getCountry().getName() == n1.getCountry().getName())
		{
			vectorOfNodes[i].addNode(n2);
		}
		if (vectorOfNodes[i].getCountry().getName() == n2.getCountry().getName())
		{
			vectorOfNodes[i].addNode(n1);
		}
	}
}
vector<Node> Graph::getVectorOfNodes()
{
	return vectorOfNodes;
}

std::ostream& operator<<(std::ostream& stream, Graph& g)
{
	string output;
	output.append("Info of this graph:\n");
	output.append("Number of vertices: " + to_string(g.nbrVert) + "\n");
	output.append("Array of nodes:\n");
	std::stringstream buffer;
	for (int i = 0; i < g.vectorOfNodes.size(); i++)
	{
		buffer << g.vectorOfNodes[i] << "\n";
	}
	return stream << output << buffer.rdbuf();
}