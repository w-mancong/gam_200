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


	void AddLogicComponent(Entity en);
}

#endif