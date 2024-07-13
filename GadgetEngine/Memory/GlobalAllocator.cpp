#include "GlobalAllocator.h"

#include <new>
#include <iostream>

#include "Utils/Profiler.h"

using namespace Gadget;

#ifdef GADGET_DEBUG
std::atomic<uint64_t> GlobalAllocator::numAllocs = 0;
std::atomic<uint64_t> GlobalAllocator::numFrees = 0;
std::atomic<double> GlobalAllocator::allocTime = 0.0;
std::atomic<double> GlobalAllocator::freeTime = 0.0;
#endif //GADGET_DEBUG

static StringID MemAllocSID = SID("MemAlloc");
static StringID MemFreeSID = SID("MemFree");

#pragma warning(disable : 28251)

void* operator new(size_t size_) noexcept(false){
#ifdef GADGET_DEBUG
	const auto startTime = Timing::CurrentTime();
#endif //GADGET_DEBUG

	void* ptr = malloc(size_);
	if(ptr == nullptr){
		throw std::bad_alloc();
	}

#ifdef GADGET_DEBUG
	GlobalAllocator::AddToAllocCount();
	GlobalAllocator::AddAllocTime(Timing::TimeSince(startTime));
#endif //GADGET_DEBUG

	return ptr;
}

#pragma warning(default : 28251)

void operator delete(void* p_) noexcept{
#ifdef GADGET_DEBUG
	const auto startTime = Timing::CurrentTime();
#endif //GADGET_DEBUG

	free(p_);
	
#ifdef GADGET_DEBUG
	GlobalAllocator::AddToFreeCount();
	GlobalAllocator::AddFreeTime(Timing::TimeSince(startTime));
#endif //GADGET_DEBUG
}