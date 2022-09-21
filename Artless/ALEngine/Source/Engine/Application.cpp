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

		Entity test;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			Transform t1{ { 0.0f, 150.0f, 0.0f }, { 50.0f, 50.0f }, 0.0f };
			Transform t2{ { 0.0f, 0.0f, 0.0f }, { 50.0f, 50.0f }, 0.0f };

			CreateSprite(t1, "Assets/Images/awesomeface.png");
			test = CreateSprite(t2, "Assets/Images/awesomeface.png");

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
			Transform& trans = Coordinator::Instance()->GetComponent<Transform>(test);

			f32 constexpr spd{ 150.0f };

			if (Input::KeyDown(KeyCode::W))
			{
				trans.position.y += spd * Time::m_DeltaTime;
			}
			if (Input::KeyDown(KeyCode::S))
			{
				trans.position.y -= spd * Time::m_DeltaTime;
			}
			if (Input::KeyDown(KeyCode::D))
			{
				trans.position.x += spd * Time::m_DeltaTime;
			}
			if (Input::KeyDown(KeyCode::A))
			{
				trans.position.x -= spd * Time::m_DeltaTime;
			}
		}
		
		void Engine::FixedUpdate(void)
		{
			//Raycast2DCollision({ -25, 25 }, { 25, 25 });
			UpdateRigidbodySystem();
			UpdateColliderSystem();
		}
	}
}