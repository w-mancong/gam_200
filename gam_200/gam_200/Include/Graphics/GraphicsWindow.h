#ifndef	GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include "pch.h"
namespace ManCong
{
	namespace Graphics
	{
		class GraphicsWindow
		{
		public:
			static void InitGLFWWindow(void);
			static void ResizeWindow(GLFWwindow* window, int width, int height);

			static GLFWwindow* GetWindow(void);

		private:
			static GLFWwindow* window;
		};
	}
}

#endif