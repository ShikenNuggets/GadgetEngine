#ifndef GADGET_MEMORY_GLOBAL_ALLOCATOR_H
#define GADGET_MEMORY_GLOBAL_ALLOCATOR_H

#include <atomic>

namespace Gadget{
	class GlobalAllocator{
	#ifdef GADGET_DEBUG
	public:
		//DO NOT call these directly
		static void AddToAllocCount(){ numAllocs++; }
		static void AddToFreeCount(){ numFrees++; }
		static void AddAllocTime(double time_){ allocTime += time_; }
		static void AddFreeTime(double time_){ freeTime += time_; }

		static uint64_t GetNumAllocs(){ return numAllocs; }
		static uint64_t GetNumFrees(){ return numFrees; }
		static double GetAllocTime(){ return allocTime; }
		static double GetFreeTime(){ return freeTime; }

	private:
		static std::atomic<uint64_t> numAllocs;
		static std::atomic<uint64_t> numFrees;

		static std::atomic<double> allocTime;
		static std::atomic<double> freeTime;
	#endif //GADGET_DEBUG
	};
}

#endif //!GADGET_MEMORY_GLOBAL_ALLOCATOR_H