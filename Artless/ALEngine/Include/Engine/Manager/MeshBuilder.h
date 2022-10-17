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

namespace ALEngine::Engine
{
	class MeshBuilder : public Templates::Singleton<MeshBuilder>
	{
	public:
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

		void Init(void);
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
			Helper function to load and create an image. Images (with the same filePath) will
			not be loaded in more than once. The data that was loaded and created the image
			will be used to render all images
			\param [in] filePath:
			Path to the image to be loaded
			\return
			Pointer to the sprite that was created
		***********************************************************************************/
		Sprite CreateSprite(std::string const& filePath);

		friend class Templates::Singleton<MeshBuilder>;
		friend class Memory::StaticMemory;

		using mem = Memory::DynamicAllocator<std::pair<std::string, Sprite>>;
		std::vector<std::pair<std::string, Sprite>, mem> m_Sprites;
	};

	/*!*********************************************************************************
		\brief
		Used to group together the common data for batch rendering of entities on screen
	***********************************************************************************/
	struct BatchData
	{
		Math::vec4 const* vColor{ nullptr };
		Math::mat4 const* vMatrix{ nullptr };
		u64 const* texHandle{ nullptr };
	};

	void GenerateDrawCall(BatchData const& bd);
	u32 GetVao(void);
}

#endif