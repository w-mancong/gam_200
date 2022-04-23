#ifndef	SPRITE_H
#define SPRITE_H

#include "Utility/Type.h"
#include "Graphics/Shader.h"
namespace ManCong
{
	namespace ECS
	{
		namespace Components
		{
			struct Sprite
			{
				u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
				u32 indicesSize{ 0 };
				u32 texture{ 0 };
				Graphics::Shader* shader{ nullptr };
			};
		}
	}
}

#endif