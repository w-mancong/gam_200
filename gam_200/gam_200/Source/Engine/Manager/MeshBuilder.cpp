#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
	// layout location inside vertex shader
	u32 constexpr POS{ 0 }, COLOR{ 1 }, TEX{ 2 };
}

namespace ManCong
{
	namespace Engine
	{
		MeshBuilder::MeshBuilder(void) : s1{ "Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag" }, s2{ "Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag" } {}

		Image MeshBuilder::MakeImage(std::string const& filePath)
		{
			Image img;
			f32 vertices[] = {
				// positions    // colors           // texture coords
				 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
				 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
				-0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
				-0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
			};
			u32 indices[] = {
				1, 0, 2,
				2, 0, 3
			};
			glGenVertexArrays(1, &img.vao);
			glGenBuffers(1, &img.vbo);
			glGenBuffers(1, &img.ebo);

			glBindVertexArray(img.vao);

			glBindBuffer(GL_ARRAY_BUFFER, img.vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, img.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// texture coord attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(2);

			// load and create a texture 
			// -------------------------
			glGenTextures(1, &img.texture[0]);
			glBindTexture(GL_TEXTURE_2D, img.texture[0]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);

			glGenTextures(1, &img.texture[1]);
			glBindTexture(GL_TEXTURE_2D, img.texture[1]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			data = stbi_load("Assets/Images/awesomeface.png", &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			stbi_image_free(data);

			img.indicesSize = ARRAY_SIZE(indices);
			img.shader = &s1;

			img.shader->use();
			img.shader->Set("texture2", 1);

			//glm::mat4 trans = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
			//glm::mat4 trans = glm::mat4(1.0f);
			//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//std::cout << trans[0].x << ' ' << trans[0].y << ' ' << trans[0].z << ' ' << trans[0].w << std::endl;
			//std::cout << trans[1].x << ' ' << trans[1].y << ' ' << trans[1].z << ' ' << trans[1].w << std::endl;
			//std::cout << trans[2].x << ' ' << trans[2].y << ' ' << trans[2].z << ' ' << trans[2].w << std::endl;
			//std::cout << trans[3].x << ' ' << trans[3].y << ' ' << trans[3].z << ' ' << trans[3].w << std::endl << std::endl;

			//using namespace Math;
			//Matrix4x4 transform = Matrix4x4::Scale(0.5f, 0.5f, 0.5f) * Matrix4x4::Rotation(90.0f, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4x4::Translate(-0.75f, 0.0f, 0.0f);
			//std::cout << transform << std::endl << std::endl;

			//std::cout << Matrix4x4::Translate(1.0f, 1.0f, 0.0f) << std::endl << std::endl;
			//std::cout << rot << std::endl << std::endl;
			//std::cout << scale << std::endl << std::endl;
			//img.shader->Set("transform", transform);

			//u32 transformLoc = glGetUniformLocation(img.shader->id, "transform");
			//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			return img;
		}
	}
}