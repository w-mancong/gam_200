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
	/*!*********************************************************************************
		\brief Register AnimatorSystem
	***********************************************************************************/
	void RegisterLogicSystem(void);

	/*!*********************************************************************************
		\brief 
	***********************************************************************************/
	template <typename T>
	void AddLogicComponent(Entity en, T component);

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