#ifndef	SPRITE_H
#define SPRITE_H

#include "Utility/Type.h"
#include "ECS/Components/RenderData.h"
#include "ECS/Components/Color.h"
namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			struct Sprite
			{
				u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
				u32 indicesSize{ 0 };
				u32 texture{ 0 };
				Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
				RenderLayer layer{ RenderLayer::Background };
				RenderMode mode{ RenderMode::Fill };
			};
		}
	}
}

#endif