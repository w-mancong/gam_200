#include "pch.h"
#include "Engine/Manager/MeshBuilder.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

namespace
{
	// layout location inside vertex shader
	u32 constexpr POS{ 0 }, COLOR{ 1 }, TEX{ 2 }, SPRITE_RESERVE_SIZE{ 100 };
	u32 instanceVBO{ 0 }, batchVBO{ 0 };
	u64 const NUM_VERTICES{ 4 }, 
		TOTAL_POS_BYTE{ sizeof(ALEngine::Math::vec3) * NUM_VERTICES * ALEngine::ECS::MAX_ENTITIES },
		TOTAL_COLOR_BYTE{ sizeof(ALEngine::Math::vec4) * NUM_VERTICES * ALEngine::ECS::MAX_ENTITIES },
		TOTAL_TEXCOORD_BYTE{ sizeof(ALEngine::Math::vec2) * NUM_VERTICES * ALEngine::ECS::MAX_ENTITIES };

	struct Batch
	{
		u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };
	};

	Batch batch;
	void CreateBatchVao()
	{
		using namespace ALEngine;
		u32 vao{ 0 }, vbo{ 0 }, ebo{ 0 };

		u64 const TOTAL_BYTES = TOTAL_POS_BYTE + TOTAL_COLOR_BYTE + TOTAL_TEXCOORD_BYTE;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, TOTAL_BYTES, nullptr, GL_DYNAMIC_DRAW);

		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
		// color attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(TOTAL_POS_BYTE));
		// tex attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)(TOTAL_POS_BYTE + TOTAL_COLOR_BYTE));

		// top right, btm right, top left, btm left
		f32 texCoords[] =
		{
			0.0f, 1.0f,		// top right
			0.0f, 0.0f,		// btm right
			1.0f, 1.0f,		// top left
			1.0f, 0.0f,		// btm left
		};

		/*
			Buffering indices. Draw Primitive: GL_TRIANGLES
		*/
		glGenBuffers(1, &ebo);
		u64 const TOTAL_INDICES = 6 * ECS::MAX_ENTITIES;
		u32* indices = Memory::DynamicMemory::New<u32>(TOTAL_INDICES);
		for (u64 i = 0, j = 0; i < TOTAL_INDICES; i += 6, ++j)
		{
			u32 currIndex = static_cast<u32>(j) * 4;
			*(indices + i)	   = 0 + currIndex;
			*(indices + i + 1) = 1 + currIndex;
			*(indices + i + 2) = 2 + currIndex;
			*(indices + i + 3) = 1 + currIndex;
			*(indices + i + 4) = 3 + currIndex;
			*(indices + i + 5) = 2 + currIndex;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * TOTAL_INDICES, indices, GL_STATIC_DRAW);

		Memory::DynamicMemory::Delete(indices);
		batch.vao = vao;
		batch.vbo = vbo;
		batch.ebo = ebo;
	}
}

namespace ALEngine::Engine
{
	using namespace Math; using namespace Memory; using namespace ECS;
	MeshBuilder::MeshBuilder(void)
	{
		memset(m_Shapes, 0, sizeof(m_Shapes));
		CreateBatchVao();
		//CreateRectangle(); CreateCircle(); CreateTriangle();
		//CreateInstanceBuffer();
		m_Sprites.reserve(SPRITE_RESERVE_SIZE);
	}

	MeshBuilder::~MeshBuilder(void)
	{
		//for (u64 i = 0; i < static_cast<u64>(Shapes::Total); ++i)
		//{
		//	glDeleteVertexArrays(1, &(*(m_Shapes + i))->vao);
		//	glDeleteBuffers(1, &(*(m_Shapes + i))->vbo);
		//	glDeleteBuffers(1, &(*(m_Shapes + i))->ebo);
		//	StaticMemory::Delete(*(m_Shapes + i));
		//}
		glDeleteVertexArrays(1, &batch.vao);
		glDeleteBuffers(1, &batch.vbo);
		glDeleteBuffers(1, &batch.ebo);
	}

	Sprite MeshBuilder::MakeRectangle(void)
	{
		return m_Shapes[static_cast<u64>(Shapes::Rectangle)];
	}

	Sprite MeshBuilder::MakeCircle(void)
	{
		return m_Shapes[static_cast<u64>(Shapes::Circle)];
	}

	Sprite MeshBuilder::MakeTriangle(void)
	{
		return m_Shapes[static_cast<u64>(Shapes::Triangle)];
	}

	Sprite MeshBuilder::MakeSprite(std::string const& filePath)
	{
		Sprite sprite;
		for (auto it = m_Sprites.begin(); it != m_Sprites.end(); ++it)
		{
			if ((*it).first == filePath)
				sprite = it->second; break;
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
			//glDeleteVertexArrays(1, &(*it).second->vao);
			//glDeleteBuffers(1, &(*it).second->vbo);
			//glDeleteBuffers(1, &(*it).second->ebo);
			glDeleteTextures(1, &(*it).second.texture);
			// Delete ptr and free up dynamic memory
			/*DynamicMemory::Delete((*it).second);*/
		}
		m_Sprites.clear();
	}

	void MeshBuilder::CreateInstanceBuffer(void)
	{
		//glGenBuffers(1, &instanceVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

		//glBufferData(GL_ARRAY_BUFFER, sizeof(mat), mat, GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void MeshBuilder::CreateRectangle(void)
	{
		//Sprite* sprite = StaticMemory::New<Sprite>();
		//f32 position[] = {
		//	 0.5f,  0.5f, // top right
		//	 0.5f, -0.5f, // bottom right
		//	-0.5f, -0.5f, // bottom left
		//	-0.5f,  0.5f, // top left 
		//};
		//f32 texCoords[] = {
		//	1.0f, 1.0f, // top right
		//	1.0f, 0.0f, // bottom right
		//	0.0f, 0.0f, // bottom left
		//	0.0f, 1.0f  // top left 
		//};
		//u32 indices[] = {
		//	0, 2, 1,
		//	3, 2, 0
		//};

		//u64 const TOTAL_BYTES = sizeof(position) + sizeof(texCoords);
		//RECTANGLE_POSITION_SIZE_OFFSET = sizeof(position);

		//// -------------------------- Vertex Array Buffer -----------------------------------
		//glGenVertexArrays(1, &sprite->vao); glBindVertexArray(sprite->vao);
		//// -------------------------- Vertex Buffer Object ---------------------------------
		//glGenBuffers(1, &sprite->vbo); glGenBuffers(1, &sprite->ebo);
		//glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
		//glBufferData(GL_ARRAY_BUFFER, TOTAL_BYTES, nullptr, GL_STATIC_DRAW);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
		//glBufferSubData(GL_ARRAY_BUFFER, sizeof(position), sizeof(texCoords), texCoords);
		//// -------------------------- Element Buffer Object ---------------------------------
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//// position attribute
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
		//glEnableVertexAttribArray(0);

		//// Unbind vertex array to prevent accidental modifications
		//glBindVertexArray(0);

		//sprite->drawCount = ARRAY_SIZE(indices);
		//sprite->primitive = GL_TRIANGLES;
		//m_Shapes[static_cast<u64>(Shapes::Rectangle)] = sprite;
	}

	void MeshBuilder::CreateCircle(void)
	{
		//Sprite* sprite = Memory::StaticMemory::New<Sprite>();
		//u32 const VERTICES = 20;								// total number of vertices
		//f32 const ANGLE = 360.0f / static_cast<f32>(VERTICES);	// angle of circle / total number of vertices
		//u64 const TOTAL_POSITIONS = (VERTICES + 2) << 1;
		//f32* position = Memory::DynamicMemory::New<f32>(TOTAL_POSITIONS); // + 1 to include 0.0f, 0.0f, multiply 2 to cater for x and y axis

		//position[0] = 0.0f, position[1] = 0.0f, position[2] = 1.0f, position[3] = 0.0f;
		//Vector2 unit = Vector2{ 1.0f, 0.0f }; Matrix3x3 const rot = Matrix3x3::Rotation(ANGLE);
		//for (u64 i = 4; i < TOTAL_POSITIONS;)
		//{
		//	unit = rot * unit;
		//	*(position + i++) = unit.x; *(position + i++) = unit.y;
		//}

		//// -------------------------- Vertex Array Buffer -----------------------------------
		//glGenVertexArrays(1, &sprite->vao); glBindVertexArray(sprite->vao);
		//// -------------------------- Vertex Buffer Object ----------------------------------
		//glGenBuffers(1, &sprite->vbo); glGenBuffers(1, &sprite->ebo);
		//glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
		//glBufferData(GL_ARRAY_BUFFER, TOTAL_POSITIONS * sizeof(f32), nullptr, GL_STATIC_DRAW);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, TOTAL_POSITIONS * sizeof(f32), position);

		//// position attribute
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 2, (void*)0);
		//glEnableVertexAttribArray(0);
		//// Unbind vertex array to prevent accidental modifications
		//glBindVertexArray(0);

		//sprite->drawCount = VERTICES + 2;
		//sprite->primitive = GL_TRIANGLE_FAN;
		//// Release memory
		//DynamicMemory::Delete(position);
		//m_Shapes[static_cast<u64>(Shapes::Circle)] = sprite;
	}

	void MeshBuilder::CreateTriangle(void)
	{
		//Sprite* sprite = StaticMemory::New<Sprite>();
		//f32 position[] = {
		//	 0.0f,  0.5f,	// top
		//	-0.5f, -0.5f,	// left
		//	 0.5f, -0.5f,	// right
		//};
		//u32 indices[] = {
		//	0, 1, 2
		//};

		//// -------------------------- Vertex Array Buffer -----------------------------------
		//glGenVertexArrays(1, &sprite->vao); glBindVertexArray(sprite->vao);
		//glGenBuffers(1, &sprite->vbo); glGenBuffers(1, &sprite->ebo);
		//// -------------------------- Vertex Buffer Object ----------------------------------
		//glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(position), nullptr, GL_STATIC_DRAW);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
		//// -------------------------- Element Buffer Object ---------------------------------
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//// position attribute
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 2, (void*)0);
		//glEnableVertexAttribArray(0);
		//// Unbind vertex array to prevent accidental modifications
		//glBindVertexArray(0);

		//sprite->drawCount = ARRAY_SIZE(indices);
		//sprite->primitive = GL_TRIANGLES;
		//m_Shapes[static_cast<u64>(Shapes::Triangle)] = sprite;
	}

	Sprite MeshBuilder::CreateSprite(std::string const& filePath)
	{
		// load and create a texture 
		// -------------------------
		bool isJpeg = filePath[filePath.find_first_of('.') + 1] == 'j';
		u32 const FORMAT = isJpeg ? GL_RGB : GL_RGBA;

		u32 texture{ 0 };

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		if (isJpeg)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		s32 width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, FORMAT, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cerr << "Failed to load texture" << std::endl;
			std::cerr << "File path: " << filePath << std::endl;
		}
		stbi_image_free(data);

		m_Sprites.push_back(std::pair<std::string, Sprite>{ filePath, { texture } });
		// Unbind vertex array and texture to prevent accidental modifications
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return m_Sprites.back().second;
	}

	void SubMeshInstanceBuffer(Matrix4 const* mat)
	{
		//glBindVertexArray(MeshBuilder::Instance()->MakeRectangle().vao);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Matrix4) * MAX_ENTITIES, mat);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);
	}

	u32 GetBatchVao(void)
	{
		return batch.vao;
	}

	u64 GetVertexPositionSize(void)
	{
		return NUM_VERTICES * MAX_ENTITIES;
	}

	void SubVertexPosition(BatchData const& bd)
	{
		glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, TOTAL_POS_BYTE, bd.pos);
		glBufferSubData(GL_ARRAY_BUFFER, TOTAL_POS_BYTE, TOTAL_COLOR_BYTE, bd.col);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}