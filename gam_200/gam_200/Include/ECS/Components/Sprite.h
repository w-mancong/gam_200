#ifndef	SPRITE_H
#define SPRITE_H

#include "Utility/Type.h"
#include "ECS/Components/Sub/RenderData.h"
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
				Sub::RenderLayer layer{ Sub::RenderLayer::Background };
				Sub::RenderMode mode{ Sub::RenderMode::Fill };
			};
		}
	}
}

#endif