#ifndef	MESH_BUILDER_H
#define MESH_BUILDER_H

#include "pch.h"
#include "Graphics/Shader.h"
namespace ManCong
{
	namespace Engine
	{
		class MeshBuilder : public Templates::Singleton<MeshBuilder>
		{
		public:
			Mesh MakeRectangle(void);
			Mesh MakeCircle(void);
			Mesh MakeTriangle(void);
			Sprite MakeSprite(std::string const& filePath);

			Graphics::Shader& Shade()
			{
				return s2;
			}

			void Reset(void);

		private:
			MeshBuilder(void);
			virtual ~MeshBuilder(void);

			void CreateRectangle(void);
			void CreateCircle(void);
			void CreateTriangle(void);
			Sprite* CreateSprite(std::string const& filePath);

			enum class Shapes
			{
				Rectangle,
				Circle,
				Triangle,
				Total
			};

			friend class Templates::Singleton<MeshBuilder>;
			friend class Memory::InstanceMemory;

			Graphics::Shader s1, s2;
			std::vector<std::pair<std::string, Sprite*>> m_Sprites;
			Mesh* m_Meshes[static_cast<u64>(Shapes::Total)];
		};
	}
}

#endif