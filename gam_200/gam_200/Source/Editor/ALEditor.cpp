#include "pch.h"

namespace ManCong
{
	namespace Engine
	{
		void ALEditor::Init()
		{
			// Check ImGui version
			IMGUI_CHECKVERSION();

			// Create ImGui context
			ImGui::CreateContext();

			// Set imgui style to dark
			ImGui::StyleColorsDark();

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
		}

		void ALEditor::Update()
		{
			static bool show = true;
			ImGui::ShowDemoWindow(&show);
		}

		void ALEditor::Begin()
		{
			// New ImGui Frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void ALEditor::End()
		{
			// Get the ImGui IO
			ImGuiIO& io = ImGui::GetIO();

			// Set IO delta time
			io.DeltaTime = Time::m_DeltaTime;

			// Set IO display size
			io.DisplaySize = ImVec2(static_cast<float>(Graphics::OpenGLWindow::width),
				static_cast<float>(Graphics::OpenGLWindow::height));

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
	}
}