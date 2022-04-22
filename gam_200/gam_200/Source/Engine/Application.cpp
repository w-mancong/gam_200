#include "pch.h"
#include "Graphics/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"
#include "Engine/Camera.h"

using namespace ManCong::Math;
using namespace ManCong::Graphics;
using namespace ManCong::Engine;
namespace
{
	Vector3 pos{ 0.0f, 0.0f, 3.0f }, front{ 0.0f, 0.0f, -1.0f }, up{ 0.0f, 1.0f, 0.0f };
	f32 constexpr speed = 2.5f;
	Camera cam{ pos, up };
	f32 const HALF_WIDTH = static_cast<f32>(*OpenGLWindow::WindowWidth >> 1), HALF_HEIGHT = static_cast<f32>(*OpenGLWindow::WindowHeight >> 1);
	f32 prevX = HALF_WIDTH, prevY = HALF_HEIGHT;
	f32 constexpr sensitivity = 0.1f;

	void ProcessInput()
	{
		GLFWwindow* window = ManCong::Graphics::OpenGLWindow::GetWindow();
		if (Input::KeyTriggered(KeyCode::Escape))
			glfwSetWindowShouldClose(window, true);

		if (Input::KeyDown(KeyCode::W))
			pos += speed * cam.Front() * Time::dt;
		if (Input::KeyDown(KeyCode::S))
			pos -= speed * cam.Front() * Time::dt;
		if (Input::KeyDown(KeyCode::D))
			pos += Vector3::Cross(cam.Front(), cam.Up()).Normalize() * speed * Time::dt;
		if (Input::KeyDown(KeyCode::A))
			pos -= Vector3::Cross(cam.Front(), cam.Up()).Normalize() * speed * Time::dt;
		pos.y = 0.0f;
	}

	void mouse_callback(GLFWwindow* window, f64 x_posIn, f64 y_posIn)
	{
		static bool firstMouse = true;
		f32 x_pos = static_cast<f32>(x_posIn), y_pos = static_cast<f32>(y_posIn);
		if (firstMouse)
		{
			prevX = x_pos, prevY = y_pos;
			firstMouse = false;
		}
		f32 xOffset = (x_pos - prevX) * sensitivity, yOffset = (y_pos - prevY) * sensitivity;
		prevX = x_pos, prevY = y_pos;

		// processing mouse_input
		cam.Rotation(cam.Yaw() + xOffset, cam.Pitch() - yOffset);
	}
}

namespace ManCong
{
	namespace Engine
	{		
		Graphics::Mesh m1, m2;
		Graphics::Image img;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			glfwSetCursorPosCallback(OpenGLWindow::GetWindow(), mouse_callback);

		}

		void Application::Update(void)
		{
			s32 const mode[] = { GL_FILL, GL_LINE }; u64 index{ 0 };
			bool fullScreen = false;
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::GetWindow()))
			{
				// Input
				Time::Update();
				ProcessInput();

				if (Input::Input::KeyTriggered(KeyCode::Key_1))
					(++index) %= ARRAY_SIZE(mode);

				glPolygonMode(GL_FRONT_AND_BACK, mode[index]);

				// rendering commands here
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				for (u32 i = 0; i < 2; ++i)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, img.texture[i]);
				}

				img.shader->use();
				glBindVertexArray(img.vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, img.ebo);
				glDrawElements(GL_TRIANGLES, img.indicesSize, GL_UNSIGNED_INT, 0);

				//m2.shader->use();
				//glBindVertexArray(m2.vao);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m2.ebo);
				//glDrawElements(GL_TRIANGLES, m2.indicesSize, GL_UNSIGNED_INT, 0);

				// check and call events and swap the buffers
				glfwPollEvents();
				glfwSwapBuffers(OpenGLWindow::GetWindow());
			}
		}

		void Application::Exit(void)
		{
			glfwTerminate();	// clean/delete all GLFW resources
		}

		void Run(void)
		{
			Application app;
			app.Init();

			Shader ourShader{ "Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag" };
			glEnable(GL_DEPTH_TEST);
			f32 vertices[] = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};
			// world space positions of our cubes
			Vector3 cubePositions[] = {
				Vector3(0.0f,  0.0f,  0.0f),
				Vector3(2.0f,  5.0f, -15.0f),
				Vector3(-1.5f, -2.2f, -2.5f),
				Vector3(-3.8f, -2.0f, -12.3f),
				Vector3(2.4f, -0.4f, -3.5f),
				Vector3(-1.7f,  3.0f, -7.5f),
				Vector3(1.3f, -2.0f, -2.5f),
				Vector3(1.5f,  2.0f, -2.5f),
				Vector3(1.5f,  0.2f, -1.5f),
				Vector3(-1.3f,  1.0f, -1.5f)
			};
			u32 VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// texture coord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
			glEnableVertexAttribArray(1);


			// load and create a texture 
			// -------------------------
			u32 texture1, texture2;
			// texture 1
			// ---------
			glGenTextures(1, &texture1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load image, create texture and generate mipmaps
			s32 width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
			u8* data = stbi_load("Assets/Images/container.jpg", &width, &height, &nrChannels, 0);
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
			// texture 2
			// ---------
			glGenTextures(1, &texture2);
			glBindTexture(GL_TEXTURE_2D, texture2);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load image, create texture and generate mipmaps
			data = stbi_load("Assets/Images/awesomeface.png", &width, &height, &nrChannels, 0);
			if (data)
			{
				// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);

			// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
			// -------------------------------------------------------------------------------------------
			ourShader.use();
			ourShader.Set("texture1", 0);
			ourShader.Set("texture2", 1);

			// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
			// -----------------------------------------------------------------------------------------------------------
			ourShader.Set("proj", cam.ProjectionMatrix());

			Time::Init();
			// render loop
			// -----------
			while (!glfwWindowShouldClose(OpenGLWindow::GetWindow()))
			{
				Time::Update();
				// input
				// -----
				ProcessInput();
				cam.Position(pos);

				// render
				// ------
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// bind textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, texture2);

				// activate shader
				ourShader.use();

				// camera/view transformation
				ourShader.Set("view", cam.ViewMatrix());

				// render boxes
				glBindVertexArray(VAO);
				for (u32 i = 0; i < 10; i++)
				{
					// calculate the model matrix for each object and pass it to shader before drawing
					Matrix4x4 model;
					f32 angle = 20.0f * i;
					model = Matrix4x4::Rotation(angle, Vector3(1.0f, 0.3f, 0.5f)) * Matrix4x4::Translate(*(cubePositions + i));
					ourShader.Set("model", model);

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}

				// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
				// -------------------------------------------------------------------------------
				glfwSwapBuffers(OpenGLWindow::GetWindow());
				glfwPollEvents();
			}

			// optional: de-allocate all resources once they've outlived their purpose:
			// ------------------------------------------------------------------------
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);

			// glfw: terminate, clearing all previously allocated GLFW resources.
			// ------------------------------------------------------------------
			glfwTerminate();
		}
	}
}