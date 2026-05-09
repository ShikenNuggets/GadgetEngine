#pragma once

#include <chrono>

#include <GCore/Timer.hpp>

#include "Debug.h"

namespace Gadget
{
	class Time
	{
	public:
		Time();
		DISABLE_COPY_AND_MOVE(Time);

		void Start();
		void Update();
		void Delay();

		double DeltaTime() const;
		double RealDeltaTime() const;
		double PureDeltaTime() const;

		double TimeSinceStartup() const;

		double GetTimeScale() const{ return timeScale; }

		void SetTimeScale(float timeScale_)
		{
			timeScale = timeScale_;
			if (timeScale < 0.0f)
			{
				GADGET_LOG_WARNING(SID("TIME"), "Negative time scale is not guaranteed to be well-supported!");
			}
		}

		static constexpr float MsToSecondsFloat(const std::chrono::milliseconds& ms_)
		{
			return static_cast<float>(ms_.count()) / 1000.0f;
		}

	private:
		Timer timer;
		float timeScale;

		std::chrono::microseconds GetSleepTime() const;
	};
}
