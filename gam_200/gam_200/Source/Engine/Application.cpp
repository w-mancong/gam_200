#include "pch.h"

namespace ManCong
{
	namespace Engine
	{
		using namespace Math; using namespace Graphics; using namespace ECS;
		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			InitSystem();
			Transform transform{ Vector2(-1.0f, 0.0f), Vector2(2.5f, 2.5f), 0.0f };

			Entity entity = CreateSprite(transform);
			auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(entity);
			sprite.color.g = 0.0f; sprite.color.b = 0.0f; sprite.color.a = 0.35f;
			sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Lines;

			transform.scale = Vector2(2.5f, 2.5f);
			CreateSprite(Coordinator::Instance()->CreateEntity(), transform, Shape::Circle, RenderLayer::Background);
		}

		void Application::Update(void)
		{
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()))
			{
				Render();
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