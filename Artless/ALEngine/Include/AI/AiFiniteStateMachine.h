#ifndef AI_FINITE_STATE_MACHINE_H
#define AI_FINITE_STATE_MACHINE_H

/*!
file: AiFiniteStateMachine.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for AiFiniteStateMachine
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

namespace ALEngine::ECS
{
	class AiFiniteStateMachine
	{
	public:
		//map container for storing states for AiFiniteStateMachine
		std::map<u32, std::string> m_States;

		AiFiniteStateMachine(u32 currentState, u32 nextState, u32 previousState);
		~AiFiniteStateMachine();

		/*!*********************************************************************************
	    \brief
	    get key for specified state name
		\return
		unsigned int value of the key for specified state name
	    ***********************************************************************************/
		u32 GetStateId(std::string stateName);

		/*!*********************************************************************************
	    \brief
	    get next state Id
		\return
		unsigned int value of the next state Id
	    ***********************************************************************************/
		u32 GetNextState();

		/*!*********************************************************************************
	    \brief
	    get current state Id
		\return
		unsigned int value of the current state Id
	    ***********************************************************************************/
		u32 GetCurrentState();

		/*!*********************************************************************************
	    \brief
	    get previous state Id
		\return
		unsigned int value of the previous state Id
	    ***********************************************************************************/
		u32 GetPreviousState();

		/*!*********************************************************************************
	    \brief
	    set next state Id
	    ***********************************************************************************/
		void SetNextState(u32 stateId);

		/*!*********************************************************************************
		\brief
		add new state to the map container of states
		***********************************************************************************/
		void AddState(std::string stateName);

		/*!*********************************************************************************
		\brief
		remove a state from the map container of states
		***********************************************************************************/
		void RemoveState(std::string stateName);

		/*!*********************************************************************************
		\brief
		check if current state not equal to next state
		***********************************************************************************/
		bool CheckNextState();

		/*!*********************************************************************************
	    \brief
		return current number of state added or existing
		\return
		unsigned int value of the current number of state  
	    ***********************************************************************************/
		u32 GetCurrentStateNumCounter();

		/*!*********************************************************************************
		\brief
		increment the counter for current number of state
		***********************************************************************************/
		void IncrementCurrentStateNumCounter(void);

		/*!*********************************************************************************
		\brief
	    decrement the counter for current number of state 
		***********************************************************************************/
		void DecrementCurrentStateNumCounter(void);
	private:
		//current state
		u32 m_CurrentStateId{ 0 };
		//next state
		u32 m_NextStateId{ 0 };
		//previous state
		u32 m_PreviousStateId{ 0 };

		//current number of state added or exisiting
		u32 m_CurrentStateNumCounter{ 0 };
	};
}
#endif // !AI_FINITE_STATE_MACHINE_H