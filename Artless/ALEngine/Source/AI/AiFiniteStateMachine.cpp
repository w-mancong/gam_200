#include "pch.h"

namespace ALEngine::ECS
{
	
	AiFiniteStateMachine::AiFiniteStateMachine(u32 currentState, u32 nextState, u32 previousState)
	:m_CurrentStateId(currentState),
	m_NextStateId(nextState),
	m_PreviousStateId(previousState),
	m_CurrentStateNumCounter(0)
	{
	}

	AiFiniteStateMachine::~AiFiniteStateMachine()
	{
	}

	u32 AiFiniteStateMachine::GetStateId(std::string stateName)
	{
		u32 stateId{ 0 };

		// Traverse the guid map container
		for (auto& it : m_States)
		{
			// If mapped value is the guid that is needed
			if (it.second == stateName)
			{
				//assign to temp var
				stateId = it.first;
				break;
			}
		}
		return stateId;
	}

	u32 AiFiniteStateMachine::GetNextState()
	{
		return m_NextStateId;
	}

	u32 AiFiniteStateMachine::GetCurrentState()
	{
		return m_CurrentStateId;
	}

	u32 AiFiniteStateMachine::GetPreviousState()
	{
		return m_PreviousStateId;
	}

	void AiFiniteStateMachine::SetNextState(u32 stateId)
	{
		m_NextStateId = stateId;
	}

	void AiFiniteStateMachine::AddState(std::string stateName)
	{
		//get asset count for map kep
		u32 currentStatenum = GetCurrentStateNumCounter();
		m_States.insert({ currentStatenum, stateName });

		//update statenumcounter
		IncrementCurrentStateNumCounter();
	}

	void AiFiniteStateMachine::RemoveState(std::string stateName)
	{
		u32 stateToremoveid= GetStateId(stateName);
		m_States.erase(stateToremoveid);

		//update statenumcounter
		DecrementCurrentStateNumCounter();
	}

	bool AiFiniteStateMachine::CheckNextState()
	{
		return m_CurrentStateId != m_NextStateId;
	}

	u32 AiFiniteStateMachine::GetCurrentStateNumCounter()
	{
		//return assetloadedcounter value
		return m_CurrentStateNumCounter;
	}

	void AiFiniteStateMachine::IncrementCurrentStateNumCounter(void)
	{
		++m_CurrentStateNumCounter;
	}
	void AiFiniteStateMachine::DecrementCurrentStateNumCounter(void)
	{
		--m_CurrentStateNumCounter;
	}
}
