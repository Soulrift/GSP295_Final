#include "Graph.h"
#include "../MyGameWorld.h"
#include "../Global.h"
#include <fstream>

void Graph::Create()
{
	CreateNodes();
	CreateEdges();
}

vector<Node*> Graph::GetNodes()
{
	return nodes;
}

vector<Edge*> Graph::GetEdges()
{
	return edges;
}

void Graph::CreateNodes()
{
	//pull in text file to "myFile" and parse out nodes
	//also, put neighbors into the vector of ints inside
	//each node to create edges later
	vector<int> nodevalues;

	int temp;

	ifstream myFile("map2.txt");
	string line;
	int linenum = 0;
	Node* n;

	while (getline (myFile, line))
	{
		linenum++;
		istringstream linestream(line);
		string item;
		int itemnum = 0;
		while (getline (linestream, item, ','))
		{
			itemnum++;
			if (itemnum < 5)
			{
				temp = atoi(item.c_str());
				nodevalues.push_back(temp);
				if (itemnum == 4)
					n = new Node(nodevalues[0], Vector3(nodevalues[1], nodevalues[2], nodevalues[3]));
			}
			else if (itemnum > 4)
			{
				temp = atoi(item.c_str());
				n->neighbors.push_back(temp);
			}
		} // end while
		nodes.push_back(n);
		nodevalues.clear();
	}	

	//Drawing da nodes
	Vector3 tempPos;

	for(int i = 0; i < (int)nodes.size(); i++)
	{
		tempPos = nodes[i]->position;
		gMyGameWorld->CreateSphere(GLB->circID);
		gMyGameWorld->SetSpherePosition(GLB->circID, tempPos.x, tempPos.y, tempPos.z);
		gMyGameWorld->SetSphereColor(GLB->circID, 255, 255, 255);
		gMyGameWorld->SetSphereScale(GLB->circID, 0.4);
		if (i < 10)
			gMyGameWorld->AddSphereText(GLB->circID, "\n\n   ", i);
		else
			gMyGameWorld->AddSphereText(GLB->circID, "\n\n  ", i);
		nodes.at(i)->SphereID = GLB->circID;
		GLB->circID++;
	}
}

void Graph::CreateEdges()
{
	//pull neighbor data from nodes and construct edges
	Edge* e;
	double d;
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		for (int j = 0; j < (int)nodes.at(i)->neighbors.size(); j++)
		{
			d = getDist(nodes.at(i), nodes.at(nodes.at(i)->neighbors.at(j)));
			e = new Edge(nodes.at(i), nodes.at(nodes.at(i)->neighbors.at(j)), (int)d);
			edges.push_back(e);
		}
	}

	//draw the edges (lines)
	for (UINT i = 0; i < edges.size(); i++)
	{
		gMyGameWorld->CreateLine(GLB->linID);
		gMyGameWorld->SetLineStartPoint(GLB->linID, edges[i]->node1->GetPosition().x, edges.at(i)->node1->GetPosition().y, 0.0);
		gMyGameWorld->SetLineEndPoint(GLB->linID, edges.at(i)->node2->GetPosition().x, edges.at(i)->node2->GetPosition().y, 0.0);
		gMyGameWorld->SetLineColor(GLB->linID, 0, 0, 0);
		//gMyGameWorld->AddLineText(GLB->linID, "\n\n ", edges.at(i)->distance);
		edges.at(i)->LineID = GLB->linID;
		GLB->linID++;
	}
}

void Graph::Delete()
{
	nodes.clear();
	edges.clear();
}

double Graph::getDist(Node* n1, Node* n2)
{
	double temp;	
	temp = sqrt((pow((n1->GetPosition().x - n2->GetPosition().x), 2)) + (pow((n1->GetPosition().y - n2->GetPosition().y), 2)) + (pow((n1->GetPosition().z - n2->GetPosition().z), 2)));
	return temp;

}