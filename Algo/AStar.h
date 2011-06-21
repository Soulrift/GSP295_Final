#ifndef ASTAR_H
#define ASTAR_H

#include "../MyGameWorld.h"
#include "Edge.h"

using namespace std;

#define astar AStar::Instance()

enum HTYPE
{
	MANHATTAN = 0,
	EUCLIDEAN,
	SQUARE
};

class AStar
{

private:
	vector<Node*> FinalPath;
	vector<Edge*> FinalEdges;

	vector<Node*> m_nodes;
	vector<Edge*> m_edges;

	int m_start;
	int m_finish;

	AStar(){};
	AStar(const AStar&);

	int Distance(vector<Edge*> edges, Node* node1, Node* node2);
	bool Contains(vector<Node*> &nodes, Node* n);
	vector<Node*>* AdjacentRemainingNodes(vector<Edge*> edges, vector<Node*> nodes, Node* node);
	Node* ExtractSmallest(vector<Node*> &nodes);
	void GetAStarPath(Node* destination);
	void ShowPath(vector<Edge*> edges, vector<Node*> path);
	double Heuristic(Node* node1, Node* node2, HTYPE htype);

public:

	static AStar* Instance()
	{
		static AStar instance;

		return &instance;
	}

	void Run(int start, int finish);

	// Run Function for Graph
	void Run(vector<Node*> nodes, vector<Edge*> edges, HTYPE htype);

	vector<Node*> GetFinalPath();

	void FinalPathClear();
};

#endif