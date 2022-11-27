/*!
file:	SkillHovering.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration that handles hovering over skill icons

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SKILL_HOVERING_H
#define SKILL_HOVERING_H

#include <ECS/Structure/entity.h>

namespace ALEngine
{
	/*!*********************************************************************************
		\brief Interface for scripting to init the entities for hovering over skill icons
	***********************************************************************************/
	void SkillInit(ECS::Entity en);

	/*!*********************************************************************************
		\brief Interface for scripting to reset the entities to it's default value
	***********************************************************************************/
	void SkillReset(ECS::Entity en);
}

#endif