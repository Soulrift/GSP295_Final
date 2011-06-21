#include "ObjPath.h"
#include "../Object.h"
#include "../MyGameWorld.h"
#include "../Algo/AStar.h"

ObjPath* ObjPath::Instance()
{
	static ObjPath temp;

	return &temp;
}

void ObjPath::Enter(Object *obj)
{
	MSGR->SendMsg(1.0, obj->GetID(), obj->GetID(), MoveObj, NULL);

	MSGR->SendMsg(2.0, obj->GetID(), obj->GetID(), Next, NULL);
	finished = false;
}

void ObjPath::Execute(Object *obj)
{
	if(finished)
		wout << "Found End - ObjPath State Exiting" << endl;
	else
		wout << "Object Pathing - ObjPath State" << endl;
}

void ObjPath::Exit(Object *obj)
{
}

bool ObjPath::OnMsg(Object *obj, const Telegram &msg)
{
	if(msg.Message == KillObj)
	{
		obj->GetFSM()->Change(ObjKill::Instance());
		return true;
	}
	else if(msg.Message == Next)
	{
		obj->SetIt(obj->GetIt() - 1);
		gMyGameWorld->SetSpherePosition(obj->GetID() + 30, obj->GetPath().at(obj->GetIt())->GetPosition().x, obj->GetPath().at(obj->GetIt())->GetPosition().y, 0.0);
		if(obj->GetIt() > 0)
			MSGR->SendMsg(1.0, obj->GetID(), obj->GetID(), Next, NULL);
		else
		{
			finished = true;
			MSGR->SendMsg(2.0, obj->GetID(), obj->GetID(), KillObj, NULL);
		}
		return true;
	}
	else
	{
		return false;
	}
}