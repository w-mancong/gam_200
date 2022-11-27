/*!
file:	PauseLogic.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause menu

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	PAUSE_LOGIC_H
#define PAUSE_LOGIC_H

#include <ECS/Structure/entity.h>

namespace ALEngine
{
	/*!*********************************************************************************
		\brief Interface for scripting to init the entities for pause menu
	***********************************************************************************/
	void PauseInit(ECS::Entity en);

	/*!*********************************************************************************
		\brief Interface for scripting to update the entities for pause menu
	***********************************************************************************/
	void PauseUpdate(ECS::Entity en);

	/*!*********************************************************************************
		\brief Interface for scripting to reset and values to it's default state for pause menu
	***********************************************************************************/
	void PauseReset(ECS::Entity en);
}

#endif