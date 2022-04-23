#ifndef	MESH_H
#define MESH_H

#include "Utility/Type.h"
#include "Graphics/Shader.h"
namespace ManCong
{
	namespace ECS
	{
		namespace Components
		{
			struct Mesh
			{
				u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
				u32 indicesSize{ 0 };
				Graphics::Shader* shader{ nullptr };
			};
		}
	}
}

#endif