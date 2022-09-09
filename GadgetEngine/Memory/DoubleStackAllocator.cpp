#include "DoubleStackAllocator.h"

#include "Debug.h"

using namespace Gadget;

DoubleStackAllocator::DoubleStackAllocator(size_t bytes_) : size(bytes_), base(nullptr), lowerMarkers(), upperMarkers(){
	GADGET_ASSERT(bytes_ != 0, "Tried to create a 0 byte allocator!");

	base = malloc(bytes_);
	GADGET_BASIC_ASSERT(base != nullptr); //TODO - Actually handle this, somehow...

	#ifdef GADGET_DEBUG
		//Fill the memory with garbage so we can detect memory corruption more easily
		//0xCD is "clean" memory in Visual C++
		memset(base, 0xCD, size);
	#endif //GADGET_DEBUG
}

DoubleStackAllocator::~DoubleStackAllocator(){
	free(base);
}

DoubleStackAllocator::LowerMarker DoubleStackAllocator::GetLowerMarker() const{
	if(lowerMarkers.empty()){
		return 0;
	}

	return lowerMarkers.top();
}

DoubleStackAllocator::UpperMarker DoubleStackAllocator::GetUpperMarker() const{
	if(upperMarkers.empty()){
		return size;
	}

	return upperMarkers.top();
}

bool DoubleStackAllocator::CanAllocate(size_t bytesToAllocate_) const{
	return GetLowerMarker() + bytesToAllocate_ < GetUpperMarker();
}

void* DoubleStackAllocator::AllocateLower(size_t bytes_){
	GADGET_ASSERT(bytes_ > 0, "Tried to allocate 0 bytes!");
	GADGET_ASSERT(CanAllocate(bytes_), "Could not allocate " + std::to_string(bytes_) + " bytes!");

	void* ptr = GetLowerPtr();

	#ifndef GADGET_DEBUG
		//Zero initialize the newly allocated memory to prevent issues with uninitialized values
		//Don't do this on Debug builds so we can actually catch those issues
		//This could be removed to improve performance, so don't depend on it to save you
		memset(GetLowerPtr(), 0, bytes_);
	#endif //!GADGET_DEBUG

	lowerMarkers.push(GetLowerMarker() + bytes_);

	return ptr;
}

void* DoubleStackAllocator::AllocateUpper(size_t bytes_){
	GADGET_ASSERT(bytes_ > 0, "Tried to allocate 0 bytes!");
	GADGET_ASSERT(CanAllocate(bytes_), "Could not allocate " + std::to_string(bytes_) + " bytes!");

	upperMarkers.push(GetUpperMarker() - bytes_);

	#ifndef GADGET_DEBUG
		//Zero initialize the newly allocated memory to prevent issues with uninitialized values
		//Don't do this on Debug builds so we can actually catch those issues
		//This could be removed to improve performance, so don't depend on it to save you
		memset(GetUpperPtr(), 0, bytes_);
	#endif //!GADGET_DEBUG

	return GetUpperPtr();
}

void DoubleStackAllocator::FreeLower(LowerMarker marker_){
	GADGET_ASSERT(!lowerMarkers.empty() || marker_ <= GetLowerMarker(), "Tried to free unallocated memory!");
	GADGET_ASSERT(marker_ < size, "Tried to free memory outside of the stack!");

	while(GetLowerMarker() > marker_){
		#ifdef GADGET_DEBUG
			LowerMarker oldMarker = GetLowerMarker();
		#endif //GADGET_DEBUG

		lowerMarkers.pop();

		#ifdef GADGET_DEBUG
			//Fill the unused memory with garbage so we can detect memory corruption more easily
			//0xDD is "dead" memory in Visual C++
			memset(GetLowerPtr(), 0xDD, oldMarker - lowerMarkers.top());
		#endif //GADGET_DEBUG
	}

	GADGET_ASSERT(marker_ == GetLowerMarker(), "Memory was freed with an invalid marker!");
}

void DoubleStackAllocator::FreeUpper(UpperMarker marker_){
	GADGET_ASSERT(!upperMarkers.empty() || marker_ >= GetLowerMarker(), "Tried to free unallocated memory!");
	GADGET_ASSERT(marker_ < size, "Tried to free memory outside of the stack!");

	while(GetUpperMarker() < marker_){
		#ifdef GADGET_DEBUG
			UpperMarker oldMarker = GetUpperMarker();
		#endif //GADGET_DEBUG

		upperMarkers.pop();

		#ifdef GADGET_DEBUG
			//Fill the unused memory with garbage so we can detect memory corruption more easily
			//0xDD is "dead" memory in Visual C++
			memset(GetUpperMarkerPtr(oldMarker), 0xDD, upperMarkers.top() - oldMarker);
		#endif //GADGET_DEBUG
	}

	GADGET_ASSERT(marker_ == GetUpperMarker(), "Memory was freed with an invalid marker!");
}

void DoubleStackAllocator::Clear(){
	while(!lowerMarkers.empty()){
		lowerMarkers.pop();
	}

	while(!upperMarkers.empty()){
		upperMarkers.pop();
	}

	#ifdef GADGET_DEBUG
		//Fill the unused memory with garbage so we can detect memory corruption more easily
		//0xDD is "dead" memory in Visual C++
		memset(base, 0xDD, size);
	#endif //GADGET_DEBUG
}

void* DoubleStackAllocator::GetLowerPtr() const{
	return (void*)((size_t)base + GetLowerMarker());
}

void* DoubleStackAllocator::GetUpperPtr() const{
	return (void*)((size_t)base + GetUpperMarker());
}

void* DoubleStackAllocator::GetLowerMarkerPtr(LowerMarker marker_) const{
	GADGET_ASSERT(!lowerMarkers.empty() || marker_ <= GetLowerMarker(), "Tried to get unallocated memory!");
	GADGET_ASSERT(marker_ < size, "Tried to get memory outside of the stack!");

	if(marker_ <= GetLowerMarker()){
		return base;
	}

	return (void*)((size_t)base + marker_);
}

void* DoubleStackAllocator::GetUpperMarkerPtr(UpperMarker marker_) const{
	GADGET_ASSERT(upperMarkers.empty() || marker_ >= GetUpperMarker(), "Tried to get unallocated memory!");
	GADGET_ASSERT(marker_ < size, "Tried to get memory outside of the stack!");

	if(marker_ >= GetUpperMarker()){
		return (void*)((size_t)base + size);
	}

	return (void*)((size_t)base + marker_);
}