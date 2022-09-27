#include "pch.h"

namespace ALEngine::Utility
{
	std::vector<TimerData> Profiler::m_TimerDataList;

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