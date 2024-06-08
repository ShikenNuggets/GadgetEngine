#ifndef GADGET_ARRAY_H
#define GADGET_ARRAY_H

#include "Debug.h"

namespace Gadget{
	template <class T>
	class Array{
	public:
		Array() : data(nullptr), size(0), capacity(16){
			data = new T[capacity];
		}

		Array(size_t initialCapacity_) : data(nullptr), size(0), capacity(initialCapacity_){
			data = new T[capacity];
		}

		~Array(){
			delete[] data;
		}

		void Add(const T& value_){
			if(size >= capacity){
				capacity = capacity * 2;
				T* newData = new T[capacity];
				for(size_t i = 0; i < size; i++){
					newData[i] = data[i];
				}

				delete[] data;
				data = newData;
			}

			GADGET_BASIC_ASSERT(size < capacity);

			data[size] = value_;
			size++;
		}

		void Pop(){
			size--;
		}

		constexpr T operator[](size_t i_) const{
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

		T& operator[](size_t i_){
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

	private:
		T* data;
		size_t size;
		size_t capacity;
	};
}

#endif //!GADGET_ARRAY_H