#include "GlobalAllocator.h"

#include <new>
#include <iostream>

#include "Utils/Profiler.h"

using namespace Gadget;

std::atomic<uint64_t> GlobalAllocator::numAllocs = 0;
std::atomic<uint64_t> GlobalAllocator::numFrees = 0;
std::atomic<double> GlobalAllocator::allocTime = 0.0;
std::atomic<double> GlobalAllocator::freeTime = 0.0;

static StringID MemAllocSID = SID("MemAlloc");
static StringID MemFreeSID = SID("MemFree");

#pragma warning(disable : 28251)

void* operator new(size_t size_) noexcept(false){
	auto startTime = Timing::CurrentTime();

	void* ptr = malloc(size_);
	if(ptr == nullptr){
		throw std::bad_alloc();
	}

	GlobalAllocator::AddToAllocCount();
	GlobalAllocator::AddAllocTime(Timing::TimeSince(startTime));

	return ptr;
}

#pragma warning(default : 28251)

void operator delete(void* p_) noexcept{
	auto startTime = Timing::CurrentTime();

	free(p_);
	
	GlobalAllocator::AddToFreeCount();
	GlobalAllocator::AddFreeTime(Timing::TimeSince(startTime));
}