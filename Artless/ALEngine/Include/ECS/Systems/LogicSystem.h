/*!
file:	LogicSystem.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the function declarations for LogicSystem

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef LOGIC_SYSTEM_H
#define	LOGIC_SYSTEM_H

namespace
{
	/*!*****************************************************************************
		\brief To retrieve the logic component name based on the type T
	*******************************************************************************/
	template <typename T>
	std::string GetLogicComponentName(void)
	{
		std::string logicComponentName{ typeid(T).name() };
		// finding last of ':' because all our Scripts should be encased in a nested namespace ALEngine::Script
		return logicComponentName.substr(logicComponentName.find_last_of(':') + 1);
	}
}

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
#ifndef NDEBUG
			// This line checks if component is inherited from UniBehaviour
			T component;
			assert(dynamic_cast<Component::UniBehaviour*>(&component) && "Template T is not inherited from UniBehaviour");
#endif
			if (!Coordinator::Instance()->HasComponent<Component::LogicComponent>(en))
				Coordinator::Instance()->AddComponent(en, Component::LogicComponent());

			Component::LogicComponent& lc = Coordinator::Instance()->GetComponent<Component::LogicComponent>(en);
			lc.logics[GetLogicComponentName<T>()] = std::make_shared<T>();
		}
	}

	/*!*****************************************************************************
		\brief Get a specific logic component from the entity

		\param [in] en: Entity id to retrieve the logic component from
		\param [in] name: Class name of the inherited logic component you want to retrieve

		\return The shared pointer of the requested logic component
	*******************************************************************************/
	template <typename T>
	std::shared_ptr<T> GetLogicComponent(Entity en)
	{
		Component::LogicComponent& lc = Coordinator::Instance()->GetComponent<Component::LogicComponent>(en);
		std::string name = GetLogicComponentName<T>();
#ifndef NDEBUG
		assert(lc.logics.find(name) != lc.logics.end() && "Retrieving non-exsistent logic component!");
		// This line checks if component is inherited from UniBehaviour
		T component;
		assert(dynamic_cast<Component::UniBehaviour*>(&component) && "Template T is not inherited from UniBehaviour");
#endif
		return std::dynamic_pointer_cast<T>(lc.logics[name]);
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