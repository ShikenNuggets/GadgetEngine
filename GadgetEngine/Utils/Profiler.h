#ifndef GADGET_PROFILER_H
#define GADGET_PROFILER_H

#include <chrono>

#include "Debug.h"
#include "Utils/StringID.h"
#include "Utils/Utils.h"

namespace Gadget{
	namespace Timing{
		inline std::chrono::milliseconds CurrentTime(){
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		}

		inline double TimeSince(std::chrono::milliseconds previousTime_){
			auto duration = CurrentTime() - previousTime_;
			return static_cast<double>(duration.count()) / 1000.0f;
		}
	}

	class Timer{
	public:
		Timer();
		
		void StartProfiling();
		double EndProfiling();

		inline constexpr double Total() const{ return total; }
		double Average() const;
		
	private:
		std::chrono::milliseconds startTime;
		double total;
		uint64_t numIterations;
	};

	class Profiler{
	public:
		STATIC_CLASS(Profiler);

		static void Start(StringID name_);
		static double End(StringID name_);

		static double TotalTime(StringID name_);
		static double GetAverage(StringID name_);

		static void OutputAllAverageResults();

	private:
		static std::map<StringID, Timer> profiles;
	};
}

#endif //!GADGET_PROFILER_H