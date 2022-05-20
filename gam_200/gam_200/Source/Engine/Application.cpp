#include "pch.h"

namespace ManCong
{
	namespace Engine
	{
		using namespace Math; using namespace Graphics; using namespace ECS;
		class Application
		{
		public:
			void Init(void);
			void Update(void);
			void Exit(void);
		};

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();
			Transform transform{ Vector2(200.0f, 0.0f), Vector2(10.5f, 10.5f), 0.0f };

			//Entity entity = CreateSprite(transform);
			//auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(entity);
			//sprite.color.g = 0.0f; sprite.color.b = 0.0f; sprite.color.a = 0.35f;
			//sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Line;

			transform.position = Vector2(0.0f, 443.0f);
			transform.rotation = 30.0f;
			//transform.position = Vector2(600.0f, 0.0f);
			transform.scale = Vector2(200.0f, 50.0f);
			Entity entity2 = CreateSprite(transform, Shape::Circle, RenderLayer::Background);
			auto& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(entity2);
			sprite2.mode = RenderMode::Line;
		}

		void Application::Update(void)
		{
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()) && !Input::Input::KeyTriggered(KeyCode::Escape))
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