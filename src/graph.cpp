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

int main()
{
    const int NUM_PROV = 13;
    Node provinces[NUM_PROV];
    provinces[0] = Node(Country("Quebec", 1, 1, 1));
    provinces[1] = Node(Country("New Brunswick", 1, 1, 1));
    provinces[2] = Node(Country("Prince Edward Island", 1, 1, 1));
    provinces[3] = Node(Country("Nova Scotia", 1, 1, 1));
    provinces[4] = Node(Country("Ontario", 1, 1, 1));
    provinces[5] = Node(Country("Manitoba", 1, 1, 1));
    provinces[6] = Node(Country("Saskatchewan", 1, 1, 1));
    provinces[7] = Node(Country("Alberta", 1, 1, 1));
    provinces[8] = Node(Country("British Columbia", 1, 1, 1));
    provinces[9] = Node(Country("Yukon Territory", 1, 1, 1));
    provinces[10] = Node(Country("Northwest Territories", 1, 1, 1));
    provinces[11] = Node(Country("Nunavut", 1, 1, 1));
    provinces[12] = Node(Country("Newfoundland and Labrador", 1, 1, 1));

    Graph g(NUM_PROV, provinces);
    Node adjListQC[] = { provinces[1], provinces[4], provinces[12] };
    g.getVectorOfNodes()[0].setAdjList(adjListQC);
    
    // g.addEdge(provinces[0], provinces[1]);
    // g.addEdge(provinces[0], provinces[3]);
    // g.addEdge(provinces[1], provinces[4]);
    cout << g.getVectorOfNodes()[0].getAdjList();

	cout << g;
	
	return 0;
}