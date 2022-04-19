#ifndef	GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include "pch.h"
namespace ManCong
{
	namespace Graphics
	{
		class OpenGLWindow
		{
		public:
			static void InitGLFWWindow(void);
			static void FullScreen(bool fullScreen);
			static void ResizeWindow(GLFWwindow* _window, int width, int height);

			static GLFWwindow* GetWindow(void);

		private:
			static GLFWwindow* window; 
			static s32 width, height;
		};
	}
}

#endif