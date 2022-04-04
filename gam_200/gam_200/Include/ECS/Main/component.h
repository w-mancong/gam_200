#ifndef	COMPONENT_H
#define COMPONENT_H

#include <bitset>
#include "Utility/Type.h"
namespace ManCong
{
	namespace ECS
	{
		using ComponentType					= u8;	// A simple type alias
		ComponentType const MAX_COMPONENTS	= 32;	// Used to define the size of array later on
		using Signature						= std::bitset<MAX_COMPONENTS>; // Another type alias
	}
}

#endif