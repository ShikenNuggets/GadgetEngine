#ifndef GADGET_DOUBLE_STACK_ALLOCATOR_H
#define GADGET_DOUBLE_STACK_ALLOCATOR_H

#include <stack>

namespace Gadget{
	//Double ended stack allocators
	//Allows allocations to be made at both the bottom and the top of the stack
	class DoubleStackAllocator{
	public:
		typedef size_t LowerMarker;
		typedef size_t UpperMarker;

		explicit DoubleStackAllocator(size_t bytes_);
		~DoubleStackAllocator();

		LowerMarker GetLowerMarker() const;
		UpperMarker GetUpperMarker() const;
		bool CanAllocate(size_t bytesToAllocate_) const;

		void* AllocateLower(size_t bytes_);
		void* AllocateUpper(size_t bytes_);
		void FreeLower(LowerMarker marker_);
		void FreeUpper(UpperMarker marker_);
		void Clear();

	private:
		const size_t size;
		void* base;
		std::stack<LowerMarker> lowerMarkers;
		std::stack<UpperMarker> upperMarkers;

		void* GetLowerPtr() const;
		void* GetUpperPtr() const;

		void* GetLowerMarkerPtr(LowerMarker marker_) const;
		void* GetUpperMarkerPtr(UpperMarker marker_) const;
	};
}

#endif //!GADGET_DOUBLE_STACK_ALLOCATOR_H