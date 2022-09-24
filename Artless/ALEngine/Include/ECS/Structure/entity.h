#ifndef	ENTITIY_H
#define ENTITIY_H

#include "Utility/Type.h"
namespace ALEngine::ECS
{
	using Entity					= u32;		// A simple type alias
	Entity constexpr MAX_ENTITIES	= 5'000;	// Used to define the size of Entity array
}

#endif