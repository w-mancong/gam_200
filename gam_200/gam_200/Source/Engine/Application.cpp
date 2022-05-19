#include "pch.h"
#include "Time.h"

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

			Entity entity = CreateSprite(transform);
			auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(entity);
			sprite.color.g = 0.0f; sprite.color.b = 0.0f; sprite.color.a = 0.35f;
			sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Lines;

			transform.scale = Vector2(200.0f, 200.0f);
			CreateSprite(Coordinator::Instance()->CreateEntity(), transform, Shape::Circle, RenderLayer::Background);
		}

		void Application::Update(void)
		{
			Time timer;
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()))
			{
				timer.ClockTimeNow();
				Render();
				timer.WaitUntil();

				std::cout << timer.m_FPS << std::endl;
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