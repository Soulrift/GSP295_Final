#ifndef NODE_H
#define NODE_H

#include <vector>
#include "../Math/Vector3.h"

using namespace std;

class Node
{
public:
	Node(int Id, Vector3 pos);

	int id;
	int distanceFromStart;
	Vector3 position;
	vector<int> neighbors;
	int SphereID;

	bool NodeOn;

	Vector3 GetPosition();
	void SetPosition(int x, int y, int z);
	void NodeOff();

	Node* previous;
};

#endif
