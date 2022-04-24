#ifndef	MESH_BUILDER_H
#define MESH_BUILDER_H

#include "pch.h"
namespace ManCong
{
	namespace Engine
	{
		class MeshBuilder : public Templates::Singleton<MeshBuilder>
		{
		public:
			Sprite MakeRectangle(void);
			Sprite MakeCircle(void);
			Sprite MakeTriangle(void);
			Sprite MakeSprite(std::string const& filePath);

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

			std::vector<std::pair<std::string, Sprite*>> m_Sprites;
			Sprite* m_Shapes[static_cast<u64>(Shapes::Total)];
		};
	}
}

#endif