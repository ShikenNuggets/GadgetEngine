#ifndef GADGET_PROFILER_H
#define GADGET_PROFILER_H

#include <chrono>

#include "Debug.h"
#include "Utils/StringID.h"
#include "Utils/Utils.h"

namespace Gadget{
	class Timer{
	public:
		Timer();
		
		void StartProfiling();
		double EndProfiling();

		double Total() const;
		double Average() const;
		
	private:
		constexpr inline std::chrono::milliseconds CurrentTime() const{
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		}

		std::chrono::milliseconds startTime;
		std::vector<double> times;
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