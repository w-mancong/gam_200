#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

namespace
{
	// layout location inside vertex shader
	u32 constexpr POS{ 0 }, COLOR{ 1 }, TEX{ 2 }, HANDLE{ 3 }, SPRITE_RESERVE_SIZE{ 100 };
	u32 instanceVBO{ 0 }, batchVBO{ 0 }, indirectBuffer{ 0 }, maxtrixBuffer{ 0 };
	u64 const NUM_VERTICES{ 4 },
		TOTAL_POS_BYTE{ sizeof(ALEngine::Math::vec3) * NUM_VERTICES * ALEngine::ECS::MAX_ENTITIES },
		TOTAL_COLOR_BYTE{ sizeof(ALEngine::Math::vec4) * NUM_VERTICES * ALEngine::ECS::MAX_ENTITIES },
		TOTAL_TEXCOORD_BYTE{ sizeof(ALEngine::Math::vec2) * NUM_VERTICES * ALEngine::ECS::MAX_ENTITIES },
		TOTAL_TEXTURE_HANDLE_BYTE{ sizeof(u64) * NUM_VERTICES * ALEngine::ECS::MAX_ENTITIES };

	struct Batch
	{
		u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
	};

	struct Vertex
	{
		f32 x{}, y{}, z{};
		f32 u{}, v{};
	};

	struct DrawElementsCommand
	{
		u32 vertexCount;
		u32 instanceCount;
		u32 firstIndex;
		u32 baseVertex;
		ALEngine::Math::vec4 color;
		u32 baseInstance;
		u64 texHandle;
	};

	Batch batch{}; DrawElementsCommand* commands{ nullptr };
	ALEngine::Math::mat4* models{ nullptr };
	void CreateIndirectBuffer(void)
	{
		glGenBuffers(1, &indirectBuffer);
	}

	void GenerateGeometry(void)
	{
		using namespace ALEngine;
		u32 vao{}, vbo{}, ebo{};
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		CreateIndirectBuffer();

		models = Memory::DynamicMemory::New<Math::mat4>(ECS::MAX_ENTITIES);
		commands = Memory::DynamicMemory::New<DrawElementsCommand>(ECS::MAX_ENTITIES);

		std::vector<Vertex> const aQuad =
		{
			// xyz			  | // uv
		  { -0.5f,  0.5f, 0.0f, 0.0f, 1.0f }, // top left
		  { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f }, // btm left
		  {  0.5f,  0.5f, 0.0f, 1.0f, 1.0f }, // top right
		  {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f }, // btm right
		};

		std::vector<u32> const indices =
		{
			0, 1, 2, 1, 3, 2
		};

		u64 const NUM_VERTICES{ aQuad.size() * ECS::MAX_ENTITIES };
		std::vector<Vertex> vertices(NUM_VERTICES);
		u64 vertexIndex{};
		for (u64 i{}; i < ECS::MAX_ENTITIES; ++i)
		{
			for (u64 j{}; j < aQuad.size(); ++j)
				vertices[vertexIndex++] = aQuad[j];
		}

		// create buffer for batch
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		//Specify vertex attributes for the shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, x)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, u)));

		//Create an element buffer and populate it
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indices.size(), indices.data(), GL_STATIC_DRAW);

		//Setup per instance matrices
		glGenBuffers(1, &maxtrixBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, maxtrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Math::mat4) * ECS::MAX_ENTITIES, nullptr, GL_DYNAMIC_DRAW);
		//A matrix is 4 vec4s
		glEnableVertexAttribArray(5 + 0);
		glEnableVertexAttribArray(5 + 1);
		glEnableVertexAttribArray(5 + 2);
		glEnableVertexAttribArray(5 + 3);

		glVertexAttribPointer(5 + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[0].x)));
		glVertexAttribPointer(5 + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[1].x)));
		glVertexAttribPointer(5 + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[2].x)));
		glVertexAttribPointer(5 + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[3].x)));
		//Only apply one per instance
		glVertexAttribDivisor(5 + 0, 1);
		glVertexAttribDivisor(5 + 1, 1);
		glVertexAttribDivisor(5 + 2, 1);
		glVertexAttribDivisor(5 + 3, 1);

		glBindVertexArray(0);

		batch.vao = vao;
		batch.vbo = vbo;
		batch.ebo = ebo;
	}

	//void CreateBatchVao()
	//{
	//	using namespace ALEngine;
	//	u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };

	//	u64 const TOTAL_BYTES = TOTAL_POS_BYTE + TOTAL_COLOR_BYTE + TOTAL_TEXCOORD_BYTE + TOTAL_TEXTURE_HANDLE_BYTE;

	//	glGenVertexArrays(1, &vao);
	//	glBindVertexArray(vao);

	//	glGenBuffers(1, &vbo);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	glBufferData(GL_ARRAY_BUFFER, TOTAL_BYTES, nullptr, GL_STATIC_DRAW);

	//	// position attribute
	//	glEnableVertexAttribArray(POS);
	//	glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, sizeof(Math::vec3), (void*)0);
	//	// color attribute
	//	glEnableVertexAttribArray(COLOR);
	//	glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Math::vec4), (void*)(TOTAL_POS_BYTE));
	//	// tex coord attribute
	//	glEnableVertexAttribArray(TEX);
	//	glVertexAttribPointer(TEX, 2, GL_FLOAT, GL_FALSE, sizeof(Math::vec2), (void*)(TOTAL_POS_BYTE + TOTAL_COLOR_BYTE));
	//	// tex_handle attribute
	//	glEnableVertexAttribArray(HANDLE);
	//	glVertexAttribLPointer(HANDLE, 1, GL_UNSIGNED_INT64_ARB, sizeof(u64), (void*)(TOTAL_POS_BYTE + TOTAL_COLOR_BYTE + TOTAL_TEXCOORD_BYTE));

	//	/*
	//		Buffering indices.
	//		Draw Primitive: GL_TRIANGLES
	//	*/
	//	glGenBuffers(1, &ebo);
	//	u32 const TOTAL_INDICES = 6 * ECS::MAX_ENTITIES;
	//	u32* indices = Memory::DynamicMemory::New<u32>(TOTAL_INDICES);
	//	for (u64 i = 0, j = 0; i < TOTAL_INDICES; i += 6, ++j)
	//	{
	//		u32 currIndex = static_cast<u32>(j) * 4;
	//		*(indices + i)	   = 0 + currIndex;
	//		*(indices + i + 1) = 1 + currIndex;
	//		*(indices + i + 2) = 2 + currIndex;
	//		*(indices + i + 3) = 1 + currIndex;
	//		*(indices + i + 4) = 3 + currIndex;
	//		*(indices + i + 5) = 2 + currIndex;
	//	}
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * TOTAL_INDICES, indices, GL_STATIC_DRAW);

	//	Memory::DynamicMemory::Delete(indices);
	//	batch.vao = vao;
	//	batch.vbo = vbo;
	//	batch.ebo = ebo;
	//}
}

namespace ALEngine::Engine
{
	using namespace Math; using namespace Memory; using namespace ECS;
	MeshBuilder::MeshBuilder(void)
	{
		//CreateBatchVao();
		//CreateIndirectBuffer();
		GenerateGeometry();
		m_Sprites.reserve(SPRITE_RESERVE_SIZE);
	}

	MeshBuilder::~MeshBuilder(void)
	{
		glDeleteVertexArrays(1, &batch.vao);
		glDeleteBuffers(1, &batch.vbo);
		glDeleteBuffers(1, &batch.ebo);
	}

	Sprite MeshBuilder::MakeSprite(std::string const& filePath)
	{
		Sprite sprite;
		for (auto it = m_Sprites.begin(); it != m_Sprites.end(); ++it)
		{
			if ((*it).first == filePath)
			{
				sprite = it->second;
				break;
			}
		}
		if (!sprite.texture)
			sprite = CreateSprite(filePath);
		return sprite;
	}

	void MeshBuilder::Reset(void)
	{
		for (auto it = m_Sprites.begin(); it != m_Sprites.end(); ++it)
		{
			// Release resources in the gpu
			glDeleteTextures(1, &(*it).second.texture);
			glMakeTextureHandleNonResidentARB(it->second.handle);
		}
		m_Sprites.clear();
	}

	Sprite MeshBuilder::CreateSprite(std::string const& filePath)
	{
		// load and create a texture 
		// -------------------------
		// load image, create texture and generate mipmaps
		s32 width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (!data)
		{
			std::cerr << "Failed to load texture" << std::endl;
			std::cerr << "File path: " << filePath << std::endl;
			return Sprite{};
		}
		u32 format{ 0 };
		switch (nrChannels)
		{
			case STBI_rgb:
			{
				format = GL_RGB;
				break;
			}
			case STBI_rgb_alpha:
			{
				format = GL_RGBA;
				break;
			}
			// I only want to accept files that have RGB/RGBA formats
			default:
			{
				std::cerr << "Wrong file format: Must contain RGB/RGBA channels" << std::endl;
				return Sprite{};
			}
		}

		u32 texture{ 0 };
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// buffer imagge data
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		u64 handle = glGetTextureHandleARB(texture);
		glMakeTextureHandleResidentARB(handle);

		stbi_image_free(data);

		m_Sprites.push_back(std::pair<std::string, Sprite>{ filePath, { texture, handle } });
		// Unbind vertex array and texture to prevent accidental modifications
		glBindTexture(GL_TEXTURE_2D, 0);
		return m_Sprites.back().second;
	}

	u32 GetBatchVao(void)
	{
		return batch.vao;
	}

	u64 GetVertexSize(void)
	{
		return NUM_VERTICES * MAX_ENTITIES;
	}

	void SubVertexData(BatchData const& bd)
	{
		//glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, TOTAL_POS_BYTE, bd.positions);	// positions
		//glBufferSubData(GL_ARRAY_BUFFER, TOTAL_POS_BYTE, TOTAL_COLOR_BYTE, bd.colors); // colors
		//glBufferSubData(GL_ARRAY_BUFFER, TOTAL_POS_BYTE + TOTAL_COLOR_BYTE, TOTAL_TEXCOORD_BYTE, bd.tex_coords); // tex coords
		//glBufferSubData(GL_ARRAY_BUFFER, TOTAL_POS_BYTE + TOTAL_COLOR_BYTE + TOTAL_TEXCOORD_BYTE, TOTAL_TEXTURE_HANDLE_BYTE, bd.tex_handles); // tex handles
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GenerateDrawCall(BatchData const& bd)
	{
		u32 baseVert{ 0 };
		for (u64 i{}; i < bd.count; ++i, baseVert += 4)
		{
			(*(commands + i)).vertexCount	= 6;						// 2 triangles = 6 vertices
			(*(commands + i)).instanceCount = 1;						// Draw 1 instance
			(*(commands + i)).firstIndex	= 0;						// Draw from index 0 for this instance
			(*(commands + i)).baseVertex	= baseVert;					// Starting from baseVert
			(*(commands + i)).baseInstance	= i;						// gl_InstanceID
			(*(commands + i)).color			= *(bd.colors + i);			// color value for this entity
			(*(commands + i)).texHandle		= *(bd.tex_handles + i);	// texture handle for this entity
		}

		//feed the draw command data to the gpu
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsCommand) * ECS::MAX_ENTITIES, commands, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, indirectBuffer);
		//feed color into the shader
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, color)));
		glVertexAttribDivisor(2, 1); //only once per instance

		//feed the instance id to the shader.
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
		glVertexAttribDivisor(3, 1); //only once per instance

		//feed the instance id to the shader.
		glEnableVertexAttribArray(4);
		glVertexAttribLPointer(4, 1, GL_UNSIGNED_INT64_ARB, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, texHandle)));
		glVertexAttribDivisor(4, 1); //only once per instance

		// sub model matrix in
		glBindBuffer(GL_ARRAY_BUFFER, maxtrixBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4) * ECS::MAX_ENTITIES, bd.models);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}