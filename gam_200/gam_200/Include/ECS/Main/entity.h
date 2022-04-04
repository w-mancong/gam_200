#ifndef	ENTITIY_H
#define ENTITIY_H

#include "Utility/Type.h"
namespace ManCong
{
	namespace ECS
	{
		using Entity				= u32;		// A simple type alias
		Entity const MAX_ENTITIES	= 5'000;	// Used to define the size of Entity array
	}
}

#endif