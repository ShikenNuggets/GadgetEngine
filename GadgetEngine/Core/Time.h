#ifndef GADGET_TIME_H
#define GADGET_TIME_H

#include <chrono>

#include "Utils/Utils.h"

namespace Gadget{
	class Time{
	public:
		Time();
		DISABLE_COPY_AND_MOVE(Time);

		void Start();
		void Update();
		void Delay();

		float DeltaTime() const;
		float RealDeltaTime() const;
		float PureDeltaTime() const;

		float TimeSinceStartup() const;

		static constexpr float MsToSecondsFloat(const std::chrono::milliseconds& ms_){
			return static_cast<float>(ms_.count()) / 1000.0f;
		}

	private:
		float timeScale;

		std::chrono::milliseconds startTime;
		std::chrono::milliseconds previousTicks;
		std::chrono::milliseconds currentTicks;

		std::chrono::milliseconds GetSleepTime() const;
	};
}

#endif //!GADGET_TIME_H