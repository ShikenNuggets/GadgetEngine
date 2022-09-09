#ifndef GADGET_ALIGNED_ALLOCATOR_H
#define GADGET_ALIGNED_ALLOCATOR_H

#include "Debug.h"

namespace Gadget{
	class AlignedAllocator{
		static inline constexpr size_t AlignAddress(size_t address_, size_t align_){
			const size_t mask = align_ - 1;
			GADGET_BASIC_ASSERT((align_ & mask) == 0); //Alignment must be power of 2
			return (address_ + mask) & ~mask;
		}

		template <class T>
		static inline constexpr T* AlignPointer(T* ptr_, size_t align_){
			const size_t addr = reinterpret_cast<size_t>(ptr_);
			const size_t addrAligned = AlignAddress(addr, align_);
			return reinterpret_cast<T*>(addrAligned);
		}

		static void* AllocateAligned(size_t bytes_, size_t align_){
			size_t actualBytes = bytes_ + align_;
			uint8_t* rawMemory = new uint8_t[actualBytes];

			uint8_t* alignedMemory = AlignPointer(rawMemory, align_);
			if(alignedMemory == rawMemory){
				alignedMemory += align_;
			}

			ptrdiff_t shift = alignedMemory - rawMemory;
			GADGET_BASIC_ASSERT(shift > 0 && shift <= 256);

			alignedMemory[-1] = static_cast<uint8_t>(shift & 0xFF);

			return alignedMemory;
		}

		static void FreeAligned(void* ptr_){
			if(ptr_ == nullptr){
				return;
			}

			uint8_t* alignedMemory = reinterpret_cast<uint8_t*>(ptr_);

			ptrdiff_t shift = alignedMemory[-1];
			if(shift == 0){
				shift = 256;
			}

			uint8_t* rawMemory = alignedMemory - shift;
			delete[] rawMemory;
		}
	};
}

#endif //!GADGET_ALIGNED_ALLOCATOR_H