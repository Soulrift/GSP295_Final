#include "AStar.h"
#include "Graph.h"
#include "../Global.h"

void AStar::Run(int start, int finish)
{
	m_start = start;
	m_finish = finish;

	m_nodes = GRP->GetNodes();
	m_edges = GRP->GetEdges();

	for(UINT i = 0; i < m_nodes.size(); i++)
	{
		m_nodes.at(i)->distanceFromStart = INT_MAX;
		m_nodes.at(i)->previous = NULL;
	}
	//just gonna make it use euclidean at the moment
	Run(m_nodes, m_edges, EUCLIDEAN);

}

void AStar::Run(vector<Node*> nodes, vector<Edge*> edges, HTYPE htype)
{
	Vector3 tempPos;

	nodes.at(m_start)->distanceFromStart = 0; // set start position
	Node* end = nodes.at(m_finish);  // set end position

	while(nodes.size() > 0)
	{
		Node* smallest = ExtractSmallest(nodes);
		if(!smallest->NodeOn)
			continue;
		vector<Node*>* adjacentNodes = AdjacentRemainingNodes(edges, nodes, smallest);

		const int size = adjacentNodes->size();
		for(int i = 0; i < size; ++i)
		{
			Node* adjacent = adjacentNodes->at(i);
			double distance = Distance(edges, smallest, adjacent) + smallest->distanceFromStart + Heuristic(end, adjacent, htype);

			if(distance < adjacent->distanceFromStart)
			{
				adjacent->distanceFromStart = (int)distance;
				adjacent->previous = smallest;
			}
		}
		delete adjacentNodes;
	}

	GetAStarPath(end);

	ShowPath(edges, FinalPath);
	
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		nodes.at(i)->NodeOn = true;
	}
	nodes.clear();
	edges.clear();
}

double AStar::Heuristic(Node* node1, Node* node2, HTYPE htype)
{
	double h;
	switch(htype)
	{
	case SQUARE:
		h = pow(node1->GetPosition().x - node2->GetPosition().x, 2) + pow(node1->GetPosition().y - node2->GetPosition().y, 2);
		break;
	case EUCLIDEAN:
		double temp;
		temp = pow(node1->GetPosition().x - node2->GetPosition().x, 2) + pow(node1->GetPosition().y - node2->GetPosition().y, 2);
		h = sqrt(temp);
		break;
	case MANHATTAN:
		Vector3 tempvec = node1->GetPosition() - node2->GetPosition();
		h = abs(tempvec.x) + abs(tempvec.y);
		break;
	}
	return h;
}

Node* AStar::ExtractSmallest(std::vector<Node*> &nodes)
{
	int size = nodes.size();
	if(size == 0) return NULL;
	int smallestPosition = 0;
	Node* smallest = nodes.at(0);
	for(int i = 1;i < size; ++i)
	{
		Node* current = nodes.at(i);
		if(current->distanceFromStart < smallest->distanceFromStart)
		{
			smallest = current;
			smallestPosition = i;
		}
	}
	nodes.erase(nodes.begin() + smallestPosition);
	return smallest;
}

// returns distance between two connected nodes
int AStar::Distance(vector<Edge*> edges, Node* node1, Node* node2)
{
	const int size = edges.size();
	for(int i = 0; i < size; ++i)
	{
		Edge* edge = edges.at(i);
		if(edge->Connects(node1, node2))
		{
			return edge->distance;
		}
	}
	return -1;
}

// Does the 'nodes' vector contain 'node'
bool AStar::Contains(vector<Node*> &nodes, Node* n)
{
	const int size = nodes.size();
	for(int i = 0; i < size; ++i)
	{
		if(n == nodes.at(i))
		{
			return true;
		}
	}
	return false;
}

// Return all nodes adjacent to 'node' which are still in the 'nodes' collection.
vector<Node*>* AStar::AdjacentRemainingNodes(vector<Edge*> edges, vector<Node*> nodes, Node* node)
{
	vector<Node*>* adjacentNodes = new vector<Node*>();
	const int size = edges.size();
	for(int i = 0; i < size; ++i)
	{
		Edge* edge = edges.at(i);
		Node* adjacent = NULL;
		if(edge->node1 == node)
		{
			adjacent = edge->node2;
		}
		else if(edge->node2 == node)
		{
			adjacent = edge->node1;
		}
		if(adjacent && Contains(nodes, adjacent))
		{
			adjacentNodes->push_back(adjacent);
		}
	}
	return adjacentNodes;
}

void AStar::GetAStarPath(Node *destination)
{
	Node* previous = destination;
	while(previous)
	{
		FinalPath.push_back(previous);
		previous = previous->previous;
	}
}

void AStar::ShowPath(vector<Edge*> edges, vector<Node*> path)
{
	for (int i = path.size() - 1; i >= 0; i--)
	{
		gMyGameWorld->SetSphereColor(path.at(i)->SphereID, 235, 0, 0);
		GLB->as_final.push_back(path.at(i)->SphereID);

		for (int j = 0; j < (int)edges.size(); j++)
		{
			if ((i - 1) >= 0)
				if ((path.at(i)->id == edges.at(j)->node1->id) && (path.at(i - 1)->id == edges.at(j)->node2->id))
				{
					GLB->as_travelled += edges.at(j)->distance;
					gMyGameWorld->SetLineColor(edges.at(j)->LineID, 235, 0, 0);
					FinalEdges.push_back(edges.at(j));
				}
		}
	}
	gMyGameWorld->SetSphereColor(m_start, 0, 0, 235);
	gMyGameWorld->SetSphereColor(m_finish, 0, 0, 235);
}

void AStar::FinalPathClear()
{
	for(UINT i = 0; i < FinalEdges.size(); i++)
	{
		gMyGameWorld->SetLineColor(FinalEdges.at(i)->LineID, 0, 0, 0);
	}

	for(UINT i = 0; i < FinalPath.size(); i++)
	{
		gMyGameWorld->SetSphereColor(FinalPath.at(i)->SphereID, 255, 255, 255);
	}

	gMyGameWorld->SetSphereColor(m_start, 255, 255, 255);
	gMyGameWorld->SetSphereColor(m_finish, 255, 255, 255);

	m_edges.clear();
	m_nodes.clear();
	FinalPath.clear();
	FinalEdges.clear();
	
}

vector<Node*> AStar::GetFinalPath()
{
	return FinalPath;
}
