#include "Messager.h"
#include "../ObjectBase.h"
#include "../ObjectManager.h"
#include "../Global.h"

Messager* Messager::Instance()
{
	static Messager temp;
	return &temp;
}

void Messager::Send(ObjectBase *target, const Telegram &msg)
{
	if(!target->Handler(msg))
		return;
}

void Messager::SendMsg(double delay, int sendID, int tgtID, int msgID, void* other)
{
	//pull in the entities from the object manager
	ObjectBase* sender = ObjMan->GetObj(sendID);
	ObjectBase* target = ObjMan->GetObj(tgtID);

	//make sure a valid target is given
	if(target == NULL)
		return;

	// create a new telegram
	Telegram telegram(0, sendID, tgtID, msgID, other);


	//check if we're delaying the message, or sending immediately
	if(delay <= 0.0)
	{
		Send(target,telegram);
	}
	else
	{
		double Time = GLB->GetTime() + delay;
		telegram.DispatchTime = Time;
		m_Queue.insert(telegram);
	}
}

void Messager::DelayMsg()
{
	double Time = GLB->GetTime();
	//check to make sure queue isn't empty and we're in proper time range to dispatch
	while(!m_Queue.empty() && (m_Queue.begin()->DispatchTime < Time) &&
		(m_Queue.begin()->DispatchTime > 0))
	{
		const Telegram& telegram = *m_Queue.begin();
		ObjectBase* target = ObjMan->GetObj(telegram.Receiver);
		Send(target, telegram);
		m_Queue.erase(m_Queue.begin());
	}
}