#include "StackAllocator.h"

#include "Debug.h"

using namespace Gadget;

StackAllocator::StackAllocator(size_t bytes_) : size(bytes_), base(nullptr), markers(){
	GADGET_ASSERT(bytes_ != 0, "Tried to create a 0 byte allocator!");

	base = malloc(bytes_);
	GADGET_BASIC_ASSERT(base != nullptr); //TODO - Actually handle this, somehow...

	#ifdef GADGET_DEBUG
		//Fill the memory with garbage so we can detect memory corruption more easily
		//0xCD is "clean" memory in Visual C++
		memset(GetTopPtr(), 0xCD, size);
	#endif //GADGET_DEBUG
}

StackAllocator::~StackAllocator(){
	free(base);
}

StackAllocator::Marker StackAllocator::GetMarker() const{
	if(markers.empty()){
		return 0;
	}

	return markers.top();
}

bool StackAllocator::CanAllocate(size_t bytesToAllocate_) const{
	return GetMarker() + bytesToAllocate_ < size;
}

void* StackAllocator::Allocate(size_t bytes_){
	GADGET_ASSERT(bytes_ > 0, "Tried to allocate 0 bytes!");
	GADGET_ASSERT(CanAllocate(bytes_), "Could not allocate " + std::to_string(bytes_) + " bytes!");

	void* ptr = GetTopPtr();

	#ifndef GADGET_DEBUG
		//Zero initialize the newly allocated memory to prevent issues with uninitialized values
		//Don't do this on Debug builds so we can actually catch those issues
		//This could be removed to improve performance, so don't depend on it to save you
		memset(GetTopPtr(), 0, bytes_);
	#endif //!GADGET_DEBUG

	markers.push(GetMarker() + bytes_);

	return ptr;
}

void StackAllocator::FreeToMarker(Marker marker_){
	GADGET_ASSERT(!markers.empty() || marker_ <= GetMarker(), "Tried to free unallocated memory!");
	GADGET_ASSERT(marker_ < size, "Tried to free memory outside of the stack!");

	while(GetMarker() > marker_){
		#ifdef GADGET_DEBUG
			Marker oldMarker = GetMarker();
		#endif //GADGET_DEBUG

		markers.pop();

		#ifdef GADGET_DEBUG
			//Fill the unused memory with garbage so we can detect memory corruption more easily
			memset(GetTopPtr(), 0xDD, oldMarker - markers.top());
		#endif //GADGET_DEBUG
	}

	GADGET_ASSERT(marker_ == GetMarker(), "Memory freed with an invalid marker!");
}

void StackAllocator::Clear(){
	while(!markers.empty()){
		markers.pop();
	}

	#ifdef GADGET_DEBUG
		//Fill the unused memory with garbage so we can detect memory corruption more easily
		memset(base, 0xDD, size);
	#endif //GADGET_DEBUG
}

void* StackAllocator::GetTopPtr() const{
	return (void*)((size_t)base + GetMarker());
}

void* StackAllocator::GetMarkerPtr(Marker marker_) const{
	GADGET_ASSERT(!markers.empty() || marker_ <= GetMarker(), "Tried to get unallocated memory!");
	GADGET_ASSERT(marker_ < size, "Tried to get memory outside of the stack!");

	if(marker_ <= GetMarker()){
		return base;
	}

	return (void*)((size_t)base + marker_);
}