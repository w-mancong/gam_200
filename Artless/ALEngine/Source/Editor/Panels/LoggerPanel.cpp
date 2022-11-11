/*!
file:	LoggerPanel.cpp
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function definitions for the LoggerPanel class.
		The LoggerPanel class contains information and functions necessary for the 
		Logger Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

#ifdef EDITOR

#include <spdlog/sinks/ostream_sink.h>

namespace ALEngine::Editor
{
	LoggerPanel::LoggerPanel()
	{
		// Create spdlog sink to ostream to print log on imgui
		auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(m_LoggerOSS);
		ALEngine::Exceptions::Logger::GetCoreLogger()->sinks().push_back(ostream_sink);
		AL_CORE_SET_PATTERN("%^[%n] [%l] [%D %T] %v%$");

		spdlog::flush_every(std::chrono::seconds(5));

		// Set logger flags to all
		m_LogFlags = (u32)LOG_FLAGS::LOG_ALL;
	}

	LoggerPanel::~LoggerPanel()
	{

	}

	void LoggerPanel::OnImGuiRender()
	{
		// Set size constraints of inspector
		//ImGui::SetNextWindowSizeConstraints(PANEL_MIN, PANEL_MAX);

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
		ImGui::SameLine(); ImGui::CheckboxFlags("All", &m_LogFlags, (u32)LOG_FLAGS::LOG_ALL);
		ImGui::SameLine(); ImGui::CheckboxFlags("Trace", &m_LogFlags, (u32)LOG_FLAGS::LOG_TRACE);
		ImGui::SameLine(); ImGui::CheckboxFlags("Debug", &m_LogFlags, (u32)LOG_FLAGS::LOG_DEBUG);
		ImGui::SameLine(); ImGui::CheckboxFlags("Info", &m_LogFlags, (u32)LOG_FLAGS::LOG_INFO);
		ImGui::Text("           "); 
		ImGui::SameLine(); ImGui::CheckboxFlags("Warn", &m_LogFlags, (u32)LOG_FLAGS::LOG_WARN);
		ImGui::SameLine(); ImGui::CheckboxFlags("Error", &m_LogFlags, (u32)LOG_FLAGS::LOG_ERROR);
		ImGui::SameLine(); ImGui::CheckboxFlags("Critical", &m_LogFlags, (u32)LOG_FLAGS::LOG_CRITICAL);
		ImGui::Text("           ");
		//ImGui::SameLine(); 
		//if (ImGui::SmallButton("Clear"))
		//{
		//	ALEngine::Exceptions::Logger::GetCoreLogger()->flush();
		//}

		// The Text Box
		ImGui::BeginChild("LoggerTextBox", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

		// Change OSS to ISS
		std::string log_line;
		std::istringstream iss(m_LoggerOSS.str());

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
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_ERROR))
					continue;
				to_print = true;
				text_clr = IM_COL32(255, 128, 0, 255);
				break;
			case 'd':	// Debug
			case 'D':
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_DEBUG))
					continue;
				to_print = true;
				text_clr = IM_COL32(0, 128, 255, 255);
				break;
			case 't':	// Trace
			case 'T':
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_TRACE))
					continue;
				to_print = true;
				text_clr = IM_COL32(255, 255, 255, 255);
				break;
			case 'w':	// Warning
			case 'W':
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_WARN))
					continue;
				to_print = true;
				text_clr = IM_COL32(255, 255, 0, 255);
				break;
			case 'i':	// Info
			case 'I':
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_INFO))
					continue;
				to_print = true;
				text_clr = IM_COL32(0, 255, 0, 255);
				break;
			case 'c':	// Critical
			case 'C':
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_CRITICAL))
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
			ImGui::SetScrollHereY();
		ImGui::EndChild();
		//End ImGui Window
		ImGui::End();
	}
	
	void LoggerPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void LoggerPanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}

}

#endif