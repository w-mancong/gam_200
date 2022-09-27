#ifndef PROFILER_PANEL_H
#define PROFILER_PANEL_H

namespace ALEngine::Editor
{
	class ProfilerPanel
	{
	public:
		ProfilerPanel(void) {};
		~ProfilerPanel(void) {};

		void OnImGuiRender(void);

	private:
		// Panel size
		const ImVec2 PANEL_MIN{ 320, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };

		std::vector<Utility::TimerData> m_TempTimerDataList;
	};
}

#endif