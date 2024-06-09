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

		Array(const Array<T>& array_) : data(nullptr), size(0), capacity(array_.Capacity()){
			data = new T[capacity];
			for(int i = 0; i < array_.Size(); i++){
				Add(array_[i]);
			}
		}

		Array(size_t initialCapacity_) : data(nullptr), size(0), capacity(initialCapacity_){
			data = new T[capacity];
		}

		~Array(){
			delete[] data;
		}

		void Add(const T& value_){
			if(size >= capacity){
				Reserve(capacity * 2);
			}

			GADGET_BASIC_ASSERT(size < capacity);

			data[size] = value_;
			size++;
		}

		void Pop(){
			if(!IsEmpty()){
				size--;
			}
		}

		void RemoveAt(size_t index_){
			GADGET_BASIC_ASSERT(index_ < size);
			if(index_ >= size){
				return;
			}

			for(size_t i = index_; i < size - 1; i++){
				data[i] = data[i + 1];
			}

			size--;
		}

		void Remove(const T& value_){
			for(size_t i = 0; i < size; i++){
				if(data[i] == value_){
					RemoveAt(i);
					return;
				}
			}
		}

		void RemoveAll(const T& value_){
			for(size_t i = 0; i < size; i++){
				if(data[i] == value_){
					RemoveAt(i);
				}
			}
		}

		void Reserve(size_t newCapacity_){
			if(newCapacity_ <= capacity){
				return;
			}

			capacity = newCapacity_;
			Recreate();
		}

		void ShrinkToFit(){
			if(capacity == size){
				return;
			}

			capacity = size;
			Recreate();
		}

		void QuickSort(){
			if(size < 2){
				return;
			}

			QuickSort(0, size - 1);
		}

		constexpr bool Contains(const T& value_) const{
			for(int i = 0; i < size; i++){
				if(data[i] == value_){
					return true;
				}
			}

			return false;
		}

		constexpr const T& operator[](size_t i_) const{
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

		T& operator[](size_t i_){
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

		constexpr size_t Size() const{ return size; }
		constexpr size_t IsEmpty() const{ return size == 0; }
		constexpr size_t Capacity() const{ return capacity; }

	private:
		T* data;
		size_t size;
		size_t capacity;

		void Recreate(){
			GADGET_BASIC_ASSERT(size <= capacity);
			T* newData = new T[capacity];
			for(size_t i = 0; i < size; i++){
				newData[i] = data[i];
			}

			delete[] data;
			data = newData;
		}

		void QuickSort(size_t low_, size_t high_){
			if(high_ - low_ < 2){
				return;
			}

			size_t pivot = QuickSortPartition(low_, high_);
			QuickSort(low_, pivot);
			QuickSort(pivot + 1, high_);
		}

		//Hoare Partition Scheme
		size_t QuickSortPartition(size_t low_, size_t high_){
			const T& pivot = data[low_];

			size_t leftIndex = low_ = 1;
			size_t rightIndex = high_;

			while(true){
				do{
					leftIndex++;
				} while(data[leftIndex] < pivot);

				do{
					rightIndex--;
				} while(data[rightIndex] > pivot);

				if(leftIndex >= rightIndex){
					return rightIndex;
				}

				const T tmp = data[leftIndex];
				data[leftIndex] = data[rightIndex];
				data[rightIndex] = tmp;
			}
		}
	};
}

#endif //!GADGET_ARRAY_H