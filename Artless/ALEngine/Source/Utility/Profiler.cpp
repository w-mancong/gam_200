/*!
file:	Profiler.cpp
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function definitions for the Profiler class.
		The Profiler keeps track of the engine's performance data.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Utility
{
	// Declaration of TimerData list
	std::vector<TimerData> Profiler::m_TimerDataList;

	const std::vector<TimerData>& Profiler::GetTimerDataList(void)
	{
		return m_TimerDataList;
	}

	void Profiler::AddTimerData(TimerData data)
	{
		for (auto &td : m_TimerDataList)
		{
			if (td.m_Name == data.m_Name)
			{
				td.m_Time += data.m_Time;
				return;
			}
		}

		m_TimerDataList.push_back(data);
	}

	void Profiler::ClearTimerData(void)
	{
		m_TimerDataList.clear();
	}
}