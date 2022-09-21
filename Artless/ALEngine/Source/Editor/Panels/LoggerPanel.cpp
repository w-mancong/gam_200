#include <pch.h>
#include <spdlog/sinks/ostream_sink.h>

namespace ALEngine
{
	namespace Editor
	{
		LoggerPanel::LoggerPanel()
		{
			// Create spdlog sink to ostream to print log on imgui
			auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(logger_oss);
			ALEngine::Exceptions::Logger::GetCoreLogger()->sinks().push_back(ostream_sink);
			AL_CORE_SET_PATTERN("%^[%n] [%l] [%D %T] %v%$");

			// Set logger flags to all
			log_flags = LOG_FLAGS::LOG_ALL;
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
			ImGui::SameLine(); ImGui::CheckboxFlags("All", &log_flags, LOG_FLAGS::LOG_ALL);
			ImGui::SameLine(); ImGui::CheckboxFlags("Trace", &log_flags, LOG_FLAGS::LOG_TRACE);
			ImGui::SameLine(); ImGui::CheckboxFlags("Debug", &log_flags, LOG_FLAGS::LOG_DEBUG);
			ImGui::SameLine(); ImGui::CheckboxFlags("Info", &log_flags, LOG_FLAGS::LOG_INFO);
			ImGui::SameLine(); ImGui::CheckboxFlags("Warn", &log_flags, LOG_FLAGS::LOG_WARN);
			ImGui::SameLine(); ImGui::CheckboxFlags("Error", &log_flags, LOG_FLAGS::LOG_ERROR);
			ImGui::SameLine(); ImGui::CheckboxFlags("Critical", &log_flags, LOG_FLAGS::LOG_CRITICAL);

			// The Text Box
			ImGui::BeginChild("LoggerTextBox", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

			// Change OSS to ISS
			std::string log_line;
			std::istringstream iss(logger_oss.str());

			//iss.seekg()

			// Check each line for 
			while (std::getline(iss, log_line))
			{
				// Text Color
				ImU32 text_clr = IM_COL32_BLACK;

				// Bool to print text
				b8 to_print{ false };

				// Format: "[AL] [_...], where the second bracket contains the level"
				switch (log_line.c_str()[6])
				{
				case 'e':	// Error
				case 'E':
					if (!(log_flags & LOG_FLAGS::LOG_ERROR))
						continue;
					to_print = true;
					text_clr = IM_COL32(255, 128, 0, 255);
					break;
				case 'd':	// Debug
				case 'D':
					if (!(log_flags & LOG_FLAGS::LOG_DEBUG))
						continue;
					to_print = true;
					text_clr = IM_COL32(0, 128, 255, 255);
					break;
				case 't':	// Trace
				case 'T':
					if (!(log_flags & LOG_FLAGS::LOG_TRACE))
						continue;
					to_print = true;
					text_clr = IM_COL32(255, 255, 255, 255);
					break;
				case 'w':	// Warning
				case 'W':
					if (!(log_flags & LOG_FLAGS::LOG_WARN))
						continue;
					to_print = true;
					text_clr = IM_COL32(255, 255, 0, 255);
					break;
				case 'i':	// Info
				case 'I':
					if (!(log_flags & LOG_FLAGS::LOG_INFO))
						continue;
					to_print = true;
					text_clr = IM_COL32(0, 255, 0, 255);
					break;
				case 'c':	// Critical
				case 'C':
					if (!(log_flags & LOG_FLAGS::LOG_CRITICAL))
						continue;
					to_print = true;
					text_clr = IM_COL32(255, 0, 0, 255);
					break;
				}

				if (to_print)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, text_clr);
					ImGui::TextUnformatted(log_line.c_str());
					ImGui::PopStyleColor();
				}
			}

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
			ImGui::EndChild();
			//End ImGui Window
			ImGui::End();
		}
	}
}