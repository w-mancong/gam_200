#include "pch.h"
#include "Time.h"

#include <iostream>

namespace ALEngine::Engine
{
	using namespace Math;
	using namespace Graphics;
	using namespace ECS;
	using namespace Editor;
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

		// Initialize Time (Framerate Controller)
		Time::Init();

		// Init Logger
		ALEngine::Exceptions::Logger::Init();

		// Init ImGui
		ALEditor::Instance()->SetImGuiEnabled(true);
		ALEditor::Instance()->SetDockingEnabled(false);

		AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
		AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
		AL_CORE_INFO("THIS IS A INFO MESSAGE");
		AL_CORE_WARN("THIS IS A WARNING MESSAGE");
		AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
		AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");
	}

	void Application::Update(void)
	{
		// Accumulator for fixed delta time
		f32 accumulator{0.f};

		// should do the game loop here
		while (!glfwWindowShouldClose(OpenGLWindow::Window()) && !Input::Input::KeyTriggered(KeyCode::Escape))
		{
			// Get Current Time
			Time::ClockTimeNow();

			// Begin new ImGui frame
			ALEditor::Instance()->Begin();

			// Normal Update
			// Engine::Update();
			// Fixed Update (Physics)
			accumulator += Time::m_DeltaTime;
			while (accumulator >= Time::m_FixedDeltaTime)
			{
				Engine::FixedUpdate();
				accumulator -= Time::m_FixedDeltaTime;
				// AL_CORE_DEBUG(Time::m_FPS);
			}

			// Render
			Render();

			// Wait for next frame
			Time::WaitUntil();
		}
	}

	void Application::Exit(void)
	{
		ALEditor::Instance()->Exit(); // Exit ImGui
		glfwTerminate();			  // clean/delete all GLFW resources
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
		Transform &trans = Coordinator::Instance()->GetComponent<Transform>(Noah);
		Rigidbody2D &rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(Noah);
		f32 constexpr speed = 150.f;
		f32 constexpr rot = 1.0f;

		/*
		if (Input::Input::KeyTriggered(KeyCode::Space)) {
			rigid.velocity.y = 250;
		}*/

		// Use Velocity
		rigid.velocity.x = 0, rigid.velocity.y = 0;
		if (Input::Input::KeyDown(KeyCode::Down))
		{
			rigid.velocity.y = -speed;
		}
		if (Input::Input::KeyDown(KeyCode::Up))
		{
			rigid.velocity.y = speed;
		}
		if (Input::Input::KeyDown(KeyCode::Left))
		{
			rigid.velocity.x = -speed;
		}
		if (Input::Input::KeyDown(KeyCode::Right))
		{
			rigid.velocity.x = speed;
		}

		// Manual Position
		// if (Input::Input::KeyTriggered(KeyCode::W))
		//{
		//	trans.position.y += speed;
		// }
		// if (Input::Input::KeyTriggered(KeyCode::S))
		//{
		//	trans.position.y -= speed * Time::m_DeltaTime;
		// }
		// if (Input::Input::KeyTriggered(KeyCode::D))
		//{
		//	trans.position.x += speed;
		// }
		// if (Input::Input::KeyTriggered(KeyCode::A))
		//{
		//	trans.position.x -= speed;
		// }

		// if(Input::Input::KeyDown(KeyCode::W))
		//{
		//	trans.position.y += speed * Time::m_DeltaTime;
		// }
		// if (Input::Input::KeyDown(KeyCode::S))
		//{
		//	trans.position.y -= speed * Time::m_DeltaTime;
		// }
		// if (Input::Input::KeyDown(KeyCode::D))
		//{
		//	trans.position.x += speed * Time::m_DeltaTime;
		// }
		// if (Input::Input::KeyDown(KeyCode::A))
		//{
		//	trans.position.x -= speed * Time::m_DeltaTime;
		// }
		// if (Input::Input::KeyDown(KeyCode::Q))
		//{
		//	trans.rotation += rot;
		// }
		// if (Input::Input::KeyDown(KeyCode::E))
		//{
		//	trans.rotation -= rot;
		// }
	}

	void Engine::FixedUpdate(void)
	{
		// Raycast2DCollision({ -25, 25 }, { 25, 25 });
		UpdateRigidbodySystem();
		UpdateColliderSystem();
	}
}