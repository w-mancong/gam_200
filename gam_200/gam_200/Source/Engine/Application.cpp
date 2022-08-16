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

		Entity Noah, Hinata;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();
			//Obj 1
			Transform transform{ Vector2(200.0f, 0.0f), Vector2(50.5f, 50.5f), 0.0f };
			Noah = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(Noah);
			sprite2.mode = RenderMode::Line;
			sprite2.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };

			Transform& trans_noah = Coordinator::Instance()->GetComponent<Transform>(Noah);
			CreateCollider(Noah, trans_noah, ColliderType::Rectangle2D_OOBB);
			Collider2D& collider_Noah = Coordinator::Instance()->GetComponent<Collider2D>(Noah);
			collider_Noah.scale[0] = 50.0f, collider_Noah.scale[1] = 50.0f;
			trans_noah.rotation = 45;
			collider_Noah.rotation = 45;

			//Obj 2
			transform = { Vector2(150.0f, 0.0f), Vector2(50.0f, 50.0f), 0.0f };
			Hinata = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);

			Sprite& sprite3 = Coordinator::Instance()->GetComponent<Sprite>(Hinata);
			sprite3.mode = RenderMode::Line;
			sprite3.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
			Transform& trans_hinata = Coordinator::Instance()->GetComponent<Transform>(Hinata);
			CreateCollider(Hinata, trans_hinata, ColliderType::Rectangle2D_OOBB);
			Collider2D &collider_hinata = Coordinator::Instance()->GetComponent<Collider2D>(Hinata);
			collider_hinata.scale[0] = 50.0f, collider_hinata.scale[1] = 50.0f;
		}

		void Application::Update(void)
		{
			Time timer;
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()) && !Input::Input::KeyTriggered(KeyCode::Escape))
			{
				Transform& trans = Coordinator::Instance()->GetComponent<Transform>(Noah);
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
        
				UpdateCollider();
				Render();
				timer.ClockTimeNow();
				Render();
				timer.WaitUntil();
				//std::cout << timer.m_FPS << std::endl;
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