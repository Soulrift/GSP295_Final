#ifndef OBJECT_H
#define OBJECT_H

#include "ObjectBase.h"
#include "States\StateMachine.h"
#include "States\ObjCreate.h"
#include "States\ObjPath.h"
#include "States\ObjKill.h"
#include "Graphics/GameWorld.h"

#include <string>
#include <iostream>
#include <cassert>

using namespace std;

template <class entity_type> class State;

class Object: public ObjectBase
{
private:
	//State Machine for Object
	StateMachine<Object>* m_FSM;

	//Static Flags for amount of death/creates
	static int Created;
	static int Killed;
	static int counter;

	//Info for graph traversal
	int m_CurNode;

	int it;

	vector<Node*> m_path;

public:
	//ctors... increment statics here
	Object();
	~Object();

	//wrapper for state machine update
	void Update();
	virtual bool Handler(const Telegram& msg);

	//setters
	void SetPath(vector<Node*> path);
	void SetIt(int i);

	//getters
	StateMachine<Object>* GetFSM()const{return m_FSM;}
	int GetIt();
	vector<Node*> GetPath();
	
	static int GetCreates();
	static int GetKills();

	//GFX Stuffs
	void Output(char* str);

	void Start();
	
};

#endif