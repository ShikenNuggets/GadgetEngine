#ifndef GADGET_PROFILER_H
#define GADGET_PROFILER_H

#include <chrono>

#include "Debug.h"
#include "StringID.h"
#include "Utils.h"

namespace Gadget{
	class Timer{
	public:
		Timer() : startTime(0), times(){}
		
		void StartProfiling(){
			if(startTime.count() != 0){
				Debug::Log(SID("UTILS"), "Tried to restart profiling, but it was never stopped!", Debug::Warning, __FILE__, __LINE__);
				return;
			}

			startTime = CurrentTime();
		}

		double EndProfiling(){
			if(startTime.count() == 0){
				Debug::Log(SID("UTILS"), "Tried to end profiling, but it never started!", Debug::Warning, __FILE__, __LINE__);
				return;
			}

			auto duration = CurrentTime() - startTime;
			double durationSeconds = static_cast<double>(duration.count()) / 1000.0f;
			times.push_back(durationSeconds);

			startTime = std::chrono::milliseconds(0);

			return durationSeconds;
		}

		double Total(){
			double total = 0.0;
			for(const auto& t : times){
				total += t;
			}

			return total;
		}

		double Average(){
			if(times.size() == 0){
				return 0.0;
			}

			return Total() / static_cast<double>(times.size());
		}
		
	private:
		constexpr std::chrono::milliseconds CurrentTime(){
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		}

		std::chrono::milliseconds startTime;
		std::vector<double> times;
	};

	class Profiler{
	public:
		DISABLE_COPY_AND_MOVE(Profiler)
		Profiler() = delete;

		static void Start(StringID name_){
			if(Utils::ContainsKey(profiles, name_)){
				profiles[name_].StartProfiling();
			}else{
				profiles.emplace(name_, Timer());
			}
		}
		
		static double End(StringID name_){
			if(!Utils::ContainsKey(profiles, name_)){
				Debug::Log(SID("UTILS"), "Tried to end profiling on [" + name_.GetString() + "], but it was never started!");
				return 0.0;
			}

			return profiles[name_].EndProfiling();
		}

		static double TotalTime(StringID name_){
			if(Utils::ContainsKey(profiles, name_)){
				return profiles[name_].Total();
			}

			return 0.0;
		}
		
		static double GetAverage(StringID name_){
			if(Utils::ContainsKey(profiles, name_)){
				return profiles[name_].Average();
			}

			return 0.0;
		}

	private:
		static std::map<StringID, Timer> profiles;
	};
}

#endif //!GADGET_PROFILER_H