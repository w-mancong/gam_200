/*!
file:	MeshBuilder.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the function declaration for MeshBuilder. 
		MeshBuilder is a singleton pattern class combined with the concept of a factory.
		It builds and returns sprites according to the type of Mesh that is tasked to create

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	MESH_BUILDER_H
#define MESH_BUILDER_H

namespace ALEngine
{
	namespace Engine
	{
		class MeshBuilder : public Templates::Singleton<MeshBuilder>
		{
		public:
			/*!*********************************************************************************
				\brief
				Make a rectangle

				\return
				Sprite containing the relevant data to rendering a rectangle
			***********************************************************************************/
			Sprite MakeRectangle(void);

			/*!*********************************************************************************
				\brief
				Make a circle

				\return
				Sprite containing the relevant data to rendering a circle
			***********************************************************************************/
			Sprite MakeCircle(void);

			/*!*********************************************************************************
				\brief
				Make a triangle

				\return
				Sprite containing the relevant data to rendering a triangle
			***********************************************************************************/
			Sprite MakeTriangle(void);

			/*!*********************************************************************************
				\brief
				Load and make a sprite

				\param [in] filePath:
				Path to the image to be loaded

				\return
				Sprite containing the relevant data to rendering the image
			***********************************************************************************/
			u32 MakeSprite(std::string const& filePath);

			/*!*********************************************************************************
				\brief
				Reset MeshBuilder state. Unload and free memory loaded into the stream of all
				images
			***********************************************************************************/
			void Reset(void);

		private:
			/*!*********************************************************************************
				\brief
				Constructor
			***********************************************************************************/
			MeshBuilder(void);
			/*!*********************************************************************************
				\brief
				Destructor
			***********************************************************************************/
			virtual ~MeshBuilder(void);

			/*!*********************************************************************************
				\brief
				Helper function to create a single rectangle. The data will be used to render
				all rectangles
			***********************************************************************************/
			void CreateRectangle(void);

			/*!*********************************************************************************
				\brief
				Helper function to create a single circle. The data will be used to render 
				all circles
			***********************************************************************************/
			void CreateCircle(void);

			/*!*********************************************************************************
				\brief
				Helper function to create a single triangle. The data will be used to render
				all triangles
			***********************************************************************************/
			void CreateTriangle(void);

			void CreateSpriteInstanceBuffer(void);

			/*!*********************************************************************************
				\brief
				Helper function to load and create an image. Images (with the same filePath) will
				not be loaded in more than once. The data that was loaded and created the image
				will be used to render all images

				\param [in] filePath:
				Path to the image to be loaded

				\return
				Pointer to the sprite that was created
			***********************************************************************************/
			u32 CreateSprite(std::string const& filePath);

			enum class Shapes
			{
				Rectangle,
				Circle,
				Triangle,
				Total
			};

			friend class Templates::Singleton<MeshBuilder>;
			friend class Memory::StaticMemory;

			using pair_sprites = std::pair<std::string, u32>;
			std::vector<pair_sprites, Memory::DynamicAllocator<pair_sprites>> m_Sprites;
			Sprite* m_Shapes[static_cast<u64>(Shapes::Total)];

			// friend is not that good, if need friend means it's an engine design problem, we generally don't use it
			//friend class Templates::Singleton<MeshBuilder>; // -> upgrade this to instance MeshBuilder meshbuilder // .cpp -> meshbuilder->instance.Build();
			//friend class Memory::StaticMemory;

			//using pair_test = std::pair<std::string, Sprite*>;
			//std::vector<pair_test, Memory::DynamicAllocator<pair_test>> m_Sprites;
			//Sprite* m_Shapes[static_cast<u64>(Shapes::Total)]; // wrap your pointers to custom memory handler // why static_cast... so long

		};

		void SubInstanceBufferData(f32 texIndex[ECS::MAX_ENTITIES], Math::Matrix4x4 matrices[ECS::MAX_ENTITIES]);
	}
}

#endif