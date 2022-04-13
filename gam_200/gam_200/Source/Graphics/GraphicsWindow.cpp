#include "pch.h"

namespace ManCong
{
	namespace Graphics
	{
		GLFWwindow* GraphicsWindow::window = nullptr;
		void GraphicsWindow::InitGLFWWindow(void)
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); used for initializing MacOS

			window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
			if (!window)
			{
				std::cerr << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				std::exit(EXIT_FAILURE);
				return;
			}
			glfwMakeContextCurrent(window);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cerr << "Failed to initialize GLAD" << std::endl;
				std::exit(EXIT_FAILURE);
				return;
			}
			// first two params specify location of the lower left corner of window
			glViewport(0, 0, 800, 600);
			// tell glfw to call this function whenever window resizes
			glfwSetFramebufferSizeCallback(window, ResizeWindow);
		}

		void GraphicsWindow::ResizeWindow(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		GLFWwindow* GraphicsWindow::GetWindow(void)
		{
			return window;
		}
	}
}