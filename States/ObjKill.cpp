#include "ObjKill.h"
#include "../Object.h"
#include "../MyGameWorld.h"
#include "../Algo/AStar.h"
#include "../InstanceObject.h"


ObjKill* ObjKill::Instance()
{
	static ObjKill temp;

	return &temp;
}

void ObjKill::Enter(Object *obj)
{
	//inform messenger that the object is dead
	MSGR->SendMsg(3.0, obj->GetID(), obj->GetID(), ObjGone, NULL);
	//clean up the astar for a new run
	astar->FinalPathClear();
	//create a new object
	OGEN->makeObject();
	//black out the object
	gMyGameWorld->SetSphereColor(obj->GetID() + 30, 0, 0, 0);
}

void ObjKill::Execute(Object *obj)
{

	wout << "Object Being Removed - ObjKill State" << endl;	
	
}

void ObjKill::Exit(Object *obj)
{
}

bool ObjKill::OnMsg(Object* obj, const Telegram &msg)
{
	if(msg.Message == ObjGone)
	{
		//remove from manager
		ObjMan->Del(obj);
		//make the kill count increment
		obj->~Object();
		return true;
	}
	else
	{
		return false;
	}
}