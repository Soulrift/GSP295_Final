#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//State Machine - Holds information regarding current/prev/etc states
//Note - Template class - no .cpp

#include "State.h"
#include "../Messager/Telegram.h"

template <class state_type>

class StateMachine
{
private:
	//owner of the FSM
	state_type* m_Owner;
	//associated states:
	State<state_type>* m_CurState;
	State<state_type>* m_PrevState;
	State<state_type>* m_GlobState;

public:
	//ctor - force a state on build
	StateMachine(state_type* own):m_Owner(own), m_CurState(NULL), m_PrevState(NULL), m_GlobState(NULL){}

	virtual ~StateMachine(){}
	
	//Setters
	void setCurrent(State<state_type>* cur)
	{
		m_CurState = cur;
		m_CurState->Enter(m_Owner);
	}
	void setPrev(State<state_type>* prev)
	{
		m_PrevState = prev;
	}
	void setGlobal(State<state_type>* glob)
	{
		m_GlobState = glob;
	}
	void Change(State<state_type>* newState)
	{
		//changing the state to a new one
		m_PrevState = m_CurState;
		m_CurState->Exit(m_Owner);
		m_CurState = newState;
		m_CurState->Enter(m_Owner);
	}
	void Revert()
	{
		Change(m_PrevState);
	}
	//Getters
	State<state_type>* GetCurrent() const
	{
		return m_CurState;
	}
	State<state_type>* GetGlobal() const
	{
		return m_GlobState;
	}
	State<state_type>* GetPrevious() const
	{
		return m_PrevState;
	}
	
	void Update()const
	{
		
		if(m_GlobState) 
			m_GlobState->Execute(m_Owner);

		if(m_CurState)
			m_CurState->Execute(m_Owner);

	}

	//Checkers
	bool isState(const State<state_type>& check)const
	{
		if(typeid(*m_CurState) == typeid(check))
			return true;
		else
			return false;
	}

	//Message Handler
	bool Handle(const Telegram& msg)const
	{
		if(m_CurState && m_CurState->OnMsg(m_Owner, msg))
		{
			return true;
		}
		if(m_GlobState && m_GlobState->OnMsg(m_Owner, msg))
		{
			return false;
		}

	}

	

};

#endif