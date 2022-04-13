#ifndef	MESH_H
#define MESH_H

#include "Utility/Type.h"
namespace ManCong
{
	namespace Graphics
	{
		struct Mesh
		{
			u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
			u32 shaderProgram{ 0 };
			u32 indicesSize{ 0 };
		};
	}
}

#endif