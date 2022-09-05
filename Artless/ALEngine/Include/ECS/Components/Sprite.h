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
#include "ECS/Components/RenderData.h"
#include "ECS/Components/Color.h"
namespace ALEngine
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
				\brief
					Sprite component will be used to render entities in the game world
			***********************************************************************************/
			struct Sprite
			{
				u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
				u32 drawCount{ 0 };
				u32 texture{ 0 };
				u32 primitive{ GL_TRIANGLES };
				Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
				RenderLayer layer{ RenderLayer::Background };
				RenderMode mode{ RenderMode::Fill };
			};
		}
	}
}

#endif