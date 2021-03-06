/*!
file:	MeshBuilder.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the function declaration for MeshBuilder. 
		MeshBuilder is a singleton pattern class combined with the concept of a factory.
		It builds and returns sprites according to the type of Mesh that is tasked to create

		All content ? 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	MESH_BUILDER_H
#define MESH_BUILDER_H

namespace ManCong
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
			Sprite MakeSprite(std::string const& filePath);

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