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
	}

	void ALEditor::Update(void)
	{
		// Check ImGui active
		if (!m_ImGuiEnabled)
			return;

		// Content Browser Panel
		m_ContentBrowserPanel.OnImGuiRender();

		// Logger Panel
		m_LoggerPanel.OnImGuiRender();

		// Check if there is a selected entity for Inspector
		if (m_InspectorPanel.HasSelectedEntity())
			m_InspectorPanel.OnImGuiRender();	// Inspector Panel

		// Set selected entity for Scene Panel (for Gizmos)
		m_ScenePanel.SetSelectedEntity(m_InspectorPanel.GetSelectedEntity());
		m_ScenePanel.SetCurrentGizmoOperation(m_InspectorPanel.GetCurrGizmoOperation());
		m_ScenePanel.OnImGuiRender();	// Scene Panel

		// Update if selected entity has changed
		m_InspectorPanel.SetSelectedEntity(m_ScenePanel.GetSelectedEntity());

		// Scene Hierarchy Panel
		m_SceneHierarchyPanel.OnImGuiRender();

		// Profiler Panel
		m_ProfilerPanel.OnImGuiRender();
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

	void ALEditor::Docking(void)
	{
		// Ensure the parent window is not dockable into
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		
		// Get main viewport
		const ImGuiViewport* vp = ImGui::GetMainViewport();

		// Set next window info
		ImGui::SetNextWindowPos(vp->WorkPos);
		ImGui::SetNextWindowSize(vp->WorkSize);
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