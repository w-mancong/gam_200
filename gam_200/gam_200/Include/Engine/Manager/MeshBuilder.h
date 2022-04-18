#ifndef	MESH_BUILDER_H
#define MESH_BUILDER_H

#include "pch.h"
#include "Graphics/Shader.h"
namespace ManCong
{
	namespace Engine
	{
		using namespace Graphics;
		class MeshBuilder : private Templates::Singleton<MeshBuilder>
		{
		public:
			using Singleton<MeshBuilder>::GetInstance;

		private:
			MeshBuilder(void);
			~MeshBuilder(void) = default;

			friend class Templates::Singleton<MeshBuilder>;
			friend class Memory::InstanceMemory;

			Shader s1, s2;
		};
	}
}

#endif