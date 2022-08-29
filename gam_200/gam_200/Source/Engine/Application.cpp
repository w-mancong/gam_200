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

		Entity rect;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();
			Transform transform{ Vector2(200.0f, 0.0f), Vector2(10.5f, 10.5f), 0.0f };

			//Entity entity = CreateSprite(transform);
			//auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(entity);
			//sprite.color.g = 0.0f; sprite.color.b = 0.0f; sprite.color.a = 0.35f;
			//sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Line;

			transform.position = Vector2(0.0f, 0.0f);
			transform.rotation = 30.0f;
			//transform.position = Vector2(600.0f, 0.0f);
			transform.scale = Vector2(200.0f, 50.0f);
			rect = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);
			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(rect);
			sprite2.mode = RenderMode::Line;
			sprite2.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };

			// Initialize Time (Framerate Controller)
			Time::Init();
		}

		void Application::Update(void)
		{
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()) && !Input::Input::KeyTriggered(KeyCode::Escape))
			{
				Transform& trans = Coordinator::Instance()->GetComponent<Transform>(rect);
				f32 constexpr speed = 150.0f;
				f32 constexpr rot = 1.0f;
        
				if(Input::Input::KeyDown(KeyCode::W))
				{
					trans.position.y += speed * Time::m_DeltaTime;
				}
				if (Input::Input::KeyDown(KeyCode::S))
				{
					trans.position.y -= speed * Time::m_DeltaTime;
				}
				if (Input::Input::KeyDown(KeyCode::D))
				{
					trans.position.x += speed * Time::m_DeltaTime;
				}
				if (Input::Input::KeyDown(KeyCode::A))
				{
					trans.position.x -= speed * Time::m_DeltaTime;
				}
				if (Input::Input::KeyDown(KeyCode::Q))
				{
					trans.rotation += rot;
				}
				if (Input::Input::KeyDown(KeyCode::E))
				{
					trans.rotation -= rot;
				}
        
				Time::ClockTimeNow();
				Render();
				Time::WaitUntil();

				std::cout << Time::m_FPS << std::endl;
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