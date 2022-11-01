/*!
file:	ALEditor.cpp
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function definitions for the ALEditor class.
		The ALEditor class essentially manages the Dear ImGui functions, as well as the
		different editor panels generated with the help of Dear ImGui.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

#include "imgui.h"
#include "imgui_internal.h"

namespace ALEngine::Editor
{
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

		// Init GLFW
		ImGui_ImplGlfw_InitForOpenGL(Graphics::OpenGLWindow::Window() , true);
		// Set GLSL version
		ImGui_ImplOpenGL3_Init("#version 450");

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
			// Content Browser Panel
			m_ContentBrowserPanel.OnImGuiRender();

			// Logger Panel
			m_LoggerPanel.OnImGuiRender();

			// Check if there is a selected entity for Inspector
			m_InspectorPanel.OnImGuiRender();	// Inspector Panel

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
				// Set selected entity for Scene Panel (for Gizmos)
				m_ScenePanel.SetSelectedEntity(m_InspectorPanel.GetSelectedEntity());
				m_ScenePanel.SetCurrentGizmoOperation(m_InspectorPanel.GetCurrGizmoOperation());
				m_ScenePanel.OnImGuiRender();	// Scene Panel
			}

			// Update if selected entity has changed
			m_InspectorPanel.SetSelectedEntity(m_ScenePanel.GetSelectedEntity());

			// Scene Hierarchy Panel
			m_SceneHierarchyPanel.OnImGuiRender();

			// Profiler Panel
			//m_ProfilerPanel.OnImGuiRender();
			ImGui::ShowDemoWindow();
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
		ZoneScopedN("Editor Update")
		// Change ImGui Enabled or Disabled
		if (Input::KeyTriggered(KeyCode::Key_9))
		{
			m_ImGuiEnabled = !m_ImGuiEnabled;

			ImGuiIO& io = ImGui::GetIO();
			// If it is iactive, set MultiViewport to disable. 
				// This is to stop rendering panels outside of main window
			if (m_ImGuiEnabled)
			{
				io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport
				ImGui::UpdatePlatformWindows();
			}
			else
				io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport
		}

		// New ImGui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Check ImGui active
		if (!m_ImGuiEnabled)
		{
			End();
			return;
		}

		// ImGuizmo Frame
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::BeginFrame();

		// Enable DockSpace if it is to be enabled!
		if(m_DockingEnabled)
			Docking();

		Update();
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
		style.Colors[ImGuiCol_TitleBg]				= ImVec4(0.25f, 0.f, 0.45f, 1.f);
		style.Colors[ImGuiCol_TitleBgActive]		= ImVec4(0.5f, 0.2f, 0.7f, 1.f);

		// Window Colors
		style.Colors[ImGuiCol_WindowBg]				= ImVec4(0.05f, 0.f, 0.1f, 1.f);

		// Frame Backgrounds
		style.Colors[ImGuiCol_FrameBg]				= ImVec4(0.05f, 0.2f, 0.05f, 1.f);
		style.Colors[ImGuiCol_FrameBgActive]		= ImVec4(0.1f, 0.4f, 0.1f, 1.f);
		style.Colors[ImGuiCol_FrameBgHovered]		= ImVec4(0.08f, 0.3f, 0.08f, 1.f);

		// Tab Colors
		style.Colors[ImGuiCol_Tab]					= ImVec4(0.05f, 0.2f, 0.05f, 1.f);
		style.Colors[ImGuiCol_TabHovered]			= ImVec4(0.08f, 0.3f, 0.08f, 1.f);
		style.Colors[ImGuiCol_TabActive]			= ImVec4(0.1f, 0.4f, 0.1f, 1.f);
		style.Colors[ImGuiCol_TabUnfocusedActive]	= ImVec4(0.1f, 0.4f, 0.1f, 1.f);

		// Button Colors
		style.Colors[ImGuiCol_Button]				= ImVec4(0.1f, 0.4f, 0.1f, 1.f);
		style.Colors[ImGuiCol_ButtonActive]			= ImVec4(0.1f, 0.35f, 0.1f, 1.f);
		style.Colors[ImGuiCol_ButtonHovered]		= ImVec4(0.08f, 0.3f, 0.08f, 1.f);

		// Header Colors
		style.Colors[ImGuiCol_Header]				= ImVec4(0.1f, 0.4f, 0.1f, 1.f);
		style.Colors[ImGuiCol_HeaderActive]			= ImVec4(0.1f, 0.35f, 0.1f, 1.f);
		style.Colors[ImGuiCol_HeaderHovered]		= ImVec4(0.08f, 0.3f, 0.08f, 1.f);

		// Misc.
		style.Colors[ImGuiCol_CheckMark]			= ImVec4(0.1f, 0.8f, 0.1f, 1.f);
		style.Colors[ImGuiCol_SliderGrab]			= ImVec4(0.1f, 0.8f, 0.1f, 1.f);
		style.Colors[ImGuiCol_SliderGrabActive]		= ImVec4(0.1f, 0.75f, 0.1f, 1.f);

	}

	void ALEditor::EditorMenuBar(void)
	{
		if (ImGui::BeginMainMenuBar())
		{
			// Settings
			if (ImGui::BeginMenu("Settings"))
			{
				// Selectable flag
				ImGuiSelectableFlags flag = 0;

				// Set to fullscreen or normal
				ImGui::Selectable("Fullscreen", &m_FullScreen, flag);

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void ALEditor::EditorToolbar(void)
	{
		// Filepaths for play button
		static const std::string play_button_fp{ "Assets/Images/button play.png" };
		static const std::string stop_button_fp{ "Assets/Images/button stop.png" };
		static const f32 btn_size{ 20.f };

		ImGui::SetNextWindowSizeConstraints(ImVec2(25.f, 25.f), ImVec2(1000.f, 25.f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

		// Window flags
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

		// Toolbar!!
		if (ImGui::Begin("##toolbar", nullptr, flags))
		{
			// Texture GuID
			Guid id{ 0 };

			// Get texture Guid
			if(m_GameIsActive)
				id = Engine::AssetManager::Instance()->GetGuid(stop_button_fp);
			else
				id = Engine::AssetManager::Instance()->GetGuid(play_button_fp);

			// Get texture
			u64 tex = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);

			// Make button centered
			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (btn_size * 0.5f));

			// Play/Stop button
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex), ImVec2(btn_size, btn_size)))
			{
				m_GameIsActive = !m_GameIsActive;
			}
			ImGui::End();
		}

		// Pop style variable
		ImGui::PopStyleVar();
	}
	
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

	f64 ALEditor::GetSceneWidth(void)
	{
		return m_ScenePanel.GetSceneWidth();
	}

	f64 ALEditor::GetSceneHeight(void)
	{
		return m_ScenePanel.GetSceneHeight();
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

	b8 ALEditor::GetGameActive(void)
	{
		return m_GameIsActive;
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
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// Make Dockspace active
		static bool show{ true };
		ImGui::Begin("ALEditor!", &show, window_flags);
		// Pop window styles out
		ImGui::PopStyleVar(3);

		// Enable dockspace
		ImGui::DockSpace(ImGui::GetID("DockSpace"));

		// Make Dockspace inactive
		ImGui::End();
	}
}