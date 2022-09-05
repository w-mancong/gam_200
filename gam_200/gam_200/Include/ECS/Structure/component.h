/*!
file:	component.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the declarations for a component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COMPONENT_H
#define COMPONENT_H

#include <bitset>
#include "Utility/Type.h"
namespace ALEngine
{
	namespace ECS
	{
		using ComponentType					= u8;	// A simple type alias
		ComponentType const MAX_COMPONENTS	= 32;	// Used to define the size of array later on
		using Signature						= std::bitset<MAX_COMPONENTS>; // Another type alias
	}
}

#endif