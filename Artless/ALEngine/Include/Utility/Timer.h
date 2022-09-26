#ifndef TIMER_H
#define TIMER_H

namespace ALEngine::Utility
{
	class Timer
	{
	public:
		Timer(const c8* _name);
		~Timer();

		void StopTimer();

	private:
		const c8* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
		b8 m_Stopped;
	};
}

#endif