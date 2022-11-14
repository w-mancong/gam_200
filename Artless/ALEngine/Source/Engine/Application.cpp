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
		bool focus;
	}

	class Application
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);
	};

	void Application::Init(void)
	{
		// Init Logger
		ALEngine::Exceptions::Logger::Init();

		OpenGLWindow::InitGLFWWindow();
		focus = glfwGetWindowAttrib(OpenGLWindow::Window(), GLFW_VISIBLE);
		ECS::InitSystem();
		AudioManagerInit();
		ScriptEngine::Init();

		// Initialize Time (Framerate Controller)
		Time::Init();

		// Init ImGui
#ifdef EDITOR
		ALEditor::Instance()->SetImGuiEnabled(true);
		ALEditor::Instance()->SetDockingEnabled(true);
#endif

		Engine::AssetManager::Instance()->Init();

		appStatus = 1;
		RunFileWatcherThread();

		//Scene::LoadScene("Assets\\test.scene");
		//StartGameplaySystem();
	}

	void Application::Update(void)
	{
		// Accumulator for fixed delta time
		f32 accumulator{ 0.f };

		// should do the game loop here
		while (!glfwWindowShouldClose(OpenGLWindow::Window()) && appStatus)
		{
			// Get Current Time
			Time::ClockTimeNow();
			if (!focus)
			{
				glfwPollEvents();
				continue;
			}

			appStatus = !Input::KeyTriggered(KeyCode::Escape);

#ifdef EDITOR
			{
				PROFILER_TIMER("Editor UI Update")
				// Editor Command Manager Update
				Commands::EditorCommandManager::Update();
				// Begin new ImGui frame
				ALEditor::Instance()->Begin();
			}
#endif
			
			{
				PROFILER_TIMER("Normal Update")
				// Normal Update
				Engine::Update();
			}

#ifdef EDITOR
			if (ALEditor::Instance()->GetGameActive())
			{
#endif
				PROFILER_TIMER("Fixed Update")
				// Physics
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
				}
#ifdef EDITOR
			}
#endif

			{
				PROFILER_TIMER("Render Update")

				// Render
				Render();

				std::ostringstream oss;
				oss << OpenGLWindow::title << " | FPS: " << Time::m_FPS;
				glfwSetWindowTitle(OpenGLWindow::Window(), oss.str().c_str());
			}

			{
				PROFILER_TIMER("FPS Wait")

				// Wait for next frame
				Time::WaitUntil();
			}
			
			// Marks the end of a frame loop, for tracy profiler
			FrameMark
		}
	}

	void Application::Exit(void)
	{
		ExitGameplaySystem();
#ifdef EDITOR
		ALEditor::Instance()->Exit();		// Exit ImGui
#endif
		AssetManager::Instance()->Exit();	// Clean up all Assets
		AudioManagerExit();
		ScriptEngine::Shutdown();
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
		ZoneScopedN("Normal Update")
		Input::Update();
		AssetManager::Instance()->Update();
		AudioManagerUpdate();
#ifdef EDITOR
		if (!ALEditor::Instance()->GetGameActive())
			return;
#endif
		//UpdateCharacterControllerSystem();
		//UpdateEventTriggerSystem();
		//UpdateGameplaySystem();
	}

	void Engine::FixedUpdate(void)
	{
		//UpdateRigidbodySystem();
		//UpdateColliderSystem();
		//UpdatePostRigidbodySystem();
		
		//UpdateEventCollisionTriggerSystem();

		//DebugDrawRigidbody();
		//DebugDrawCollider();
		//DrawGameplaySystem();
	}

	int GetAppStatus(void)
	{
		return appStatus;
	}

	void SetAppStatus(int _appStatus)
	{
		appStatus = _appStatus;
	}

	void SetWindowFocus(bool _focus)
	{
		focus = _focus;
	}
}