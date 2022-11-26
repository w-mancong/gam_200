#include "pch.h"
#include <Engine/GSM/GameStateManager.h>

namespace ALEngine::Engine
{
	using namespace Math;
	using namespace Graphics;
	using namespace ECS;
#if EDITOR
	using namespace Editor;
#endif
	namespace
	{
		class Application
		{
		public:
			static void Init(void);
			static void Update(void);
			static void Exit(void);
		};

		std::atomic<int> appStatus;
		bool focus;
		bool editorFocus{ true };

		BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
		{
			switch (fdwCtrlType)
			{
			// When window console x button is pressed
			case CTRL_CLOSE_EVENT:
				Application::Exit();
				return TRUE;

			default:
				return FALSE;
			}
		}

#if EDITOR
		std::function<void(void)> UpdateLoop[2];
		u64 funcIndex{};

		void EditorUpdate(void)
		{
			if (!focus)
			{
				glfwPollEvents();
				return;
			}

			// Get Current Time
			Time::ClockTimeNow();

			// Editor Command Manager Update
			Commands::EditorCommandManager::Update();
			// Begin new ImGui frame
			ALEditor::Instance()->Begin();
			Input::GetMouseWorldPos();
			
			// Set the window focus
			//ImGuiFocusedFlags flag = ImGuiFocusedFlags_AnyWindow;
			//editorFocus = ImGui::IsWindowFocused(flag);

			Engine::Update();

			// Update Scene graph
			ECS::GetSceneGraph().Update();

			//RenderTransformBox();
			// Render
			Render();

			// Wait for next frame
			Time::WaitUntil();

			// Marks the end of a frame loop, for tracy profiler
			FrameMark;
		}
#endif
		void GameUpdate(void)
		{
			// Accumulator for fixed delta time
			f32 accumulator{ 0.f };

			if (GameStateManager::current != GameState::Restart)
			{				
				// Call function load
				LoadCppScripts();
			}
			else
			{
				GameStateManager::current = GameStateManager::previous;
				GameStateManager::next	  = GameStateManager::previous;
			}

			InitCppScripts();

			while (GameStateManager::current == GameStateManager::next)
			{
				if (!focus)
				{
					glfwPollEvents();
					continue;
				}

				// Get Current Time
				Time::ClockTimeNow();
#if EDITOR
				// Editor Command Manager Update
				Commands::EditorCommandManager::Update();
				// Begin new ImGui frame
				ALEditor::Instance()->Begin();

				if (ALEditor::Instance()->GetGameActive())
				{
#endif
					UpdateCppScripts();
					// Normal Update
					Engine::Update();
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

					// Update Scene graph
					ECS::GetSceneGraph().Update();
#if EDITOR
				}
#endif

				// Render
				Render();

				// Wait for next frame
				Time::WaitUntil();				

#if EDITOR
				// Marks the end of a frame m_Loop, for tracy profiler
				FrameMark;
#endif
			}

			// Free resources
			FreeCppScripts();
			// unload resource
			if (GameStateManager::next != GameState::Restart)
				UnloadCppScripts();
			
			GameStateManager::previous = GameStateManager::current;
			GameStateManager::current = GameStateManager::next;
		}
	}

	void Application::Init(void)
	{
		// Init Logger
		ALEngine::Exceptions::Logger::Init();

		OpenGLWindow::InitGLFWWindow();
		focus = glfwGetWindowAttrib(OpenGLWindow::Window(), GLFW_VISIBLE);
		ECS::InitSystem();
		AudioManagerInit();
		//ScriptEngine::Init();

		// Initialize Time (Framerate Controller)
		Time::Init();

		// Init ImGui
#if EDITOR
		ALEditor::Instance()->SetImGuiEnabled(true);
		ALEditor::Instance()->SetDockingEnabled(true);

		UpdateLoop[0] = EditorUpdate;
		UpdateLoop[1] = GameUpdate;
#endif

		Engine::AssetManager::Instance()->Init();
		GameStateManager::Init();
		RegisterCppScripts();

		appStatus = 1;
		RunFileWatcherThread();

#if !EDITOR
		OpenGLWindow::FullScreen(true);
		Scene::LoadScene("Assets\\test.scene");
		StartGameplaySystem();
		Console::StopConsole();
#endif

		//Scene::LoadScene("Assets\\test.scene");

		//Entity en = Coordinator::Instance()->GetEntityByTag("pause_menu");
		//EntityScript es;
		//es.AddInitFunction("PauseInit");
		//es.AddUpdateFunction("PauseUpdate");
		//es.AddFreeFunction("PauseReset");
		//Coordinator::Instance()->AddComponent(en, es);

		//EntityScript& es = Coordinator::Instance()->GetComponent<EntityScript>(en);
		//es.AddFreeFunction("SkillReset");

		//Scene::SaveScene("test");
	}

	void Application::Update(void)
	{
		// should do the game m_Loop here
		while (GameStateManager::current != GameState::Quit && appStatus)
		{
#if EDITOR
			UpdateLoop[funcIndex]();
#else
			GameUpdate();
#endif
		}
	}

	void Application::Exit(void)
	{
		ExitGameplaySystem();
#if EDITOR
		ALEditor::Instance()->Exit();		// Exit ImGui
#endif
		AssetManager::Instance()->Exit();	// Clean up all Assets
		AudioManagerExit();
		//ScriptEngine::Shutdown();
		glfwTerminate();					// clean/delete all GLFW resources
	}

	void Run(void)
	{		
#if !EDITOR
		Console::StopConsole();
#endif
		if (SetConsoleCtrlHandler(CtrlHandler, TRUE))
		{
			Application::Init();
			Application::Update();
			Application::Exit();
		}
	}

	void Engine::Update(void)
	{
#if EDITOR
		ZoneScopedN("Normal Delta Time Update");
#endif
		Input::Update();
		AssetManager::Instance()->Update();
		AudioManagerUpdate();
	}

	void Engine::FixedUpdate(void)
	{
#if EDITOR
		ZoneScopedN("Fixed Delta Time Update");
#endif
		UpdateGameplaySystem();

		UpdateRigidbodySystem();
		UpdateColliderSystem();
		UpdatePostRigidbodySystem();
		
		UpdateEventTriggerSystem();
		UpdateEventCollisionTriggerSystem();

		DebugDrawRigidbody();
		DebugDrawCollider();
		DrawGameplaySystem();
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

	void TerminateEngine(void)
	{
		SetAppStatus(0);
		GameStateManager::current = Engine::GameState::Quit;
	}

#if EDITOR
	void ToggleApplicationMode(void)
	{
		(++funcIndex) %= 2;
	}
#endif
}