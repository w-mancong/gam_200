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

#if _EDITOR

namespace ALEngine::Editor
{
	LoggerPanel::LoggerPanel()
	{
		// Create spdlog sink to ostream to print log on imgui
		auto my_sink = std::make_shared<my_sink_mt>();
		my_sink->m_Logger = this; 
		my_sink->set_pattern("%^[%n] [%l] %v%$");
		ALEngine::Exceptions::Logger::GetCoreLogger()->sinks().push_back(my_sink);

		AL_CORE_WARN("Hello");

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

		// Check each line for 
		for(const auto& a : m_MessageList)
		{
			// Text Color
			ImU32 text_clr = IM_COL32_BLACK;

			// Bool to print text
			b8 to_print{ false };

			// Get Log Level
			switch (static_cast<spdlog::level::level_enum>(a.second))
			{
			case spdlog::level::level_enum::err:
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_ERROR))
					continue;
				to_print = true;
				text_clr = IM_COL32(255, 128, 0, 255);
				break;
			case spdlog::level::level_enum::debug:
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_DEBUG))
					continue;
				to_print = true;
				text_clr = IM_COL32(0, 128, 255, 255);
				break;
			case spdlog::level::level_enum::trace:
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_TRACE))
					continue;
				to_print = true;
				text_clr = IM_COL32(255, 255, 255, 255);
				break;
			case spdlog::level::level_enum::warn:
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_WARN))
					continue;
				to_print = true;
				text_clr = IM_COL32(255, 255, 0, 255);
				break;
			case spdlog::level::level_enum::info:
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_INFO))
					continue;
				to_print = true;
				text_clr = IM_COL32(0, 255, 0, 255);
				break;
			case spdlog::level::level_enum::critical:
				if (!(m_LogFlags & (u32)LOG_FLAGS::LOG_CRITICAL))
					continue;
				to_print = true;
				text_clr = IM_COL32(255, 0, 0, 255);
				break;
			}

			if (to_print)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, text_clr);
				ImGui::TextUnformatted(a.first.c_str());
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