#include <iostream>
#include <string>
#include "../include/node.h"

using namespace std;

Node::Node() : country(), adjList(NULL)
{

}
Node::Node(Country c) : country(c), adjList(NULL)
{

}
Node::Node(Country c, vector<Node*> al) : country(c), adjList(al)
{

}
Country Node::getCountry()
{
	return country;
}
vector<Node*> Node::getAdjList()
{
	return adjList;
}
void Node::setAdjList(Node listOfNodes[])
{
	//We reset the adjacency list back to the start:
	adjList.clear();
	adjList.reserve(3);
	//We push all of the nodes from the array into the adjacency list
	for (int i = 0; i < 3; i++)
	{
		this->adjList.push_back(&listOfNodes[i]);
	}
}
void Node::addNode(Node& n)
{
	this->adjList.push_back(&n);
}


std::ostream& operator<<(std::ostream& stream, Node& n)
{
	std::stringstream buffer;
	buffer << "Info of the node:\n" << n.country;
	buffer << "Adjacent countries: ";
	for (int i = 0; i < n.adjList.size(); i++)
	{
		buffer << n.adjList[i]->getCountry().getName();
		if (i < n.adjList.size() - 1)
			buffer << " -> ";
	}
	buffer << "\n";
	return stream << buffer.rdbuf();
}
