#include "pch.h"
#include "Graphics/Shader.h"

namespace
{
	f32 mixValue = 0.2f, flip = 1.0f;
	f32 constexpr speed = 0.5f;

	void ProcessInput()
	{
		GLFWwindow* window = ManCong::Graphics::OpenGLWindow::GetWindow();
		if (Input::KeyTriggered(KeyCode::Escape))
			glfwSetWindowShouldClose(window, true);

		if (Input::KeyDown(KeyCode::Up))
		{
			mixValue += speed * Time::dt; // change this value accordingly (might be too slow or too fast based on system hardware)
			if (mixValue >= 1.0f)
				mixValue = 1.0f;
		}
		if (Input::KeyDown(KeyCode::Down))
		{
			mixValue -= speed * Time::dt; // change this value accordingly (might be too slow or too fast based on system hardware)
			if (mixValue <= 0.0f)
				mixValue = 0.0f;
		}
		if (Input::KeyDown(KeyCode::Left))
		{
			flip = -1.0f;
		}
		if (Input::KeyDown(KeyCode::Right))
		{
			flip = 1.0f;
		}
	}
}

namespace ManCong
{
	namespace Engine
	{		
		Graphics::Mesh m1, m2;
		Graphics::Image img;

		u32 triIndex = 0;
		void ChangeTriangle()
		{
			if (Input::Input::KeyTriggered(KeyCode::Key_2))
			{
				(++triIndex) %= 2;
				switch (triIndex)
				{
					case 0:
					{
						f32 vert[] = {
							0.8f,  0.3f,  // top right
							0.8f, -0.3f,  // btm right
							0.3f,  0.3f,  // top left 
							0.3f, -0.3f,  // btm left
						};
						glBindBuffer(GL_ARRAY_BUFFER, m2.vbo);
						glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
						break;
					}
					case 1:
					{
						f32 vert[] = {
							 0.2f,  0.2f,  // top right
							 0.2f, -0.2f,  // btm right
							-0.2f,  0.2f,  // top left 
							-0.2f, -0.2f,  // btm left
						};
						glBindBuffer(GL_ARRAY_BUFFER, m2.vbo);
						glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
						break;
					}
				}
			}
		}

		using namespace Graphics;
		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			img = MeshBuilder::GetInstance()->MakeImage("Assets/Images/container.jpg");
			Time::Init();
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
				ChangeTriangle();

				if (Input::Input::KeyTriggered(KeyCode::Key_3))
				{
					fullScreen = !fullScreen;
					OpenGLWindow::FullScreen(fullScreen);
				}

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
				img.shader->Set("mixValue", mixValue);
				img.shader->Set("flip", flip);

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
			app.Update();
			app.Exit();
		}
	}
}