/*!
file:	OpenGLWindow.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain static class OpenGLWindow that controls the GLFW window

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

namespace ALEngine::Graphics
{
	class OpenGLWindow
	{
	public:
		/*!*********************************************************************************
			\brief
			Create and initializes GLFW Window
		***********************************************************************************/
		static void InitGLFWWindow(void);

		/*!*********************************************************************************
			\brief Toggle GLFW window to be between fullscreen/windowed mode 
		***********************************************************************************/
		static void FullScreen(bool fullScreen);

		///*!*********************************************************************************
		//	\brief Set GLFW window to be fullScreen in window mode
		//***********************************************************************************/
		//static void FullScreenWindowed(void);

		/*!*********************************************************************************
			\brief
			Return the instance of GLFWwindow that was created
		***********************************************************************************/
		static GLFWwindow* Window(void);

		static f32 ar;
		static u32 width, height;
		static std::string title;

	private:
		static GLFWwindow* window; 
	};
}

#endif