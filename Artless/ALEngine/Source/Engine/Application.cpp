#include "pch.h"

namespace ALEngine::Engine
{
	using namespace Math;
	using namespace Graphics;
	using namespace ECS;
	using namespace Editor;
	namespace
	{
		std::atomic<int> appStatus;
	}

	class Application
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);
	};

	Entity player;
	Entity Target;

	void Application::Init(void)
	{
		OpenGLWindow::InitGLFWWindow();
		ECS::InitSystem();

		// Initialize Time (Framerate Controller)
		Time::Init();

		// Init Logger
		ALEngine::Exceptions::Logger::Init();

		//// Init ImGui
		ALEditor::Instance()->SetImGuiEnabled(true);
		ALEditor::Instance()->SetDockingEnabled(true);

		Engine::AssetManager::Instance()->Init();

		//AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
		//AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
		//AL_CORE_INFO("THIS IS A INFO MESSAGE");
		//AL_CORE_WARN("THIS IS A WARNING MESSAGE");
		//AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
		//AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");

		player = Coordinator::Instance()->CreateEntity();
		Target = Coordinator::Instance()->CreateEntity();

		Transform playerTransform;
		playerTransform.position = { 10, 300 };
		playerTransform.scale = { 100, 100 };
		Coordinator::Instance()->AddComponent(player, playerTransform);

		CreateCollider(player);
		CreateRigidbody(player);
		CreateCharacterController(player);
		//CreateEventTrigger(player);
		//Subscribe(player, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Poop);

		Transform targetTransform;
		targetTransform.position = { 0, -250 };
		targetTransform.scale = { 800,100 };
		Coordinator::Instance()->AddComponent(Target, targetTransform);

		CreateCollider(Target);

		appStatus = 1;
		RunFileWatcher();
	}

	void Application::Update(void)
	{
		// Accumulator for fixed delta time
		f32 accumulator{ 0.f };

		// should do the game loop here
		while (!glfwWindowShouldClose(OpenGLWindow::Window()) && appStatus)
		{
			Input::Update();
			AssetManager::Instance()->Update();
			// Get Current Time
			Time::ClockTimeNow();

			appStatus = !Input::KeyTriggered(KeyCode::Escape);

			//if (Input::KeyTriggered(KeyCode::Space)) {
			//	EventTrigger& new_event = Coordinator::Instance()->GetComponent<EventTrigger>(player);
			//	new_event.OnPointEnter.isTriggered = true;
			//}

			// ImGui Editor
			{
				PROFILER_TIMER("Editor");
				// Begin new ImGui frame
				ALEditor::Instance()->Begin();
			}

			// Normal Update
			{
				PROFILER_TIMER("Update");
				// Normal Update
				Engine::Update();
			}

			// Physics
			{
				PROFILER_TIMER("Physics");

				// Fixed Update (Physics)
				accumulator += Time::m_DeltaTime;

				// Steps to limit num times physics will run per frame
				int currNumSteps{ 0 };

				while (accumulator >= Time::m_FixedDeltaTime)
				{
					// Exit if physics happen more than limit
					if (currNumSteps++ >= Utility::MAX_STEP_FIXED_DT)
						break;

					Engine::FixedUpdate();
					accumulator -= Time::m_FixedDeltaTime;
					// AL_CORE_DEBUG(Time::m_FPS);
				}
			}

			// Render
			{
				PROFILER_TIMER("Render");
				Render();

				std::ostringstream oss;
				oss << OpenGLWindow::title << " | FPS: " << Time::m_FPS;
				glfwSetWindowTitle(OpenGLWindow::Window(), oss.str().c_str());
			}

			// Wait Time
			{
				PROFILER_TIMER("FPS Wait");
				// Wait for next frame
				Time::WaitUntil();
			}
		}
	}

	void Application::Exit(void)
	{
		ALEditor::Instance()->Exit();		// Exit ImGui
		AssetManager::Instance()->Exit();	// Clean up all Assets
		glfwTerminate();					// clean/delete all GLFW resources
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

	}

	void Engine::FixedUpdate(void)
	{
		UpdateCharacterControllerSystem();
		
		UpdateRigidbodySystem();
		UpdateColliderSystem();
		UpdatePostRigidbodySystem();

		UpdateEventCollisionTriggerSystem();
		UpdateEventTriggerSystem();

		DebugDrawCollider();
		DebugDrawRigidbody();
	}

	int GetAppStatus(void)
	{
		return appStatus;
	}
}