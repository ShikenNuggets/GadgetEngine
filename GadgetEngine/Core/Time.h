#ifndef GADGET_TIME_H
#define GADGET_TIME_H

#include <chrono>

namespace Gadget{
	class Time{
	public:
		static Time* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

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
		static Time* instance;

		Time();

		float timeScale;

		std::chrono::milliseconds startTime;
		std::chrono::milliseconds previousTicks;
		std::chrono::milliseconds currentTicks;

		std::chrono::milliseconds GetSleepTime() const;

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		Time(const Time&) = delete;
		Time(Time&&) = delete;
		Time& operator=(const Time&) = delete;
		Time& operator=(Time&&) = delete;
	};
}

#endif //!GADGET_TIME_H