﻿/*!
file:		OpenGLWindow.cpp
author:		Wong Man Cong
co-author:	Lucas Nguyen
email:		w.mancong@digipen.edu
			l.nguyen@digipen.edu
brief:		This file contains function to initialise the GLFW Window and provides
			interface to interact with it

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"

namespace ALEngine::Graphics
{
	namespace
	{
		void ResizeWindow(GLFWwindow* _window, int width, int height)
		{
			(void)_window;
			glViewport(0, 0, width, height);
		}

		void window_close_callback([[maybe_unused]] GLFWwindow* _window)
		{
			Engine::SetAppStatus(0);
		}

		void window_focus_callback([[maybe_unused]] GLFWwindow* window, int focused)
		{
#if EDITOR
			//focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
#endif
			Engine::SetWindowFocus(focused);
			Engine::ToggleMuteChannel(Engine::Channel::Master);
		}

		void scroll_callback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset)
		{
			Input::m_MouseWheelEvent = yoffset == -1 ? MouseWheelEvent::MouseWheelDown : MouseWheelEvent::MouseWheelUp;
		}

		u32 constexpr DEFAULT_WIDTH{ 1200 }, DEFAULT_HEIGHT{ 600 };
	}

	GLFWwindow* OpenGLWindow::window = nullptr;
	u32 OpenGLWindow::width{ DEFAULT_WIDTH }, OpenGLWindow::height{ DEFAULT_HEIGHT };
	std::string OpenGLWindow::title{ "ALEngine" };
	void OpenGLWindow::InitGLFWWindow(void)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//Console::StopConsole();

#ifdef _DEBUG
		// Enable OPENGL Debug Context if on debug mode
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		Serializer::Serializer config{ "Assets/Dev/Objects/Config.json" };
		title = config.GetString("window title", "ALEngine");
		Math::vec2 dimension = config.GetVec2("dimensions", { DEFAULT_WIDTH, DEFAULT_HEIGHT });
		width = static_cast<u32>(dimension.x);
		height = static_cast<u32>(dimension.y);

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
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

#ifdef _DEBUG
		int major{ 0 }, minor{ 0 };
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		std::cout << "Version: " << major << "." << minor << std::endl;

		char* vendor = (char*)glGetString(GL_VENDOR);
		char* device = (char*)glGetString(GL_RENDERER);
		char* version = (char*)glGetString(GL_VERSION);

		s32 n;
		glGetIntegerv(GL_NUM_EXTENSIONS, &n);
		std::vector<std::string> extensions(n);
		if (n > 0)
		{
			s32 i;
			for (i = 0; i < n; i++)
			{
				extensions[i] = (char*)glGetStringi(GL_EXTENSIONS, i);
			}
		}

		std::cout << "Vendor: " << vendor << std::endl;
		std::cout << "Device: " << device << std::endl;
		std::cout << "Version: " << version << std::endl;
		std::cout << "Supported extensions: " << std::endl;
		for (s32 i = 0; i < n; ++i)
		{
			std::cout << extensions[i] << std::endl;
		}
#endif // _DEBUG

		// first two params specify location of the lower left corner of window
		glViewport(0, 0, width, height);
		// tell glfw to call this function whenever window resizes
		glfwSetFramebufferSizeCallback(window, ResizeWindow);
		// tell glfw to call this function whenever window closes
		glfwSetWindowCloseCallback(window, window_close_callback);
		// tell glfw to call this function whenever window focus/not focusing
		glfwSetWindowFocusCallback(window, window_focus_callback);
		// tell glfw to call this function whenever mouse scroll happens
		glfwSetScrollCallback(window, scroll_callback);
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