#include "pch.h"

namespace ManCong
{
	namespace Graphics
	{
		namespace
		{
			void ResizeWindow(GLFWwindow* _window, int width, int height)
			{
				glViewport(0, 0, width, height);
				ECS::ViewportResizeCameraUpdate();
			}
		}

		GLFWwindow* OpenGLWindow::window = nullptr;
		s32 OpenGLWindow::width = 1200, OpenGLWindow::height = 600;
		void OpenGLWindow::InitGLFWWindow(void)
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, 4);
			//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

			window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
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
			glViewport(0, 0, width, height);
			// tell glfw to call this function whenever window resizes
			glfwSetFramebufferSizeCallback(window, ResizeWindow);
			glEnable(GL_MULTISAMPLE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void OpenGLWindow::FullScreen(bool fullScreen)
		{
			s32 w = 0, h = 0;	// sorta buggy code: cannot see the window border
			if (fullScreen)
			{
				RECT desktop;
				HWND const hDesktop = GetDesktopWindow();
				GetWindowRect(hDesktop, &desktop);
				w = desktop.right; h = desktop.bottom;
			}
			else
				w = width, h = height;
			glfwSetWindowMonitor(window, fullScreen ? glfwGetPrimaryMonitor() : NULL, 0, 0, w, h, GLFW_DONT_CARE);
		}

		GLFWwindow* OpenGLWindow::Window(void)
		{
			return window;
		}
	}
}