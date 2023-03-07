/*!
file:	ALEditor.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains the function definitions for the ALEditor class.
		The ALEditor class essentially manages the Dear ImGui functions, as well as the
		different editor panels generated with the help of Dear ImGui.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

#if EDITOR

#include "imgui.h"
#include "imgui_internal.h"
#include <Engine/GSM/GameStateManager.h>

#include <Windows.h>

#define TRACY_FILEPATH "\\Tracy\\Tracy.exe"

namespace ALEngine::Editor
{
	namespace 
	{
		const c8* EMPTY_SCENE_FP{ "Assets/Dev/SceneManager/empty.scene" };
	}
	ALEditor::ALEditor(void)
	{
		// Initialize the editor
		Init();
	}

	void ALEditor::Init(void)
	{
		// Check ImGui version
		IMGUI_CHECKVERSION();

		// Create ImGui context
		ImGui::CreateContext();

		// Set imgui style to dark
		ImGui::StyleColorsDark();

		// Set minimum window size
		ImGui::GetStyle().WindowMinSize = m_WinMinSize;

		// ImGui IO stuff
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport

		// If viewports are enabled, tweak WindowRounding/WindowBg so plaform windows can look like the normal ones
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.f;
			style.Colors[ImGuiCol_WindowBg].w = 1.f;
		}

		// Scale of ImGui stuff
		style.ScaleAllSizes(1.2f);

		// Font Loading for ImGui
		io.Fonts->AddFontFromFileTTF("Assets/fonts/Arial-italic.ttf", 20.f);

		// Init GLFW
		ImGui_ImplGlfw_InitForOpenGL(Graphics::OpenGLWindow::Window(), true);
		// Set GLSL version
		ImGui_ImplOpenGL3_Init("#version 450");

		// Load Editor Data
		LoadData();

		// Set docking enabled or disabled
		m_DockingEnabled = true;

		// Initialize style of the Editor
		InitializeStyle();
	}

	void ALEditor::Update(void)
	{
		// Check ImGui active
		if (!m_ImGuiEnabled)
			return;

		// Main Menu Bar
		EditorMenuBar();

		// Editor toolbar
		EditorToolbar();

		// Check if Game Mode
		if (m_FullScreen && m_GameIsActive)
		{
			// Set window size to be full screen
			ImVec2 vpSize = ImVec2(ImGui::GetMainViewport()->WorkSize.x,
				ImGui::GetMainViewport()->WorkSize.y - ImGui::FindWindowByName("##toolbar")->Size.y);
			ImVec2 vpPos = ImVec2(ImGui::GetMainViewport()->WorkPos.x,
				ImGui::GetMainViewport()->WorkPos.y + ImGui::FindWindowByName("##toolbar")->Size.y);
			ImGui::SetNextWindowSize(vpSize);
			ImGui::SetNextWindowPos(vpPos);
			m_GamePanel.OnImGuiRender();
		}
		else
		{
			// Save Scene, Ctrl + S
			if (Input::KeyDown(KeyCode::Ctrl) && Input::KeyTriggered(KeyCode::S))
				SaveScene();

			// Content Browser Panel
			m_ContentBrowserPanel.OnImGuiRender();

			// Logger Panel
			m_LoggerPanel.OnImGuiRender();

			if (m_AnimatorEditorPanelEnabled)
			{
				m_AnimatorEditorPanel.OnImGuiRender(m_AnimatorEditorPanelEnabled);
			}

			if (m_AudioPanelEnabled)
			{
				m_AudioEditorPanel.OnImGuiRender(m_AudioPanelEnabled);
			}

			m_TileEditor.OnImGuiRender();

			if (m_SceneBuildOrderPanel.GetPanelIsOpen())
				m_SceneBuildOrderPanel.OnImGuiRender();

			if(m_AnimatorPanel.GetPanelIsOpen())
				m_AnimatorPanel.OnImGuiRender();

			// Check if game is running
			if (m_GameIsActive)
			{
				// Set to be editor scene panel size and pos
				ImVec2 sceneSize = ImGui::FindWindowByName("Editor Scene")->Size;
				ImVec2 scenePos = ImGui::FindWindowByName("Editor Scene")->Pos;
				ImGui::SetNextWindowSize(sceneSize);
				ImGui::SetNextWindowPos(scenePos);
				// Run game panel
				m_GamePanel.OnImGuiRender();
			}
			else
			{	// Run editor scene panel
				m_ScenePanel.OnImGuiRender();	// Scene Panel
			}

			// Scene Hierarchy Panel
			m_SceneHierarchyPanel.OnImGuiRender();

			// Check if there is a selected entity for Inspector
			m_InspectorPanel.OnImGuiRender();	// Inspector Panel

#ifndef NDEBUG
			//ImGui::ShowDemoWindow();
#endif
		}
	}

	void ALEditor::Exit(void)
	{
		// Shutdown imgui
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		// Destroy imgui context
		ImGui::DestroyContext();
	}

	void ALEditor::Begin(void)
	{
		ZoneScopedN("Editor Update");
		// Change ImGui Enabled or Disabled
		
		//if (Input::KeyDown(KeyCode::Ctrl) && Input::KeyTriggered(KeyCode::Key_9))
		//{
		//	m_ImGuiEnabled = !m_ImGuiEnabled;

		//	//ImGuiIO& io = ImGui::GetIO();
		//	//// If it is iactive, set MultiViewport to disable.
		//	//	// This is to stop rendering panels outside of main window
		//	//if (m_ImGuiEnabled)
		//	//{
		//	//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport
		//	//}
		//	//else
		//	//{
		//	//	io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport
		//	//}
		//}

		// New ImGui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Enable DockSpace if it is to be enabled!
		if (m_DockingEnabled)
			Docking();

		// Check ImGui active
		if (!m_ImGuiEnabled)
		{
			End();
			return;
		}

		// ImGuizmo Frame
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::BeginFrame();

		Update();

		m_IsReceivingKBInput = ImGui::GetIO().WantTextInput;
	}

	void ALEditor::End(void)
	{
		// Get the ImGui IO
		ImGuiIO& io = ImGui::GetIO();

		// Set IO delta time
		io.DeltaTime = Time::m_DeltaTime;

		// Set IO display size
		io.DisplaySize = ImVec2(static_cast<f32>(Graphics::OpenGLWindow::width),
			static_cast<f32>(Graphics::OpenGLWindow::height));

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// If viewports enabled, draw window outside of application window!
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* curr_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(curr_context);
		}
	}

	void ALEditor::InitializeStyle(void)
	{
		// Get the style
		ImGuiStyle& style = ImGui::GetStyle();

		// Title Colors
		style.Colors[ImGuiCol_TitleBg] = m_ColorTitleBg;				// Title Background
		style.Colors[ImGuiCol_TitleBgActive] = m_ColorTitleActiveBg;	// Title Active Background	

		// Window Colors
		style.Colors[ImGuiCol_WindowBg] = m_ColorWindowBg;				// Window Background

		// Frame Backgrounds
		style.Colors[ImGuiCol_FrameBg] = m_ColorNormal;					// Normal Color
		style.Colors[ImGuiCol_FrameBgActive] = m_ColorActive;			// Active Color
		style.Colors[ImGuiCol_FrameBgHovered] = m_ColorHovered;			// Hovered Color

		// Tab Colors
		style.Colors[ImGuiCol_Tab] = m_ColorNormal;						// Normal Color
		style.Colors[ImGuiCol_TabHovered] = m_ColorHovered;				// Hovered Color
		style.Colors[ImGuiCol_TabActive] = m_ColorActive;				// Active Color
		style.Colors[ImGuiCol_TabUnfocusedActive] = m_ColorActive;		// Active Color

		// Button Colors
		style.Colors[ImGuiCol_Button] = m_ColorActive;					// Active Color
		style.Colors[ImGuiCol_ButtonActive] = m_ColorActive2;			// Active Color 2
		style.Colors[ImGuiCol_ButtonHovered] = m_ColorHovered;			// Hovered Color

		// Header Colors
		style.Colors[ImGuiCol_Header] = m_ColorActive;					// Active Color
		style.Colors[ImGuiCol_HeaderActive] = m_ColorActive2;			// Active Color 2
		style.Colors[ImGuiCol_HeaderHovered] = m_ColorHovered;			// Hovered Color

		// Misc.
		style.Colors[ImGuiCol_CheckMark] = m_ColorInteractive;			// Interactive Color
		style.Colors[ImGuiCol_SliderGrab] = m_ColorInteractive;			// Interactive Color
		style.Colors[ImGuiCol_SliderGrabActive] = m_ColorActive3;		// Active Color 3

	}

	void ALEditor::EditorMenuBar(void)
	{
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::SetNextWindowSize({ m_MenuSize.x, 0.f });
			// Settings
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::Selectable("Create New Scene##MainMenuBar"))
				{
					SetSelectedEntity(ECS::MAX_ENTITIES);
					CreateScene();
				}

				if (ImGui::Selectable("Save Scene##MainMenuBar"))
					SaveScene();

				if (ImGui::Selectable("Save Scene As...##MainMenuBar"))
				{
					std::string tempName = m_CurrentSceneName;
					m_CurrentSceneName = "";
					SaveScene();

					if (m_CurrentSceneName == "")
						m_CurrentSceneName = tempName;
				}

				if (ImGui::Selectable("Load Scene"))
				{
					m_CurrentSceneName = Utility::WindowsFileDialog::LoadFile("ALEngine Scene (*.scene)\0*.scene\0");

					if (!m_CurrentSceneName.empty())
					{
						if (m_CurrentSceneName.rfind(".scene") == std::string::npos)
							m_CurrentSceneName += ".scene";

						u64 str_it = m_CurrentSceneName.rfind("Assets\\");
						m_CurrentSceneName = m_CurrentSceneName.substr(str_it, m_CurrentSceneName.size());
						Engine::Scene::LoadScene(m_CurrentSceneName.c_str());
					}
				}

				// Set to fullscreen or normal
				ImGui::Checkbox("Fullscreen##MainMenuBar", &m_FullScreen);

				ImGui::EndMenu();
			}

			ImGui::SetNextWindowSize({ m_MenuSize.x, 0.f });
			if (ImGui::BeginMenu("Tools"))
			{
				// Selectable flag
				ImGuiSelectableFlags flag = 0;

				// Set active for animator panel
				ImGui::Selectable("Create Clips/Animation", &m_AnimatorEditorPanelEnabled, flag);

				//  Set active for audio panel
				ImGui::Selectable("Audio Mixer", &m_AudioPanelEnabled, flag);

				//  Set active for audio panel
				if (ImGui::MenuItem("Tile Editor"))
					m_TileEditor.SetPanelIsOpen(true);

				// Set Active for Scene Build Order
				if (ImGui::MenuItem("Scene Build Order"))
					m_SceneBuildOrderPanel.SetPanelIsOpen(true);

				// Set active for animator panel
				if (ImGui::MenuItem("Animator Editor"))
					m_AnimatorPanel.SetPanelIsOpen(true);

#ifdef TRACY_ENABLE
				if (ImGui::MenuItem("Tracy Profiler"))
				{
					if (!m_ProfilerRunning)
					{
						TCHAR path[MAX_PATH];
						GetCurrentDirectory(MAX_PATH, path);

						[[maybe_unused]] HANDLE hProcess{ nullptr };
						[[maybe_unused]] HANDLE hThread{ nullptr };
						STARTUPINFO si;
						PROCESS_INFORMATION pi;
						[[maybe_unused]] DWORD dwProcessID{ 0 };
						[[maybe_unused]] DWORD dwThreadID{ 0 };

						ZeroMemory(&pi, sizeof(pi));
						ZeroMemory(&si, sizeof(si));
						si.cb = sizeof(si);
						BOOL bCreateProcess{ NULL };
						
						std::string abs_path = path;
						abs_path += +TRACY_FILEPATH;
						bCreateProcess = CreateProcess(abs_path.c_str(),
											nullptr,
											nullptr,
											nullptr,
											FALSE,
											0,
											nullptr,
											nullptr,
											&si,
											&pi);
						
						if (bCreateProcess == FALSE)
							AL_CORE_CRITICAL("Failed to Create Process, Tracy.exe not running.");
						else
							m_ProfilerRunning = true;

						// Close Process and Thread Handles
						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);
					}
				}
#endif

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void ALEditor::EditorToolbar(void)
	{
		// Filepaths for play button
		static const std::string play_button_fp{ "Assets/Dev/Images/button_play.png" };
		static const std::string stop_button_fp{ "Assets/Dev/Images/button_stop.png" };
		static const f32 btn_size{ 20.f };

		ImGui::SetNextWindowSizeConstraints(ImVec2(25.f, 25.f), ImVec2(1000.f, 25.f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

		// Window flags
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

		// Set constraints
		ImGui::SetNextWindowSizeConstraints(m_ToolbarMinSize, ImGui::GetMainViewport()->WorkSize);

		// Toolbar!!
		if (ImGui::Begin("##toolbar", nullptr, flags))
		{
			// Texture GuID
			Guid id{ 0 };

			// Get texture Guid
			if (m_GameIsActive)
				id = Engine::AssetManager::Instance()->GetGuid(stop_button_fp);
			else
				id = Engine::AssetManager::Instance()->GetGuid(play_button_fp);

			// Get texture
			u64 tex = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);

			// Get Window Height
			f32 btnSize = ImGui::GetContentRegionAvail().y - 7.f;

			// Make button centered
			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (btnSize * 0.5f));

			// Play/Stop button
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex), ImVec2(btnSize, btnSize)))
			{
				m_GameIsActive = !m_GameIsActive;
				Engine::ToggleApplicationMode();
				// Go into game scene, save state
				if (m_GameIsActive)
				{
					SetSelectedEntity(ECS::MAX_ENTITIES);
					Engine::Scene::SaveState();
					Engine::GameStateManager::next = Engine::GameState::Gameplay;
					Engine::GameStateManager::current = Engine::GameState::Gameplay;
					//ECS::StartGameplaySystem();
				}
				else
				{
					ECS::ParticleSystem::GetParticleSystem().ClearParticles();
					//ECS::ExitGameplaySystem();
					//Coordinator::Instance()->DestroyEntities();
					
					Engine::GameStateManager::Next(Engine::GameState::Editor);
					m_InspectorPanel.SetSelectedEntity(ECS::MAX_ENTITIES);
				}
			}
			ImGui::End();
		}

		// Pop style variable
		ImGui::PopStyleVar();
	}

	void ALEditor::LoadData(void)
	{
		// Math namespace
		using namespace Math;

		// Get data
		Serializer::Serializer editor_data{ "Assets/Dev/Objects/EditorDefaults.json" };

		//=========================================================
		// Load editor style values
		//=========================================================
		// Store Colors
		Vec3 col{};

		// Title Bg
		col = editor_data.GetVec3("ColorTitleBg", Vec3());
		m_ColorTitleBg = ImVec4(col.x, col.y, col.z, 1.f);
		// Title Active Bg
		col = editor_data.GetVec3("ColorTitleActiveBg", Vec3());
		m_ColorTitleActiveBg = ImVec4(col.x, col.y, col.z, 1.f);
		// Window Bg
		col = editor_data.GetVec3("ColorWindowBg", Vec3());
		m_ColorWindowBg = ImVec4(col.x, col.y, col.z, 1.f);
		// Normal Color
		col = editor_data.GetVec3("ColorNormal", Vec3());
		m_ColorNormal = ImVec4(col.x, col.y, col.z, 1.f);
		// Active
		col = editor_data.GetVec3("ColorActive", Vec3());
		m_ColorActive = ImVec4(col.x, col.y, col.z, 1.f);
		// Active 2
		col = editor_data.GetVec3("ColorActive2", Vec3());
		m_ColorActive2 = ImVec4(col.x, col.y, col.z, 1.f);
		// Active 3
		col = editor_data.GetVec3("ColorActive3", Vec3());
		m_ColorActive3 = ImVec4(col.x, col.y, col.z, 1.f);
		// Hovered
		col = editor_data.GetVec3("ColorHovered", Vec3());
		m_ColorHovered = ImVec4(col.x, col.y, col.z, 1.f);
		// Interactive
		col = editor_data.GetVec3("ColorInteractive", Vec3());
		m_ColorInteractive = ImVec4(col.x, col.y, col.z, 1.f);

		//=========================================================
		// Load Panel Values
		//=========================================================
		// Toolbar Min
		Math::Vec2 toolbarMin = editor_data.GetVec2("ToolbarMin", Math::Vec2());
		m_ToolbarMinSize = ImVec2(toolbarMin.x, toolbarMin.y);

		// Panel Min Size
		Vec2 panel_min = editor_data.GetVec2("PanelMin", Vec2());

		// Content Browser Panel
		m_ContentBrowserPanel.SetPanelMin(panel_min);
		m_ContentBrowserPanel.SetDefaults(editor_data.GetVec2("ContentBrowserPos", Math::Vec2()),
			editor_data.GetVec2("ContentBrowserSize", panel_min));

		// Scene Panel
		m_ScenePanel.SetPanelMin(panel_min);
		m_ScenePanel.SetDefaults(editor_data.GetVec2("ScenePos", Math::Vec2()),
			editor_data.GetVec2("SceneSize", panel_min));

		// Game Panel
		m_GamePanel.SetPanelMin(panel_min);
		m_GamePanel.SetDefaults(editor_data.GetVec2("GamePos", Math::Vec2()),
			editor_data.GetVec2("GameSize", panel_min));

		// Scene Hierarchy Panel
		m_SceneHierarchyPanel.SetPanelMin(panel_min);
		m_SceneHierarchyPanel.SetDefaults(editor_data.GetVec2("SceneHierarchyPos", Math::Vec2()),
			editor_data.GetVec2("SceneHierarchySize", panel_min));

		// Logger Panel
		m_LoggerPanel.SetPanelMin(panel_min);
		m_LoggerPanel.SetDefaults(editor_data.GetVec2("LoggerPos", Math::Vec2()),
			editor_data.GetVec2("LoggerSize", panel_min));

		// Inspector Panel
		m_InspectorPanel.SetPanelMin(panel_min);
		m_InspectorPanel.SetDefaults(editor_data.GetVec2("InspectorPos", Math::Vec2()),
			editor_data.GetVec2("InspectorSize", panel_min));

		// Create Clip animator panel
		m_AnimatorEditorPanel.SetPanelMin(panel_min);
		m_AnimatorEditorPanel.SetDefaults(editor_data.GetVec2("AnimatorPos", Math::Vec2()),
			editor_data.GetVec2("AnimatorSize", panel_min));

		// Create Clip audio panel
		m_AudioEditorPanel.SetPanelMin(panel_min);
		m_AudioEditorPanel.SetDefaults(editor_data.GetVec2("AudioPos", Math::Vec2()),
			editor_data.GetVec2("AudioSize", panel_min));

		// Tile Editor Panel
		m_TileEditor.SetPanelMin(panel_min);

		// Animator Panel
		m_AnimatorPanel.SetPanelMin(panel_min);

	}

	void ALEditor::LoadMap(void)
	{
		using namespace Gameplay;
		const std::string PARENT_NAME{ "Map Parent" };

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();

		std::vector<std::vector<std::string>> map = MapManager::Instance()->GetMap();

		// Width and Height of map
		u32 width{ MapManager::Instance()->GetWidth() }, 
			height{ MapManager::Instance()->GetHeight() };
		f32 mapSizeW{ static_cast<f32>(width * 100) }, 
			mapSizeH{ static_cast<f32>(height * 100) };
		f32 halfmapSizeW{ mapSizeW * 0.5f },
			halfMapSizeH{ mapSizeH * 0.5f };
		u32 w{ 0 }, h{ 0 };

		// Create parent entity that will be invisible
		ECS::Entity parent = Coordinator::Instance()->GetEntityByTag(PARENT_NAME.c_str());

		// Destroy previous parent
		if (parent != ECS::MAX_ENTITIES)
		{
			sceneGraph.Destruct((s32)parent);
		}

		parent = Coordinator::Instance()->CreateEntity(PARENT_NAME.c_str());
		Coordinator::Instance()->AddComponent(parent, Transform{});
		sceneGraph.Push(-1, parent);

		// Do each 
		for (auto col : map)
		{
			/*
			ECS::Entity rowTiles = Coordinator::Instance()->CreateEntity();
			Coordinator::Instance()->AddComponent(rowTiles, Transform{});

			// Set name
			EntityData& rowEnttData = Coordinator::Instance()->GetComponent<EntityData>(rowTiles);
			rowEnttData.tag = "Col " + std::to_string(h);

			sceneGraph.Push((s32)parent, (s32)rowTiles);	
			*/

			w = 0;
			for (auto row : col)
			{
				ECS::Entity tile = Coordinator::Instance()->CreateEntity();
				//sceneGraph.Push((s32)rowTiles, (s32)tile);
				sceneGraph.Push(parent, tile);

				// Set default transform and scale
				Transform transform;
				transform.scale = { 100, 100 };
				transform.localScale = { 100, 100 };
				transform.position = { -halfmapSizeW + (f32)w * 100.f, -halfMapSizeH + (f32)h * 100.f };

				// Set name
				EntityData &data = Coordinator::Instance()->GetComponent<EntityData>(tile);
				data.tag = row;

				Coordinator::Instance()->AddComponent(tile, transform);

				if (row != "Empty")
				{
					std::string tile_fp = MapManager::Instance()->GetTileImage(row);
					if (!tile_fp.empty())
						ECS::CreateSprite(tile, tile_fp.c_str());
				}
				++w;
			}
			++h;
		}
	}

	void ALEditor::SetDefaultPanel(void)
	{
		// Set default sizes and positions

	}

	void ALEditor::Docking(void)
	{
		// Ensure the parent window is not dockable into
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		// Get main viewport
		const ImGuiViewport* vp = ImGui::GetMainViewport();

		// Set next window info
		ImGui::SetNextWindowPos(vp->WorkPos);
		ImGui::SetNextWindowSize(ImVec2(vp->WorkSize));
		ImGui::SetNextWindowViewport(vp->ID);

		// Window style
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		// Set window flags
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// Make Dockspace active
		static bool show{ true };
		ImGui::Begin("ALEditor!", &show, window_flags);
		// Pop window styles out
		ImGui::PopStyleVar(3);

		ImGuiDockNodeFlags dock_flag = 0;

		if (!m_ImGuiEnabled)
		{
			dock_flag = ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// Enable dockspace
		ImGui::DockSpace(ImGui::GetID("DockSpace"), ImVec2(), dock_flag);

		// Make Dockspace inactive
		ImGui::End();
	}

	void ALEditor::SaveScene(void)
	{
		const u32 nameSize{ 50 };
		static c8 scene_name[nameSize]{};
		static b8 rejectName{ false };

		if (m_CurrentSceneName.empty())
		{
			ImGui::OpenPopup("New Scene Name##ModalSaveScene");
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			m_CurrentSceneName = Utility::WindowsFileDialog::SaveFile("ALEngine Scene (*.scene)\0*.scene\0");

			if (!m_CurrentSceneName.empty())
			{
				if (m_CurrentSceneName.rfind(".scene") == std::string::npos)
					m_CurrentSceneName += ".scene";

				u64 str_it = m_CurrentSceneName.rfind("Assets\\");
				m_CurrentSceneName = m_CurrentSceneName.substr(str_it, m_CurrentSceneName.size());	
			}
		}
		Engine::Scene::SaveScene(m_CurrentSceneName.c_str());
		AL_CORE_INFO("Scene {} Saved!", m_CurrentSceneName);
	}
	
	void ALEngine::Editor::ALEditor::CreateScene(void)
	{
		m_CurrentSceneName = "";
		Engine::Scene::LoadScene(EMPTY_SCENE_FP);
	}
}

// Getters and Setters
namespace ALEngine::Editor
{
	void ALEditor::SetImGuiEnabled(b8 isEnabled)
	{
		m_ImGuiEnabled = isEnabled;
	}

	b8 ALEditor::GetImGuiEnabled(void)
	{
		return m_ImGuiEnabled;
	}

	void ALEditor::SetDockingEnabled(b8 isEnabled)
	{
		m_DockingEnabled = isEnabled;
	}

	void ALEditor::SetSelectedEntity(ECS::Entity setter)
	{
		m_InspectorPanel.SetSelectedEntity(setter);
	}

	const ECS::Entity ALEditor::GetSelectedEntity(void)
	{
		return m_InspectorPanel.GetSelectedEntity();
	}

	ImGuizmo::OPERATION ALEditor::GetCurrentGizmoOperation(void)
	{
		return m_CurrentGizmoOperation;
	}

	void ALEditor::SetCurrentGizmoOperation(ImGuizmo::OPERATION _op)
	{
		m_CurrentGizmoOperation = _op;
	}

	f64 ALEditor::GetSceneWidth(void)
	{
		if (m_GameIsActive)
			return m_GamePanel.GetGamePanelWidth();
		return m_ScenePanel.GetSceneWidth();
	}

	f64 ALEditor::GetSceneHeight(void)
	{
		if (m_GameIsActive)
			return m_GamePanel.GetGamePanelHeight();
		return m_ScenePanel.GetSceneHeight();
	}

	f32& ALEditor::GetSceneCameraWidth(void)
	{
		return m_ScenePanel.GetCameraWidth();
	}

	f32& ALEditor::GetSceneCameraHeight(void)
	{
		return m_ScenePanel.GetCameraHeight();
	}

	Engine::Camera& ALEditor::GetEditorCamera(void)
	{
		return m_ScenePanel.GetEditorCamera();
	}

	Math::Vec2 ALEditor::GetMouseWorldPos()
	{
		if (m_GameIsActive)
			return m_GamePanel.GetMouseWorldPos();
		return m_ScenePanel.GetMouseWorldPos();
	}

	Math::Vec2 ALEditor::GetMousePosWRTPanel()
	{
		if (m_GameIsActive)
			return m_GamePanel.GetMousePosWRTPanel();
		return m_ScenePanel.GetMousePosWRTPanel();
	}

	Math::Vec2 ALEditor::WorldToScreenPosVec(Math::Vec2 pos)
	{
		if (m_GameIsActive)
			return m_GamePanel.WorldToScreenPosVec(pos);
		return m_ScenePanel.WorldToScreenPosVec(pos);
	}

	b8 ALEditor::GetGameActive(void)
	{
		return m_GameIsActive;
	}

	void ALEditor::SetGameActive(b8 gameActive)
	{
		m_GameIsActive = gameActive;
	}

	b8 ALEditor::GetReceivingKBInput(void)
	{
		return m_IsReceivingKBInput;
	}

	void ALEditor::SetReceivingKBInput(b8 receivingInput)
	{
		m_IsReceivingKBInput = receivingInput;
	}

	b8 ALEditor::GetEditorInFocus(void)
	{
		return m_EditorInFocus;
	}

	void ALEditor::SetCurrentSceneName(std::string sceneName)
	{
		m_CurrentSceneName = sceneName;
	}
	std::string const& ALEditor::GetCurrentSceneName(void) const
	{
		return m_CurrentSceneName;
	}

	void ALEditor::SetCurrentTileMapPath(std::string tileMap)
	{
		m_CurrentTileMapPath = tileMap;
	}

	std::string const& ALEditor::GetCurrentTileMapPath(void) const
	{
		return m_CurrentTileMapPath;
	}

	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}

#endif