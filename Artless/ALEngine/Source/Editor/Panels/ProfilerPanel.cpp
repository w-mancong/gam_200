/*!
file:	ProfilerPanel.cpp
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function definitions for the ProfilerPanel class.
		The ProfilerPanel class contains information and functions necessary for
		the Profiler Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Editor
{
#if 0
	void ProfilerPanel::OnImGuiRender(void)
	{
		// Counter for 
		static f32 counter{ 0.f }, total_time{ 0.f };
		counter += Time::m_DeltaTime;

		if (counter >= 1.f)
		{
			m_TempTimerDataList.clear();
			total_time = 0.f;
			for (auto it : Utility::Profiler::GetTimerDataList())
			{
				m_TempTimerDataList.push_back(it);
				total_time += it.m_Time;
			}
			counter = 0.f;

			Utility::Profiler::ClearTimerData();
		}

		if (m_TempTimerDataList.empty())
			return;

		// Set size constraints of inspector
		ImGui::SetNextWindowSizeConstraints(PANEL_MIN, PANEL_MAX);

		// Begin ImGui
		if (!ImGui::Begin("Profiler"))
		{
			ImGui::End();
			return;
		}

		if (total_time > 0.f)
		{
			const c8* head = "Time Taken (Per Second)";
			ImVec2 textSize = ImGui::CalcTextSize(head);
			f32 winWidth = ImGui::GetWindowWidth();

			ImGui::SameLine((winWidth - textSize.x) * 0.5f);
			ImGui::Text(head);
			for (auto i : m_TempTimerDataList)
			{
				f32 percent = i.m_Time / total_time * 100.f;
				std::string timer_data = i.m_Name + ": %.3fms (%.2f%%)";
				ImGui::Text(timer_data.c_str(), i.m_Time, percent);
			}
		}

		ImGui::End();
	}
#endif
}