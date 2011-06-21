#ifndef EDGE_H
#define EDGE_H

#include "Node.h"

class Edge
{
public:

	Node* node1;
	Node* node2;
	int distance;

	int LineID;

	Edge(Node* node1, Node* node2, int distance);

	bool Connects(Node* node1, Node* node2);

};

#endif
