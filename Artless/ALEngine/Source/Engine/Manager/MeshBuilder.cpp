#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

namespace
{
	// layout location inside vertex shader
	u32 constexpr POS{ 0 }, TEX{ 1 }, COLOR{ 2 }, HANDLE{ 3 }, DRAW_ID{ 4 }, INSTANCE_MATRIX{ 5 }, SPRITE_RESERVE_SIZE{ 100 };

	struct Vertex2D
	{
		float x, y, z;  // Position
		float u, v;     // Uv
	};

	struct DrawElementsCommand
	{
		GLuint vertexCount;
		GLuint instanceCount;
		GLuint firstIndex;
		GLuint baseVertex;
		GLuint baseInstance;
	};

	const std::vector<Vertex2D> gQuad = {
		//xy			            //uv
		{ -0.5f, -0.5f,   0.0f,	    0.0f, 0.0f },   // btm left
		{  0.5f, -0.5f,   0.0f,	    1.0f, 0.0f },   // btm right
		{ -0.5f,  0.5f,   0.0f,	    0.0f, 1.0f },   // top left
		{  0.5f,  0.5f,   0.0f,	    1.0f, 1.0f }    // top right
	};

	const std::vector<unsigned int> gQuadIndex = {
		3, 2, 0,
		0, 1, 3
	};

	struct Batch
	{
		u32 vao;	// vertex array
		u32 vbo;	// vertex buffer
		u32 ebo;	// element buffer
		u32 mbo;	// matrix buffer
		u32 cbo;	// color buffer
		u32 hbo;	// texture handle buffer
		u32 ibo;	// indirect buffer
	} batch;

	using namespace ALEngine;
	DrawElementsCommand* vDrawCommand{ nullptr };

	void GenerateGeometry(void)
	{
		u64 const num_vertices = gQuad.size() * ECS::MAX_ENTITIES;
		std::vector<Vertex2D> vVertex(num_vertices);
		u64 vertexIndex{};
		// populate geometry
		for (u64 i{}; i < ECS::MAX_ENTITIES; ++i)
		{
			for (u64 j{}; j != gQuad.size(); ++j)
				vVertex[vertexIndex++] = gQuad[j];
		}

		vDrawCommand = Memory::StaticMemory::New<DrawElementsCommand>(ECS::MAX_ENTITIES);

		u32 vao{}, vbo{}, ebo{}, mbo{}, cbo{}, hbo{}, ibo{};
		/***********************************************************
									VAO
		***********************************************************/
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		/***********************************************************
									VBO
		***********************************************************/
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vVertex.size(), vVertex.data(), GL_STATIC_DRAW);
		// Enabling the vertex attributes
		glEnableVertexAttribArray(POS);
		glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)(offsetof(Vertex2D, x)));
		glEnableVertexAttribArray(TEX);
		glVertexAttribPointer(TEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)(offsetof(Vertex2D, u)));

		/***********************************************************
									EBO
		***********************************************************/
		u64 quad_bytes = sizeof(u32) * gQuadIndex.size();
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad_bytes, nullptr, GL_STATIC_DRAW);

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, quad_bytes, gQuadIndex.data());

		/***********************************************************
									MBO
		***********************************************************/
		glGenBuffers(1, &mbo);
		glBindBuffer(GL_ARRAY_BUFFER, mbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Math::mat4) * ECS::MAX_ENTITIES, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(INSTANCE_MATRIX + 0);
		glEnableVertexAttribArray(INSTANCE_MATRIX + 1);
		glEnableVertexAttribArray(INSTANCE_MATRIX + 2);
		glEnableVertexAttribArray(INSTANCE_MATRIX + 3);

		glVertexAttribPointer(INSTANCE_MATRIX + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (void*)(offsetof(Math::mat4, mat[0])));
		glVertexAttribPointer(INSTANCE_MATRIX + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (void*)(offsetof(Math::mat4, mat[1])));
		glVertexAttribPointer(INSTANCE_MATRIX + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (void*)(offsetof(Math::mat4, mat[2])));
		glVertexAttribPointer(INSTANCE_MATRIX + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (void*)(offsetof(Math::mat4, mat[3])));
		//Only apply one per instance
		glVertexAttribDivisor(INSTANCE_MATRIX + 0, 1);
		glVertexAttribDivisor(INSTANCE_MATRIX + 1, 1);
		glVertexAttribDivisor(INSTANCE_MATRIX + 2, 1);
		glVertexAttribDivisor(INSTANCE_MATRIX + 3, 1);

		/***********************************************************
									CBO
		***********************************************************/
		glGenBuffers(1, &cbo);
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Math::vec4) * ECS::MAX_ENTITIES, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(COLOR);
		glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Math::vec4), (void*)(offsetof(Math::vec4, x)));
		glVertexAttribDivisor(COLOR, 1);

		/***********************************************************
									HBO
		***********************************************************/
		glGenBuffers(1, &hbo);
		glBindBuffer(GL_ARRAY_BUFFER, hbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(u64) * ECS::MAX_ENTITIES, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(HANDLE);
		glVertexAttribLPointer(HANDLE, 1, GL_UNSIGNED_INT64_ARB, sizeof(u64), (void*)0);
		glVertexAttribDivisor(HANDLE, 1);

		glGenBuffers(1, &ibo);

		/***********************************************************
								 Unbind VAO
		***********************************************************/
		glBindVertexArray(0);

		batch.vao = vao, batch.vbo = vbo, batch.ebo = ebo, batch.mbo = mbo, batch.cbo = cbo, batch.hbo = hbo, batch.ibo = ibo;
	}
}

namespace ALEngine::Engine
{
	using namespace Math; using namespace Memory; using namespace ECS;
	MeshBuilder::MeshBuilder(void)
	{

	}

	MeshBuilder::~MeshBuilder(void)
	{
		glDeleteVertexArrays(1, &batch.vao);
		glDeleteBuffers(1, &batch.vbo);
		glDeleteBuffers(1, &batch.ebo);
		glDeleteBuffers(1, &batch.mbo);
		glDeleteBuffers(1, &batch.cbo);
		glDeleteBuffers(1, &batch.hbo);
		glDeleteBuffers(1, &batch.ibo);
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

	void MeshBuilder::Init(void)
	{
		GenerateGeometry();
		m_Sprites.reserve(SPRITE_RESERVE_SIZE);
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

	void GenerateDrawCall(BatchData const& bd)
	{
		u64 baseVert = 0;
		for (u64 i{}; i < ECS::MAX_ENTITIES; ++i)
		{
			(vDrawCommand + i)->vertexCount = 6;		//4 triangles = 12 vertices
			(vDrawCommand + i)->instanceCount = 1;		//Draw 1 instance
			(vDrawCommand + i)->firstIndex = 0;			//Draw from index 0 for this instance
			(vDrawCommand + i)->baseVertex = baseVert;	//Starting from baseVert
			(vDrawCommand + i)->baseInstance = i;		//gl_InstanceID
			baseVert += gQuad.size();
		}

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, batch.ibo);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsCommand) * ECS::MAX_ENTITIES, vDrawCommand, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, batch.mbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Math::mat4) * ECS::MAX_ENTITIES, bd.vMatrix, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, batch.cbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Math::vec4) * ECS::MAX_ENTITIES, bd.vColor, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, batch.hbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(u64) * ECS::MAX_ENTITIES, bd.texHandle, GL_DYNAMIC_DRAW);

		//feed the instance id to the shader.
		glBindBuffer(GL_ARRAY_BUFFER, batch.ibo);
		glEnableVertexAttribArray(DRAW_ID);
		glVertexAttribIPointer(DRAW_ID, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
		glVertexAttribDivisor(DRAW_ID, 1);
	}

	u32 GetVao(void)
	{
		return batch.vao;
	}
}