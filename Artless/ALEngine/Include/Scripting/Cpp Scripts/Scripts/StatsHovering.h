/*!
file:	StatsHovering.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration that handles hovering over the stats information

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	STATS_HOVERING_H
#define STATS_HOVERING_H

#include <ECS/Structure/entity.h>

namespace ALEngine
{
	/*!*********************************************************************************
		\brief Interface for scripting to init the entities for hovering over the stats bar
	***********************************************************************************/
	void StatsInit(ECS::Entity en);

	/*!*********************************************************************************
		\brief Interface for scripting to reset the entities back to it's default value
	***********************************************************************************/
	void StatsReset(ECS::Entity en);
}

#endif