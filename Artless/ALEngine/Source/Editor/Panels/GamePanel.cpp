#include "pch.h"

namespace ALEngine::Editor
{
	GamePanel::GamePanel(void)
	{
	}

	GamePanel::~GamePanel(void)
	{
	}

	void GamePanel::OnImGuiRender(void)
	{
		// Set position & size of Game Scene
		//ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2(Graphics::OpenGLWindow::width, 
			Graphics::OpenGLWindow::height));

		// Begin ImGui Panel
		if (!ImGui::Begin("Game"))
		{
			ImGui::End();
			return;
		}

		ImGui::Image((void*)ECS::GetFBTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		// End ImGui Panel
		ImGui::End();
	}
}