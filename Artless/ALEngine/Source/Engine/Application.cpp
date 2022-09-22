#include "pch.h"
#include "Time.h"

#include <iostream>

namespace ALEngine::Engine
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

			if(!(i % 2))
				*(en + i) = CreateSprite(t, "Assets/Images/awesomeface.png");
			else
				*(en + i) = CreateSprite(t, "Assets/Images/container.jpg");
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(*(en + i));
			sprite.color = Color{ Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f) };
		}

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

			//std::cout << Time::m_FPS << std::endl;

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
	}
		
	void Engine::FixedUpdate(void)
	{
		//Raycast2DCollision({ -25, 25 }, { 25, 25 });
		UpdateRigidbodySystem();
		UpdateColliderSystem();
	}
}