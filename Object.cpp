#include "Object.h"
#include "States\StateMachine.h"
#include "States\ObjCreate.h"
#include "States\ObjPath.h"
#include "States\ObjKill.h"
#include "MyGameWorld.h"
#include "Global.h"

int Object::Created = 0;
int Object::Killed = 0;
int Object::counter = 0;

Object::Object():ObjectBase()
{
	Created++;
	GLB->SetCount(GLB->GetCount()+1);
	m_FSM = new StateMachine<Object>(this);
}
	
Object::~Object()
{
	Killed++;
	gMyGameWorld->RemoveSphere(GetID() + 30);
	delete m_FSM;
}

void Object::Start()
{
	m_FSM->setCurrent(ObjCreate::Instance());
}

bool Object::Handler(const Telegram &msg)
{
	return m_FSM->Handle(msg);
}

void Object::Update()
{
	m_FSM->Update();
}

void Object::Output(char *str)
{
	gMyGameWorld->ClearSphereText(GetID() + 30);
	gMyGameWorld->AddSphereText(GetID() + 30, str);
}

int Object::GetCreates()
{
	return Created;
}

int Object::GetKills()
{
	return Killed;
}

vector<Node*> Object::GetPath()
{
	return m_path;
}

void Object::SetPath(vector<Node*> path)
{
	m_path = path;
}

int Object::GetIt()
{
	return it;
}

void Object::SetIt(int i)
{
	it = i;
}

