#include "Core/Time.h"

#include <thread>

#include <GCore/Math/Math.hpp>

#include "App.h"
#include "Config.h"

using namespace Gadget;

static constexpr int gMsInSecond = 1000;
static constexpr double gMinRealDeltaTime = 0.1;

Time::Time() : timeScale(1.0f){}

void Time::Start()
{
	timer.Reset();
}

void Time::Update()
{
	timer.Update();
}

void Time::Delay(){
	const auto sleepTime = GetSleepTime();

	if (sleepTime.count() > 0)
	{
		std::this_thread::sleep_for(sleepTime);
	}
}

std::chrono::microseconds Time::GetSleepTime() const
{
	// If the framerate is 0, this is treated as an unlimited framerate
	// This also prevents a division by 0 later
	const int targetFPS = static_cast<int>(App::GetConfig().GetOptionFloat(EngineVars::Display::targetFPSKey));
	if (targetFPS <= 0)
	{
		return std::chrono::microseconds(0);
	}

	const auto microsecondsPerFrame = std::chrono::microseconds(1'000'000 / targetFPS);
	if (microsecondsPerFrame.count() == 0)
	{
		return std::chrono::microseconds(0);
	}

	const auto durationSeconds = std::chrono::duration<double>(timer.SecondsSinceLastUpdate());
	const auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(durationSeconds);

	auto sleepTime = microsecondsPerFrame - durationMicroseconds;
	if (sleepTime > microsecondsPerFrame)
	{
		return microsecondsPerFrame;
	}

	return sleepTime;
}

double Time::DeltaTime() const
{
	return RealDeltaTime() * timeScale;
}

double Time::RealDeltaTime() const
{
	//If the actual change in time is more than a tenth of a second, then just use 1/10 as the deltaTime
	//This is to prevent time-related glitches in the event of hitting a breakpoint, a window event, a lag spike, etc
	//The actual value here is somewhat arbitrary, but to maximize its effect it should be somewhere between 1/5 and 1/20
	//If it's too low it'll affect normal framerates, and if it's too high it won't be effective enough

	//We also don't want the delta time to be negative for various reasons
	return Math::Clamp(0.0, gMinRealDeltaTime, PureDeltaTime());
}

double Time::PureDeltaTime() const
{
	return timer.DeltaSeconds();
}

double Time::TimeSinceStartup() const
{
	return timer.SecondsSinceStart();
}
