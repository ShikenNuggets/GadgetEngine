#ifndef GADGET_STACK_ALLOCATOR_H
#define GADGET_STACK_ALLOCATOR_H

#include <stack>

namespace Gadget{
	class StackAllocator{
	public:
		typedef size_t Marker;

		explicit StackAllocator(size_t bytes_);
		~StackAllocator();

		Marker GetMarker() const;
		bool CanAllocate(size_t bytesToAllocate_) const;

		void* Allocate(size_t bytes_);
		void FreeToMarker(Marker marker_);
		void Clear();

	private:
		const size_t size;
		void* base;
		std::stack<Marker> markers;

		void* GetTopPtr() const;
		void* GetMarkerPtr(Marker marker_) const;
	};
}

#endif //!GADGET_STACK_ALLOCATOR_H