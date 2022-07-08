#include "DoubleStackAllocator.h"

using namespace Gadget;

DoubleStackAllocator::DoubleStackAllocator(size_t bytes_) : size(bytes_), base(nullptr), lowerMarkers(), upperMarkers(){
	_ASSERT(bytes_ != 0); //Why the hell are you trying to allocate 0 bytes?

	base = malloc(bytes_);
	_ASSERT(base != nullptr); //TODO - Actually handle this, somehow...

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
	_ASSERT(bytes_ > 0); //Why the hell are you trying to allocate 0 bytes?
	_ASSERT(CanAllocate(bytes_)); //Tried to allocate too much memory

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
	_ASSERT(bytes_ > 0); //Why the hell are you trying to allocate 0 bytes?
	_ASSERT(CanAllocate(bytes_)); //Tried to allocate too much memory

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
	_ASSERT(!lowerMarkers.empty()); //Tried to free unallocated memory
	_ASSERT(marker_ <= GetLowerMarker()); //Tried to free unallocated memory
	_ASSERT(marker_ < size); //Tried to free memory outside of the stack

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

	_ASSERT(marker_ == GetLowerMarker()); //Freed with invalid marker
}

void DoubleStackAllocator::FreeUpper(UpperMarker marker_){
	_ASSERT(!upperMarkers.empty()); //Tried to free unallocated memory
	_ASSERT(marker_ >= GetLowerMarker()); //Tried to free unallocated memory
	_ASSERT(marker_ < size); //Tried to free memory outside of the stack

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

	_ASSERT(marker_ == GetUpperMarker()); //Freed with invalid marker
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
	_ASSERT(!lowerMarkers.empty()); //Tried to get unallocated memory
	_ASSERT(marker_ <= GetLowerMarker()); //Tried to get unallocated memory
	_ASSERT(marker_ < size); //Tried to get memory outside of the stack

	if(marker_ <= GetLowerMarker()){
		return base;
	}

	return (void*)((size_t)base + marker_);
}

void* DoubleStackAllocator::GetUpperMarkerPtr(UpperMarker marker_) const{
	_ASSERT(upperMarkers.empty()); //Tried to get unallocated memory
	_ASSERT(marker_ >= GetUpperMarker()); //Tried to get unallocated memory
	_ASSERT(marker_ < size); //Tried to get memory outside of the stack

	if(marker_ >= GetUpperMarker()){
		return (void*)((size_t)base + size);
	}

	return (void*)((size_t)base + marker_);
}