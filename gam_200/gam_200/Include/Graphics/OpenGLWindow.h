#ifndef	GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

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

			static GLFWwindow* Window(void);

			static s32 *WindowWidth, *WindowHeight;

		private:
			static GLFWwindow* window; 
			static s32 width, height;
		};
	}
}

#endif