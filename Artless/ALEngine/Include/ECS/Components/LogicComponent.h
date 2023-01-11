/*!
file:	LogicComponent.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the logic component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	LOGIC_COMPONENT_H
#define LOGIC_COMPONENT_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::ECS::Component
{
	struct LogicComponent
	{
		// This vector will contain all the UniBehaviour of the entity
		std::vector<UniBehaviour*> logics;
	};
}

#endif