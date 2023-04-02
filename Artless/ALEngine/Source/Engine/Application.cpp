/*!
file:	Application.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition that starts the flow of the entire program

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"
#include <Engine/GSM/GameStateManager.h>
#include <GameplayCamera.h>
#include <GameplaySystem.h>
#include <GameplaySystem_Interface_Management_Enemy.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <ECS/Systems/LogicSystem.h>
#include <../Scripts/others/ScriptManager.h>

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
			// Get Current Time
			Time::ClockTimeNow();

			// Editor Command Manager Update
			Commands::EditorCommandManager::Update();
			// Begin new ImGui frame
			ALEditor::Instance()->Begin();

			if (!focus)
			{
				if (!ImGui::IsPopupOpen(0u, ImGuiPopupFlags_AnyPopupId) && (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows | ImGuiFocusedFlags_DockHierarchy)))
				{
					ALEditor::Instance()->End();
					glfwPollEvents();
					return;					
				}
			}

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
				ECS::Load();
				GameStateManager::next = GameStateManager::current = GameState::Running;
			}
			else
			{
				GameStateManager::current = GameStateManager::previous;
				GameStateManager::next	  = GameStateManager::previous;
			}

			ECS::Init();

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
					// Normal Update
					Engine::Update();
					ECS::Update();
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
						ECS::LateUpdate();
						accumulator -= Time::m_FixedDeltaTime;
					}

					ECS::UpdateUIpositions();
					// Update Scene graph
					ECS::GetSceneGraph().Update();

#if EDITOR
				}
#endif
				Input::Update();
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
			ECS::Free();
			// unload resource
			if (GameStateManager::next != GameState::Restart)
			{
				ECS::Unload();
				ClearPrefabCollection();
#if !EDITOR
				if (GameStateManager::next != GameState::Quit)
					AssetManager::Instance()->Reset();
#endif
			}

#if !EDITOR
			Coordinator::Instance()->DestroyEntities();
#endif

			Coordinator::Instance()->ResetSystem();
#if EDITOR
			if (ALEditor::Instance()->GetGameActive())
			{
#endif
				Scene::LoadScene();
#if EDITOR
			}
			else
			{
				Coordinator::Instance()->DestroyEntities();
				Engine::Scene::LoadState();
			}
#endif
			GameStateManager::previous = GameStateManager::current;
			GameStateManager::current  = GameStateManager::next;
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
#if EDITOR
		Script::InitScriptManager();
#endif

		Engine::AssetManager::Instance()->Init();
		Scene::InitSceneManager();
		GameStateManager::Init();

		// Initialize Time (Framerate Controller)
		Time::Init();
		//Scene::CutsceneManager::Instance()->Init();

		// Init ImGui
#if EDITOR
		ALEditor::Instance()->SetImGuiEnabled(true);
		ALEditor::Instance()->SetDockingEnabled(true);

		UpdateLoop[0] = EditorUpdate;
		UpdateLoop[1] = GameUpdate;
#endif

		appStatus = 1;
#if EDITOR
		RunFileWatcherThread();
#else
		//OpenGLWindow::FullScreen(true);
		Console::StopConsole();
#endif
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
			std::ostringstream oss{};
			oss << OpenGLWindow::title + " | FPS: " << Time::m_FPS;
			glfwSetWindowTitle(OpenGLWindow::Window(), oss.str().c_str());
		}
	}

	void Application::Exit(void)
	{
#if EDITOR
		ALEditor::Instance()->Exit();		// Exit ImGui
#endif
		AssetManager::Instance()->Exit();	// Clean up all Assets
		AudioManagerExit();
		glfwTerminate();					// clean/delete all GLFW resources
		Serializer::Serializer config{ "Assets\\Dev\\Objects\\Config.json" };
		config.SetInt("FullScreen", OpenGLWindow::fullScreen);
	}

	void Run(void)
	{		
#if !EDITOR
		//Console::StopConsole();
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
		AssetManager::Instance()->Update();
		Input::Update();
#endif
		AudioManagerUpdate(); 
		UpdateEventTriggerSystem();
	}

	void Engine::FixedUpdate(void)
	{
#if EDITOR
		ZoneScopedN("Fixed Delta Time Update");
#endif
		//UpdateGameplaySystem();

		UpdateButtonSystem();

		UpdateRigidbodySystem();
		UpdateColliderSystem();
		UpdatePostRigidbodySystem();
		
#if EDITOR
		if (!ALEditor::Instance()->GetGameActive())
			return;
#endif

		UpdateEventCollisionTriggerSystem();

		DebugDrawRigidbody();
		DebugDrawCollider();
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