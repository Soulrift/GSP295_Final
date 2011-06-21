#include "ObjCreate.h"
#include "../Object.h"
#include "../MyGameWorld.h"
#include "../Algo/AStar.h"

ObjCreate* ObjCreate::Instance()
{
	static ObjCreate temp;

	return &temp;
}

void ObjCreate::Enter(Object *obj)
{
	int start;
	int finish;
	int temp;
	MSGR->SendMsg(2.0, obj->GetID(), obj->GetID(), Created, NULL);
	start = rand() % 18;
	finish = rand() % 18;
	//make sure i don't get start = end right off
	while(start == finish)
		finish = rand() % 18;

	if(start > finish)
	{
		temp = finish;
		finish = start;
		start = temp;
	}
	GLB->as_start = start;
	GLB->as_end = finish;
	astar->Run(start, finish);
	vector<Node*> path = astar->GetFinalPath();
	obj->SetPath(path);
	obj->SetIt((int)path.size() - 1);

	// Create player as a sphere
	gMyGameWorld->CreateSphere(obj->GetID() + 30);
	gMyGameWorld->SetSphereScale(obj->GetID() + 30, 0.50);
	gMyGameWorld->SetSphereColor(obj->GetID() + 30, 0, 255, 0);
	gMyGameWorld->SetSpherePosition(obj->GetID() + 30, obj->GetPath().at(obj->GetIt())->GetPosition().x, obj->GetPath().at(obj->GetIt())->GetPosition().y, 0.0);
}

void ObjCreate::Execute(Object *obj)
{
	wout << "Object Start - ObjCreate State" << endl;
}

void ObjCreate::Exit(Object *obj)
{
}

bool ObjCreate::OnMsg(Object *obj, const Telegram &msg)
{
	if(msg.Message == Created)
	{
		obj->GetFSM()->Change(ObjPath::Instance());
		return true;
	}
	else
		return false;
}