/*!
file:		OpenGLWindow.cpp
author:		Wong Man Cong
co-author:	Lucas Nguyen
email:		w.mancong@digipen.edu
			l.nguyen@digipen.edu
brief:		This file contains a class "Time" that acts as the framerate controller.
			The delta time and FPS are calculated here.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"

namespace ALEngine
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
		s32 OpenGLWindow::width = 800, OpenGLWindow::height = 600;
		void OpenGLWindow::InitGLFWWindow(void)
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, 4);
			//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef _DEBUG
			// Enable OPENGL Debug Context if on debug mode
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); 
#endif

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

			// Check GL Context
			int flags{ 0 }; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{	// If it Debug Context is active				
				glEnable(GL_DEBUG_OUTPUT);	// Enable GL_DEBUG_OUTPUT
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);	// Enable Synchronous 
														// Will output error before problem function returns
				//glDebugMessageCallback(Exceptions::glDebugOutput, nullptr);
				//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}

			int major{ 0 }, minor{ 0 };
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			std::cout << "Version: " << major << "." << minor << std::endl;
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