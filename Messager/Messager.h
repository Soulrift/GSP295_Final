#ifndef MESSAGER_H
#define MESSAGER_H

//Messager Class - Recieves/Holds/Sends messages for object
//stores everything in a priority queue (set)

#include "Telegram.h"
#include "../Global.h"
#include "../ObjectBase.h"
#include <set>

using namespace std;

//define list of message types
enum msg_type
{
	Created,
	MoveObj,
	KillObj,
	Next,
	ObjGone
};

#define MSGR Messager::Instance()

class Messager
{
private:
	//ctor
	Messager(){}
	//priority queue
	multiset<Telegram> m_Queue;
	//send messages
	void Send(ObjectBase* target, const Telegram& msg);
public:
	static Messager* Instance();
	//start sending everything
	void SendMsg(double delay, int sendID, int tgtID, int msgID, void* other);
	void DelayMsg();

};

#endif