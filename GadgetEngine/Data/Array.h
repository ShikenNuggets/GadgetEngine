#ifndef GADGET_ARRAY_H
#define GADGET_ARRAY_H

#include "Debug.h"

namespace Gadget{
	template <class T>
	class Array{
	public:
		Array() : data(nullptr), size(0), capacity(16){
			GADGET_BASIC_ASSERT(capacity > 0);
			data = new T[capacity];
		}

		Array(const Array<T>& array_) : data(nullptr), size(0), capacity(0){
			Reserve(array_.Capacity());
			for(int i = 0; i < array_.Size(); i++){
				Add(array_[i]);
			}
		}

		Array(size_t initialCapacity_) : data(nullptr), size(0), capacity(0){
			GADGET_BASIC_ASSERT(initialCapacity_ > 0);
			Reserve(initialCapacity_);
		}

		~Array(){
			delete[] data;
		}

		void Add(const T& value_){
			GADGET_BASIC_ASSERT(capacity != 0);
			if(capacity == 0){
				Reserve(16);
			}

			if(size >= capacity){
				Reserve(capacity * 2);
			}

			GADGET_BASIC_ASSERT(size < capacity);

			data[size] = value_;
			size++;
		}

		void Add(const T* range_, size_t rangeSize_){
			Reserve(size + rangeSize_);

			for(size_t i = 0; i < rangeSize_; i++){
				Add(range_[i]);
			}
		}

		void Add(const Array<T>& range_){
			GADGET_BASIC_ASSERT(range_.Capacity() > 0);
			Add(range_.data, range_.Size());
		}

		void InsertAt(size_t index_, const T& value_){
			if(index_ >= size){
				Add(value_);
				return;
			}

			Add(T()); //This gets us the size and capacity increase without too much trouble
			for(size_t i = size - 1; i > index_; i--){
				data[i] = data[i - 1];
			}

			data[index_] = value_;
		}

		void InsertAt(size_t index_, const T* range_, size_t rangeSize_){
			if(index_ >= size){
				Add(range_, rangeSize_);
				return;
			}

			Reserve(size + rangeSize_);
			for(size_t i = 0; i < rangeSize_; i++){
				Add(T());
			}

			for(size_t i = size - 1; i > index_; i--){
				data[i] = data[i - rangeSize_];
			}

			for(size_t i = 0; i < rangeSize_; i++){
				data[i + index_] = range_[i];
			}
		}

		void InsertAt(size_t index_, const Array<T>& range_){
			GADGET_BASIC_ASSERT(range_.Capacity() > 0);
			InsertAt(index_, range_.data, range_.Size());
		}

		constexpr void Pop(size_t elementsToPop = 1){
			GADGET_BASIC_ASSERT(elementsToPop > 0); //You're probably doing something weird if you try to pop 0 elements
			if(size - elementsToPop < 0){
				size = 0;
			}else{
				size -= elementsToPop;
			}

			GADGET_BASIC_ASSERT(size < capacity);
		}

		constexpr void RemoveAt(size_t index_){
			GADGET_BASIC_ASSERT(index_ < size);
			if(index_ >= size){
				return;
			}

			for(size_t i = index_; i < size - 1; i++){
				data[i] = data[i + 1];
			}

			size--;
		}

		constexpr void RemoveAt(size_t startIndex_, size_t rangeSize_){
			if(startIndex_ >= size || rangeSize_ == 0){
				return;
			}

			if(startIndex_ + rangeSize_ > size){
				Pop(size - startIndex_);
				return;
			}

			for(size_t i = startIndex_; i < size - rangeSize_; i++){
				data[i] = data[i + rangeSize_];
			}

			GADGET_BASIC_ASSERT(rangeSize_ < size);
			size -= rangeSize_;
		}

		constexpr void Remove(const T& value_){
			for(size_t i = 0; i < size; i++){
				if(data[i] == value_){
					RemoveAt(i);
					return;
				}
			}
		}

		constexpr void RemoveAll(const T& value_){
			for(size_t i = 0; i < size; i++){
				if(data[i] == value_){
					RemoveAt(i);
				}
			}
		}

		constexpr void Clear(){
			size = 0;
		}

		void Reserve(size_t newCapacity_){
			if(newCapacity_ <= capacity){
				return;
			}

			capacity = newCapacity_;
			Reallocate();
		}

		void ShrinkToFit(){
			GADGET_BASIC_ASSERT(capacity >= size);

			if(capacity == size){
				return;
			}

			capacity = size;
			Reallocate();
		}

		constexpr void QuickSort(){
			if(size < 2){
				return;
			}

			QuickSort(0, size - 1);

			GADGET_BASIC_ASSERT(IsSorted());
		}

		constexpr int64_t Find(const T& value_) const{
			for(size_t i = 0; i < size; i++){
				if(data[i] == value_){
					return i;
				}
			}

			return -1;
		}

		constexpr bool Contains(const T& value_) const{ return Find(value_) != -1; }

		Array SubRange(size_t startIndex_, size_t endIndex_) const{
			if(startIndex_ >= size){
				return Array();
			}

			if(endIndex_ > size){
				endIndex_ = size;
			}

			size_t subRangeSize = endIndex_ - startIndex_;
			Array arr = Array(subRangeSize);
			for(size_t i = startIndex_; i < endIndex_; i++){
				arr.Add(data[i]);
			}

			return arr;
		}

		constexpr const T& operator[](size_t i_) const{
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

		constexpr T& operator[](size_t i_){
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

		constexpr bool operator==(const Array<T>& array_) const{
			GADGET_BASIC_ASSERT(array_.Capacity() > 0);
			if(size != array_.size){
				return false;
			}

			for(size_t i = 0; i < size; i++){
				if(data[i] != array_[i]){
					return false;
				}
			}

			return true;
		}

		constexpr Array<T>& operator=(const Array<T>& other_){
			GADGET_BASIC_ASSERT(other_.Capacity() > 0);

			if(data != nullptr){
				delete[] data;
				size = 0;
			}

			capacity = other_.Capacity();
			data = new T[capacity];
			for(int i = 0; i < other_.Size(); i++){
				Add(other_[i]);
			}

			return *this;
		}

		constexpr bool IsSorted(){
			for(size_t i = 0; i < size - 1; i++){
				if(data[i] > data[i + 1]){
					return false;
				}
			}

			return true;
		}

		constexpr size_t Size() const{ return size; }
		constexpr size_t IsEmpty() const{ return size == 0; }
		constexpr size_t Capacity() const{ return capacity; }

	private:
		T* data;
		size_t size;
		size_t capacity;

		void Reallocate(){
			GADGET_BASIC_ASSERT(size <= capacity);

			T* newData = nullptr;
			if(capacity > 0){
				newData = new T[capacity];
				for(size_t i = 0; i < size && i < capacity; i++){
					newData[i] = data[i];
				}
			}

			delete[] data;
			data = newData;
		}

		constexpr void QuickSort(size_t low_, size_t high_){
			if(low_ >= 0 && high_ >= 0 && low_ < high_){
				size_t pivot = QuickSortPartition(low_, high_);
				QuickSort(low_, pivot);
				QuickSort(pivot + 1, high_);
			}
		}

		//Hoare Partition Scheme
		constexpr size_t QuickSortPartition(size_t low_, size_t high_){
			const T pivot = data[low_];

			size_t leftIndex = low_ - 1;
			size_t rightIndex = high_ + 1;

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