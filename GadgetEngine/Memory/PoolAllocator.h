#ifndef GADGET_POOL_ALLOCATOR_H
#define GADGET_POOL_ALLOCATOR_H

#include <list>

namespace Gadget{
	template <class T>
	class PoolAllocator{
	public:
		PoolAllocator(size_t numElements_) : numElements(numElements_), base(nullptr), freeElements(){
			base = static_cast<T*>(malloc(numElements * sizeof(T)));

			for(size_t i = 0; i < numElements; i++){
				freeElements.push_back(base + (i * sizeof(T)));
			}
		}

		~PoolAllocator(){
			free(base);
		}

		constexpr bool CanAllocate() const{
			return !freeElements.empty();
		}

		T* Allocate(){
			_ASSERT(CanAllocate());
			
			T* ptr = freeElements.front();
			freeElements.pop_front();
		}

		void Free(T* ptr_){
			_ASSERT(ptr_ > base && ptr_ < base + (numElements * sizeof(T))); //Tried to free memory outside of the pool
			freeElements.push_back(ptr_);
		}

	private:
		const size_t numElements;
		T* base;
		std::list<T*> freeElements; //TODO - Find a way to store this in the pool itself
	};
}

#endif //!GADGET_POOL_ALLOCATOR_H