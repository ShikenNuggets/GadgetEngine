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

	class DoubleBufferedStackAllocator{
	public:
		DoubleBufferedStackAllocator(size_t stackSizeBytes_) : currentStack(0), stacks{ StackAllocator(stackSizeBytes_), StackAllocator(stackSizeBytes_) }{}

		void SwapBuffers(){
			currentStack = (currentStack == 0) ? 1 : 0;
		}

		StackAllocator& CurrentBuffer(){
			_ASSERT(currentStack < numStacks);
			return stacks[currentStack];
		}

	private:
		static constexpr int numStacks = 2;

		uint8_t currentStack;
		StackAllocator stacks[numStacks];
	};
}

#endif //!GADGET_STACK_ALLOCATOR_H