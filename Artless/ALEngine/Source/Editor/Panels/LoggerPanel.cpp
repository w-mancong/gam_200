#include "pch.h"

using namespace ALEngine::Editor;

LoggerPanel::LoggerPanel()
{

}

LoggerPanel::~LoggerPanel()
{

}

void LoggerPanel::OnImGuiRender()
{
	// Start ImGui Window
	if (ImGui::Begin("Logger"))
	{
		ImGui::End();
		//AL_CORE_ERROR("Logger Panel collapsed");
		return;
	}


	//End ImGui Window
	ImGui::End();
}