#include "pch.h"

namespace ManCong
{
	namespace Engine
	{
		void ALEditor::Init(GLFWwindow* win)
		{
			// Check ImGui version
			IMGUI_CHECKVERSION();

			// Create ImGui context
			ImGui::CreateContext();

			// Set imgui style to dark
			ImGui::StyleColorsDark();

			// ImGui IO stuff
			ImGuiIO& io = ImGui::GetIO();
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

			// Init GLFW
			ImGui_ImplGlfw_InitForOpenGL(win, true);

			// Set GLSL version
			ImGui_ImplOpenGL3_Init("#version 450");
		}

		void ALEditor::Update()
		{
			// New ImGui Frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Get the ImGui IO
			ImGuiIO& io = ImGui::GetIO();

			// Set IO delta time
			io.DeltaTime = Time::m_DeltaTime;

			// Set IO display size
			io.DisplaySize = ImVec2(static_cast<float>(Graphics::OpenGLWindow::width), 
				static_cast<float>(Graphics::OpenGLWindow::height));

			static bool show = true;
			ImGui::ShowDemoWindow(&show);
		}
		
		void ALEditor::Render()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
}