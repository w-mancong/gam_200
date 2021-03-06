#include "pch.h"
#include "Engine/Manager/MeshBuilder.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

namespace
{
	// layout location inside vertex shader
	u32 constexpr POS{ 0 }, COLOR{ 1 }, TEX{ 2 };
}

namespace ManCong
{
	namespace Engine
	{
		MeshBuilder::MeshBuilder(void)
		{
			memset(m_Shapes, 0, sizeof(m_Shapes));
			CreateRectangle(); CreateCircle(); CreateTriangle();
		}

		MeshBuilder::~MeshBuilder(void)
		{
			for (u64 i = 0; i < static_cast<u64>(Shapes::Total); ++i)
			{
				glDeleteVertexArrays(1, &(*(m_Shapes + i))->vao);
				glDeleteBuffers(1, &(*(m_Shapes + i))->vbo);
				glDeleteBuffers(1, &(*(m_Shapes + i))->ebo);
				Memory::InstanceMemory::Delete(*(m_Shapes + i));
			}
		}

		Sprite MeshBuilder::MakeRectangle(void)
		{
			return *m_Shapes[static_cast<u64>(Shapes::Rectangle)];
		}

		Sprite MeshBuilder::MakeCircle(void)
		{
			return *m_Shapes[static_cast<u64>(Shapes::Circle)];
		}

		Sprite MeshBuilder::MakeTriangle(void)
		{
			return *m_Shapes[static_cast<u64>(Shapes::Triangle)];
		}

		Sprite MeshBuilder::MakeSprite(std::string const& filePath)
		{
			Sprite* sprite{ nullptr };
			for (auto it = m_Sprites.begin(); it != m_Sprites.end(); ++it)
			{
				if ((*it).first == filePath)
					sprite = (*it).second; break;
			}
			if (!sprite)
				sprite = CreateSprite(filePath);
			return *sprite;
		}

		void MeshBuilder::Reset(void)
		{
			for (auto it = m_Sprites.begin(); it != m_Sprites.end(); ++it)
			{
				// Release resources in the gpu
				glDeleteVertexArrays(1, &(*it).second->vao);
				glDeleteBuffers(1, &(*it).second->vbo);
				glDeleteBuffers(1, &(*it).second->ebo);
				glDeleteTextures(1, &(*it).second->texture);
				// Delete ptr and free up dynamic memory
				Memory::DynamicMemory::Delete((*it).second);
			}
			m_Sprites.clear();
		}

		void MeshBuilder::CreateRectangle(void)
		{
			Sprite* sprite = Memory::InstanceMemory::New<Sprite>();
			f32 position[] = {
				 0.5f,  0.5f, // top right
				 0.5f, -0.5f, // bottom right
				-0.5f, -0.5f, // bottom left
				-0.5f,  0.5f, // top left 
			};
			u32 indices[] = {
				1, 0, 2,
				2, 0, 3
			};

			// -------------------------- Vertex Array Buffer -----------------------------------
			glGenVertexArrays(1, &sprite->vao); glBindVertexArray(sprite->vao);
			// -------------------------- Vertex Buffer Object ---------------------------------
			glGenBuffers(1, &sprite->vbo); glGenBuffers(1, &sprite->ebo);
			glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(position), nullptr, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
			// -------------------------- Element Buffer Object ---------------------------------
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// Unbind vertex array to prevent accidental modifications
			glBindVertexArray(0);

			sprite->indicesSize = ARRAY_SIZE(indices);
			m_Shapes[static_cast<u64>(Shapes::Rectangle)] = sprite;
		}

		void MeshBuilder::CreateCircle(void)
		{
			using namespace Math;
			Sprite* sprite = Memory::InstanceMemory::New<Sprite>();
			u32 const VERTICES = 20;								// total number of vertices
			f32 const ANGLE = 360.0f / static_cast<f32>(VERTICES);	// angle of circle / total number of vertices
			u64 const TOTAL_POSITIONS = (VERTICES + 1) << 1, TOTAL_INDICES = VERTICES * 3;
			f32* position = Memory::DynamicMemory::New<f32>(TOTAL_POSITIONS); // + 1 to include 0.0f, 0.0f, multiply 2 to cater for x and y axis
			u32* indices  = Memory::DynamicMemory::New<u32>(TOTAL_INDICES);

			position[0] = 0.0f, position[1] = 0.0f, position[2] = 1.0f, position[3] = 0.0f;
			Vector2 unit = Vector2{ 1.0f, 0.0f }; Matrix3x3 const rot = Matrix3x3::Rotation(ANGLE);
			for (u64 i = 4; i < TOTAL_POSITIONS;)
			{
				unit = rot * unit;
				*(position + i++) = unit.x; *(position + i++) = unit.y;
			}

			u32 curr_index = 1, next_index = 2;
			for (u64 i = 0; i < TOTAL_INDICES;)
			{
				*(indices + i++) = 0, *(indices + i++) = curr_index, * (indices + i++) = next_index;
				curr_index = next_index; ++next_index;
			}
			indices[TOTAL_INDICES - 1] = 1;

			// -------------------------- Vertex Array Buffer -----------------------------------
			glGenVertexArrays(1, &sprite->vao); glBindVertexArray(sprite->vao);
			// -------------------------- Vertex Buffer Object ----------------------------------
			glGenBuffers(1, &sprite->vbo); glGenBuffers(1, &sprite->ebo);
			glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
			glBufferData(GL_ARRAY_BUFFER, TOTAL_POSITIONS * sizeof(f32), nullptr, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, TOTAL_POSITIONS * sizeof(f32), position);
			// -------------------------- Element Buffer Object ---------------------------------
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, TOTAL_INDICES * sizeof(u32), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 2, (void*)0);
			glEnableVertexAttribArray(0);
			// Unbind vertex array to prevent accidental modifications
			glBindVertexArray(0);

			sprite->indicesSize = TOTAL_INDICES;
			// Release memory
			Memory::DynamicMemory::Delete(position), Memory::DynamicMemory::Delete(indices);
			m_Shapes[static_cast<u64>(Shapes::Circle)] = sprite;
		}

		void MeshBuilder::CreateTriangle(void)
		{
			Sprite* sprite = Memory::InstanceMemory::New<Sprite>();
			f32 position[] = {
				 0.0f,  0.5f,	// top
				-0.5f, -0.5f,	// left
				 0.5f, -0.5f,	// right
			};
			u32 indices[] = {
				0, 1, 2
			};

			// -------------------------- Vertex Array Buffer -----------------------------------
			glGenVertexArrays(1, &sprite->vao); glBindVertexArray(sprite->vao);
			glGenBuffers(1, &sprite->vbo); glGenBuffers(1, &sprite->ebo);
			// -------------------------- Vertex Buffer Object ----------------------------------
			glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(position), nullptr, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
			// -------------------------- Element Buffer Object ---------------------------------
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 2, (void*)0);
			glEnableVertexAttribArray(0);
			// Unbind vertex array to prevent accidental modifications
			glBindVertexArray(0);

			sprite->indicesSize = ARRAY_SIZE(indices);
			m_Shapes[static_cast<u64>(Shapes::Triangle)] = sprite;
		}

		Sprite* MeshBuilder::CreateSprite(std::string const& filePath)
		{
			Sprite* sprite = Memory::DynamicMemory::New<Sprite>();
			// -------------------------------------------------------------------------------
			//								Create an Image
			// -------------------------------------------------------------------------------
			f32 position[] = {
				 0.5f,  0.5f, // top right
				 0.5f, -0.5f, // bottom right
				-0.5f, -0.5f, // bottom left
				-0.5f,  0.5f, // top left 
			};
			f32 texCoords[] = {
				1.0f, 1.0f, // top right
				1.0f, 0.0f, // bottom right
				0.0f, 0.0f, // bottom left
				0.0f, 1.0f  // top left 
			};
			u32 indices[] = {
				1, 0, 2,
				2, 0, 3
			};

			u64 const TOTAL_BYTES = sizeof(position) + sizeof(texCoords);

			// Generate vertex array
			glGenVertexArrays(1, &sprite->vao); glBindVertexArray(sprite->vao);
			// Generate buffers
			glGenBuffers(1, &sprite->vbo); glGenBuffers(1, &sprite->ebo);
			glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
			glBufferData(GL_ARRAY_BUFFER, TOTAL_BYTES, nullptr, GL_STATIC_DRAW);
			// Filling buffer with data
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(position), sizeof(texCoords), texCoords);
			// Element buffer object
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// texture coord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)(sizeof(position)));
			glEnableVertexAttribArray(1);

			// load and create a texture 
			// -------------------------
			bool isJpeg = filePath[filePath.find_first_of('.') + 1] == 'j';
			u32 const FORMAT = isJpeg ? GL_RGB : GL_RGBA;

			glGenTextures(1, &sprite->texture);
			glBindTexture(GL_TEXTURE_2D, sprite->texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			// set the texture wrapping parameters
			if (isJpeg)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

			sprite->indicesSize = ARRAY_SIZE(indices);
			m_Sprites.push_back( std::pair<std::string, Sprite*>{ filePath, sprite } );
			// Unbind vertex array and texture to prevent accidental modifications
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			return m_Sprites.back().second;
		}
	}
}