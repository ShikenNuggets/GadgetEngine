#include "Profiler.h"

using namespace Gadget;

//----------------------------------------------------------------------------------------------------//
//---------- Timer -----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

Timer::Timer() : startTime(0), total(0.0), numIterations(0){};

void Timer::StartProfiling(){
	if(startTime.count() != 0){
		Debug::Log(SID("UTILS"), "Tried to restart profiling, but it was never stopped!", Debug::Warning, __FILE__, __LINE__);
		return;
	}

	startTime = Timing::CurrentTime();
}

double Timer::EndProfiling(){
	if(startTime.count() == 0){
		Debug::Log(SID("UTILS"), "Tried to end profiling, but it never started!", Debug::Warning, __FILE__, __LINE__);
		return 0.0;
	}

	const double durationSeconds = Timing::TimeSince(startTime);
	total += durationSeconds;
	numIterations++;

	startTime = std::chrono::milliseconds(0);

	return durationSeconds;
}

double Timer::Average() const{
	if(numIterations == 0){
		return 0.0;
	}

	return Total() / static_cast<double>(numIterations);
}

//----------------------------------------------------------------------------------------------------//
//---------- Profiler --------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//

std::map<StringID, Timer> Profiler::profiles;

void Profiler::Start(StringID name_){
	if(!Utils::ContainsKey(profiles, name_)){
		profiles.emplace(name_, Timer());
	}

	profiles[name_].StartProfiling();
}

double Profiler::End(StringID name_){
	if(!Utils::ContainsKey(profiles, name_)){
		Debug::Log(SID("UTILS"), "Tried to end profiling on [" + name_.GetString() + "], but it was never started!");
		return 0.0;
	}

	return profiles[name_].EndProfiling();
}

double Profiler::TotalTime(StringID name_){
	if(Utils::ContainsKey(profiles, name_)){
		return profiles[name_].Total();
	}

	return 0.0;
}

double Profiler::GetAverage(StringID name_){
	if(Utils::ContainsKey(profiles, name_)){
		return profiles[name_].Average();
	}

	return 0.0;
}

void Profiler::OutputAllAverageResults(){
	for(auto& profile : profiles){
		Debug::Log("Average " + profile.first.GetString() + " Duration: " + std::to_string(profile.second.Average() * 1000.0) + "ms", Debug::Info);
	}
}