/*!
file:	ALEditor.cpp
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the ALEditor class

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

namespace ALEngine::Editor
{
	void ALEditor::Init()
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

	void ALEditor::Exit()
	{
		// Shutdown imgui
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		// Destroy imgui context
		ImGui::DestroyContext();
	}

	void ALEditor::Update()
	{
		// Check ImGui active
		if (!m_ImGuiEnabled)
			return;

		cbp.OnImGuiRender();
		logger_panel.OnImGuiRender();
		if (imguizmo_panel.HasEntityTransform())
			imguizmo_panel.OnImGuiRender();
		hierarchy_panel.OnImGuiRender();
	}

	void ALEditor::Begin()
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

	void ALEditor::End()
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
	
	void ALEditor::Docking()
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