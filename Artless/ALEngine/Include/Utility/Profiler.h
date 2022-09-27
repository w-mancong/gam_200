#ifndef PROFILER_H
#define PROFILER_H

namespace ALEngine::Utility
{
	struct TimerData
	{
		std::string m_Name;
		f32 m_Time;
	};

	class Profiler
	{
	public:
		static const std::vector<TimerData> &GetTimerDataList(void) { return m_TimerDataList; }
		static void AddTimerData(TimerData data);
		static void ClearTimerData(void);

	private:
		Profiler(void) {};
		~Profiler(void) = default;

		static std::vector<TimerData> m_TimerDataList;
	};
}

#define PROFILER_TIMER(name) Timer ProfilerTimer##__LINE__(name)

#endif