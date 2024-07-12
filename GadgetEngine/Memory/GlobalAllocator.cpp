#include "GlobalAllocator.h"

#include <new>
#include <iostream>

using namespace Gadget;

std::atomic<uint64_t> GlobalAllocator::numAllocs = 0;
std::atomic<uint64_t> GlobalAllocator::numFrees = 0;

#pragma warning(disable : 28251)

void* operator new(size_t size_) noexcept(false){
	void* ptr = malloc(size_);
	if(ptr == nullptr){
		throw std::bad_alloc();
	}

	GlobalAllocator::numAllocs++;
	return ptr;
}

#pragma warning(default : 28251)

void operator delete(void* p_) noexcept{
	GlobalAllocator::numFrees++;
	free(p_);
}