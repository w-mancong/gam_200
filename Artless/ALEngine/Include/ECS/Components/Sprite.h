/*!
file:	Sprite.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the sprite component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SPRITE_H
#define SPRITE_H

#include "Utility/Type.h"
namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief
			Sprite component will be used to render entities in the game world
	***********************************************************************************/
	struct Sprite
	{
		u32 texture{ 0 };
		u64 handle{ 0 };
		Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
		RenderLayer layer{ RenderLayer::Background };
	};
}

#endif