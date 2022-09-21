#include <pch.h>
#include <spdlog/sinks/ostream_sink.h>

using namespace ALEngine::Editor;

LoggerPanel::LoggerPanel()
{
	auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(logger_oss);
	ALEngine::Exceptions::Logger::GetCoreLogger()->sinks().push_back(ostream_sink);
	AL_CORE_SET_PATTERN("%^[%n] [%l] [%D %T] %v%$");
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
		AL_CORE_ERROR("Logger Panel Collapsed");
		return;
	}

	// Align text baseline to the baseline of the panel
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Log Levels:");
	//ImGui::SameLine(); //CheckboxFlags("All");


	// The Text Box
	ImGui::BeginChild("LoggerTextBox", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

	// Change OSS to ISS
	std::string log_line;
	std::istringstream iss(logger_oss.str());

	// Check each line for 
	while (std::getline(iss, log_line))
	{
		switch (log_line.c_str()[6])
		{
		case 'e':
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::TextUnformatted(log_line.c_str());
			ImGui::PopStyleColor();
			break;
		case 'd':
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));
			ImGui::TextUnformatted(log_line.c_str());
			ImGui::PopStyleColor();
			break;
		}
	}
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
	//End ImGui Window
	ImGui::End();
}