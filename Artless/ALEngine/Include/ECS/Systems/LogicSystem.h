/*!
file:	LogicSystem.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the function declarations for LogicSystem

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef LOGIC_SYSTEM_H
#define	LOGIC_SYSTEM_H

namespace ALEngine::ECS
{
	class Coordinator;

	/*!*********************************************************************************
		\brief Register AnimatorSystem
	***********************************************************************************/
	void RegisterLogicSystem(void);

	/*!*********************************************************************************
		\brief If the component is inherited from UniBehaviour, the component will be 
		added into the vector inside of LogicComponent
	***********************************************************************************/
	template <typename T>
	void AddLogicComponent(Entity en)
	{
		if constexpr (std::is_polymorphic<T>::value)
		{
			// This line checks if component is inherited from UniBehaviour
			T component;
			if (!dynamic_cast<Component::UniBehaviour*>( &component ))
				return;

			if (!Coordinator::Instance()->HasComponent<Component::LogicComponent>(en))
				Coordinator::Instance()->AddComponent(en, Component::LogicComponent());

			Component::LogicComponent& lc = Coordinator::Instance()->GetComponent<Component::LogicComponent>(en);
			lc.logics.emplace_back(std::make_shared<T>());
		}
	}

	/*!*********************************************************************************
		\brief Run all the UniBehaviour's Load function inside LogicComponent
	***********************************************************************************/
	void Load(void);

	/*!*********************************************************************************
		\brief Run all the UniBehaviour's Init function inside LogicComponent
	***********************************************************************************/
	void Init(void);

	/*!*********************************************************************************
		\brief Run all the UniBehaviour's Update function inside LogicComponent
	***********************************************************************************/
	void Update(void);

	/*!*********************************************************************************
		\brief Run all the UniBehaviour's LateUpdate function inside LogicComponent
	***********************************************************************************/
	void LateUpdate(void);

	/*!*********************************************************************************
		\brief Run all the UniBehaviour's Free function inside LogicComponent
	***********************************************************************************/
	void Free(void);

	/*!*********************************************************************************
		\brief Run all the UniBehaviour's Unload function inside LogicComponent
	***********************************************************************************/
	void Unload(void);
}

#endif