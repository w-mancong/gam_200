/*!
file:	OpenGLWindow.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain static class OpenGLWindow that controls the GLFW window

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

namespace ALEngine
{
	namespace Graphics
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
				\brief
				Toggle GLFW window to be between fullscreen/windowed mode 
			***********************************************************************************/
			static void FullScreen(bool fullScreen);

			/*!*********************************************************************************
				\brief
				Return the instance of GLFWwindow that was created
			***********************************************************************************/
			static GLFWwindow* Window(void);

			static s32 width, height;

		private:
			static GLFWwindow* window; 
		};
	}
}

#endif