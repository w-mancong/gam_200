/*!
file:	entity.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the declarations for a entity

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ENTITIY_H
#define ENTITIY_H

namespace ALEngine::ECS
{
	using Entity					= u32;		// A simple type alias
	Entity constexpr MAX_ENTITIES	= 5'000;	// Used to define the size of Entity array
}

#endif