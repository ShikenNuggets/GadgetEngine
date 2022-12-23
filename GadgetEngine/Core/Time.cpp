#include "Time.h"

#include "Config.h"
#include "Math/Math.h"

using namespace Gadget;

Time* Time::instance = nullptr;

Time::Time() : timeScale(1.0f), startTime(0), previousTicks(0), currentTicks(0){}

Time* Time::GetInstance(){
	if(instance == nullptr){
		instance = new Time();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void Time::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

void Time::Start(){
	startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
	previousTicks = startTime;
	currentTicks = startTime;
}

void Time::Update(){
	previousTicks = currentTicks;
	currentTicks = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}

void Time::Delay(){
	auto sleepTime = GetSleepTime();

	if(sleepTime.count() > 0){
		std::this_thread::sleep_for(sleepTime);
	}
}

std::chrono::milliseconds Time::GetSleepTime() const{
	//If the framerate is 0, this is treated as an unlimited framerate
	//This also prevents a division by 0 later
	int targetFPS = static_cast<int>(Config::GetInstance()->GetOptionFloat(EngineVars::Display::targetFPSKey));
	if(targetFPS == 0){
		return std::chrono::milliseconds(0);
	}

	auto msPerFrame = std::chrono::milliseconds(1000 / targetFPS);

	if(msPerFrame.count() == 0){
		return std::chrono::milliseconds(0);
	}

	auto updatedTicks = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());

	auto sleepTime = msPerFrame - (updatedTicks - currentTicks);
	if(sleepTime > msPerFrame){
		return msPerFrame;
	}

	return sleepTime;
}

float Time::DeltaTime() const{
	return RealDeltaTime() * timeScale;
}

float Time::RealDeltaTime() const{
	//If the actual change in time is more than a tenth of a second, then just use 1/10 as the deltaTime
	//This is to prevent time-related glitches in the event of hitting a breakpoint, a window event, a lag spike, etc
	//The actual value here is somewhat arbitrary, but to maximize its effect it should be somewhere between 1/5 and 1/20
	//If it's too low it'll affect normal framerates, and if it's too high it won't be effective enough

	//We also don't want the delta time to be negative for various reasons
	return Math::Clamp(0.0f, 0.1f, PureDeltaTime());
}

float Time::PureDeltaTime() const{
	return MsToSecondsFloat(currentTicks - previousTicks);
}

float Time::TimeSinceStartup() const{
	return MsToSecondsFloat(currentTicks - startTime);
}