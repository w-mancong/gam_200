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
	if (!ImGui::Begin("Logger"))
	{
		ImGui::End();
		//AL_CORE_ERROR("Logger Panel Collapsed");
		return;
	}

	// Align text baseline to the baseline of the panel
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Log Levels:");
	//ImGui::SameLine(); //CheckboxFlags("All");

	std::stringstream buffer;
	std::streambuf* o = std::cout.rdbuf(buffer.rdbuf());

	// The Text Box
	ImGui::BeginChild("LoggerTextBox", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	ImGui::TextUnformatted(buffer.str().c_str());
	ImGui::EndChild();
	//End ImGui Window
	ImGui::End();
}