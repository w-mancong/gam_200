#ifndef	TIME_H
#define TIME_H

namespace ManCong
{
	namespace Utility
	{
		class Time
		{
		public:
			static void Init(void);
			static void Update(void);

			static f32 dt, fps, fixedTime;
		private:
			Time(void)	= default;
			~Time(void) = default;

			static std::chrono::steady_clock::time_point oldTime;
		};
	}
}

#endif