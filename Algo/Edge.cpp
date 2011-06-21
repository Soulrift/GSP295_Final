#include "Edge.h"
#include "../Global.h"

Edge::Edge(Node* node1, Node* node2, int distance): node1(node1), node2(node2), distance(distance)
{
}

bool Edge::Connects(Node* node1, Node* node2)
{
	return ((node1 == this->node1 && node2 == this->node2) || (node1 == this->node2 && node2 == this->node1));
}