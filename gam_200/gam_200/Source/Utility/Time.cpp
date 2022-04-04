#include "pch.h"

namespace
{
	f64 const NANO_TO_SECONDS = 1'000'000'000.0;
	u64 const MAX_SAMPLES = 100;

	u64 tickIndex = 0;
	float tickSum = 0;
	float tickList[MAX_SAMPLES];

	/* need to zero out the ticklist array before starting */
	/* average will ramp up until the buffer is full */
	/* returns average ticks per frame over the MAXSAMPLES last frames */
	float CalcAverageTick(float newTick)
	{
		tickSum -= tickList[tickIndex];  /* subtract value falling off */
		tickSum += newTick;              /* add new value */
		tickList[tickIndex] = newTick;   /* save new value so it can be subtracted later */
		(++tickIndex) %= MAX_SAMPLES;
		/* return average */
		return(tickSum / MAX_SAMPLES);
	}
}

namespace ManCong
{
	namespace Utility
	{
		f32 Time::dt = 0.0f, Time::fps = 0.0f, Time::fixedTime = 0.0f;
		std::chrono::steady_clock::time_point Time::oldTime{};

		void Time::Init(void)
		{
			oldTime = std::chrono::high_resolution_clock::now();
		}

		void Time::Update(void)
		{
			auto newTime = std::chrono::high_resolution_clock::now();
			dt = static_cast<f32>((newTime - oldTime).count()) / static_cast<f32>(NANO_TO_SECONDS);
			fps = CalcAverageTick(1.0f / dt);
			oldTime = newTime;
		}
	}
}