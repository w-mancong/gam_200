#include "pch.h"

namespace
{
	void ProcessInput()
	{
		GLFWwindow* window = ManCong::Graphics::GraphicsWindow::GetWindow();
		if (Input::KeyTriggered(KeyCode::Escape))
			glfwSetWindowShouldClose(window, true);
	}
}

namespace ManCong
{
	namespace Engine
	{
		using namespace Graphics;
		void Application::Init(void)
		{
			GraphicsWindow::InitGLFWWindow();
		}

		void Application::Update(void)
		{
			// should do the game loop here
			while (!glfwWindowShouldClose(GraphicsWindow::GetWindow()))
			{
				// Input
				ProcessInput();

				// rendering commands here
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				// check and call events and swap the buffers
				glfwPollEvents();
				glfwSwapBuffers(GraphicsWindow::GetWindow());
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