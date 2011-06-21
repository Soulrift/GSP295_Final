#ifndef NAVGRAPH_H
#define NAVGRAPH_H

#include "Edge.h"
#include "../Math/Vector3.h"
#include <vector>

using namespace std;

#define GRP Graph::Instance()

class Graph
{
protected:
	Graph(){};

	void CreateEdges();
	void CreateNodes();

public:
	static Graph* Instance()
	{
		static Graph instance;

		return &instance;
	}

	int Cid;
	int Lid;
	vector<Node*> nodes;
	vector<Edge*> edges;

	vector<Node*> GetNodes();
	vector<Edge*> GetEdges();

	void Create();
	void Delete();
	double getDist(Node* n1, Node* n2);
};

#endif