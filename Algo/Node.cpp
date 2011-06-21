#include "node.h"
#include "../Global.h"

Node::Node(int Id, Vector3 pos):id(Id), previous(NULL), distanceFromStart(INT_MAX)
{
	position = pos;
	NodeOn = true;
}

Vector3 Node::GetPosition()
{
	return position;
}

void Node::SetPosition(int x, int y, int z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Node::NodeOff()
{
	NodeOn = false;
}
