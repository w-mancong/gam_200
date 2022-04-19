#ifndef	IMAGE_H
#define IMAGE_H

#include "Utility/Type.h"
namespace ManCong
{
	namespace Graphics
	{
		class Shader;
		struct Image
		{
			u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
			u32 indicesSize{ 0 };
			u32 texture[2]{ 0 };
			Shader* shader{ nullptr };
		};
	}
}

#endif