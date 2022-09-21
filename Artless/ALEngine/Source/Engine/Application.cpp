#include "pch.h"
#include "Time.h"

#include <iostream>

namespace ALEngine
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

		Entity en[2500];

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			for (u64 i = 0; i < 2500; ++i)
			{
				Transform t{ { Random::Range(-600.0f, 600.0f), Random::Range(-300.0f, 300.0f), 0.0f },
					{ Random::Range(20.0f, 75.0f), Random::Range(20.0f, 75.0f)},
					{ Random::Range(0.0f, 360.0f) } };

				*(en + i) = CreateSprite(t, "Assets/Images/awesomeface.png");
				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(*(en + i));
				sprite.color = Color{ Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f) };
			}

			//Transform t1{ { 0.0f, 150.0f, -200.0f }, { 50.0f, 50.0f }, 0.0f };
			//Transform t2{ { 0.0f, 0.0f, 0.0f }, { 50.0f, 50.0f }, 0.0f };
			//Transform t3{ { -150.0f, 0.0f, 0.0f }, { 50.0f, 50.0f }, 0.0f };

			//test = CreateSprite(t2, "Assets/Images/awesomeface.png");
			//CreateSprite(t1, "Assets/Images/awesomeface.png");
			//CreateSprite(t3, "Assets/Images/awesomeface.png");

			//Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(test);
			//sprite.color = Color{ 0.4f, 0.5f, 0.25f, 0.85f };
			//sprite.layer = RenderLayer::UI;

			// Initialize Time (Framerate Controller)
			Time::Init();

			//// Init ImGui
			//ALEditor::Instance()->Init();
		}

		void Application::Update(void)
		{
			// Accumulator for fixed delta time
			f32 accumulator{ 0.f };

			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()) && !Input::KeyTriggered(KeyCode::Escape))
			{				
				// Get Current Time
				Time::ClockTimeNow();

				//// Begin new ImGui frame
				//ALEditor::Instance()->Begin();

				// Normal Update
				Engine::Update();
				// Fixed Update (Physics)
				accumulator += Time::m_DeltaTime;
				while (accumulator >= Time::m_FixedDeltaTime)
				{
					Engine::FixedUpdate();
					accumulator -= Time::m_FixedDeltaTime;
				}

				// Render
				Render();

				std::cout << Time::m_FPS << std::endl;

				// Wait for next frame
				Time::WaitUntil();
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
		
		void Engine::Update(void)
		{
			f32 constexpr rot_spd{ 50.0f };
			for (u64 i = 0; i < ARRAY_SIZE(en); ++i)
			{
				Transform& trans = Coordinator::Instance()->GetComponent<Transform>(*(en + i));
				trans.rotation += rot_spd * Time::m_DeltaTime;
			}

			//Transform& trans = Coordinator::Instance()->GetComponent<Transform>(test);

			//f32 constexpr spd{ 150.0f };

			//if (Input::KeyDown(KeyCode::W))
			//{
			//	trans.position.y += spd * Time::m_DeltaTime;
			//}
			//if (Input::KeyDown(KeyCode::S))
			//{
			//	trans.position.y -= spd * Time::m_DeltaTime;
			//}
			//if (Input::KeyDown(KeyCode::D))
			//{
			//	trans.position.x += spd * Time::m_DeltaTime;
			//}
			//if (Input::KeyDown(KeyCode::A))
			//{
			//	trans.position.x -= spd * Time::m_DeltaTime;
			//}
		}
		
		void Engine::FixedUpdate(void)
		{
			//Raycast2DCollision({ -25, 25 }, { 25, 25 });
			UpdateRigidbodySystem();
			UpdateColliderSystem();
		}
	}
}