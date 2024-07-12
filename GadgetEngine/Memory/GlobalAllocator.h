#ifndef GADGET_MEMORY_GLOBAL_ALLOCATOR_H
#define GADGET_MEMORY_GLOBAL_ALLOCATOR_H

#include <atomic>

namespace Gadget{
	class GlobalAllocator{
	public:
		static std::atomic<uint64_t> numAllocs;
		static std::atomic<uint64_t> numFrees;
	};
}

#endif //!GADGET_MEMORY_GLOBAL_ALLOCATOR_H