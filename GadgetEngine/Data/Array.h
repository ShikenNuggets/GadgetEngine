#ifndef GADGET_ARRAY_H
#define GADGET_ARRAY_H

#include "Debug.h"
#include "Math/Math.h"

namespace Gadget{
	template <class T>
	class Array{
	public:
		class Iterator{
		public:
			constexpr Iterator(Array<T>& data_, int64_t index_) : data(data_), index(index_){}

			constexpr inline const T& operator*() const{ return data[index]; }
			constexpr inline T& operator*(){ return data[index]; }

			constexpr inline Iterator& operator++(){ index++; return *this; }
			constexpr inline Iterator& operator--(){ index--; return *this; }
			constexpr inline bool operator!=(const Iterator& it_) const{ return index != it_.index; }

			constexpr inline int64_t Index() const{ return index; }

		private:
			Array<T>& data;
			int64_t index;
		};

		class ConstIterator{
		public:
			constexpr ConstIterator(const Array<T>& data_, int64_t index_) : data(data_), index(index_){}

			constexpr inline const T& operator*() const{ return data[index]; }

			constexpr inline ConstIterator& operator++(){ index++; return *this; }
			constexpr inline ConstIterator& operator--(){ index--; return *this; }
			constexpr inline bool operator!=(const ConstIterator& it_) const{ return index != it_.index; }

			constexpr inline int64_t Index() const{ return index; }

		private:
			const Array<T>& data;
			int64_t index;
		};

		Array() : data(nullptr), size(0), capacity(0){}

		Array(const Array<T>& array_) : data(nullptr), size(0), capacity(0){
			Reserve(array_.Capacity());
			for(int64_t i = 0; i < array_.Size(); i++){
				Add(array_[i]);
			}
		}

		Array(Array<T>&& other_) noexcept : data(other_.data), size(other_.size), capacity(other_.capacity){
			other_.data = nullptr;
			other_.size = 0;
			other_.capacity = 0;

			GADGET_BASIC_ASSERT(other_.IsEmpty());
		}

		constexpr Array<T>& operator=(const Array<T>& other_){
			if(&other_ == this){
				return *this;
			}

			GADGET_BASIC_ASSERT(other_.Capacity() > 0);

			Clear();

			capacity = other_.Capacity();
			Reallocate();
			for(int64_t i = 0; i < other_.Size(); i++){
				Add(other_[i]);
			}

			return *this;
		}

		constexpr Array<T>& operator=(Array<T>&& other_) noexcept{
			if(&other_ == this){
				return *this;
			}

			Clear();

			capacity = other_.capacity;
			Reallocate();
			for(int64_t i = 0; i < other_.Size(); i++){
				Add(other_[i]);
			}

			other_.data = nullptr;
			other_.size = 0;
			other_.capacity = 0;

			GADGET_BASIC_ASSERT(other_.IsEmpty());
			return *this;
		}

		explicit Array(int64_t initialCapacity_) : data(nullptr), size(0), capacity(0){
			Reserve(initialCapacity_);
		}

		~Array(){
			Clear();
			::operator delete[](data, capacity * sizeof(T));
		}

		T& Add(const T& value_){
			GADGET_BASIC_ASSERT(capacity >= 0);
			if(capacity <= 0){
				Reserve(16);
			}

			if(size >= capacity){
				Reserve(capacity * 2);
			}

			GADGET_BASIC_ASSERT(size < capacity);

			new (&data[size]) T(value_);
			size++;

			return data[size - 1];
		}

		void Add(const T* range_, int64_t rangeSize_){
			Reserve(size + rangeSize_);

			for(int64_t i = 0; i < rangeSize_; i++){
				Add(range_[i]);
			}
		}

		void Add(const Array<T>& range_){
			GADGET_ASSERT(range_.Capacity() > 0, "Adding an Array of 0 to another array... did you mean to do that?");
			Add(range_.data, range_.Size());
		}

		template <typename... Args>
		T& Emplace(Args... args_){
			GADGET_BASIC_ASSERT(capacity >= 0);
			if(capacity == 0){
				Reserve(16);
			}

			if(size >= capacity){
				Reserve(capacity * 2);
			}

			GADGET_BASIC_ASSERT(size < capacity);

			new (&data[size]) T(args_...);
			size++;

			return data[size - 1];
		}

		void InsertAt(int64_t index_, const T& value_){
			if(index_ >= size){
				Add(value_);
				return;
			}

			Reserve(size + 1);
			ShiftElements(index_, 1);

			new (&data[index_]) T(value_);
		}

		void InsertAt(int64_t index_, const T* range_, int64_t rangeSize_){
			if(index_ >= size){
				Add(range_, rangeSize_);
				return;
			}

			Reserve(size + rangeSize_);
			ShiftElements(index_, rangeSize_);

			for(int64_t i = 0; i < rangeSize_; i++){
				new (&data[i + index_]) T(range_[i]);
			}
		}

		void InsertAt(int64_t index_, const Array<T>& range_){
			GADGET_ASSERT(range_.Capacity() > 0, "Adding an Array of 0 to another array... did you mean to do that?");
			InsertAt(index_, range_.data, range_.Size());
		}

		constexpr void Pop(int64_t elementsToPop = 1){
			GADGET_BASIC_ASSERT(elementsToPop <= size);
			const int64_t startSize = size;
			for(int64_t i = 0; i < elementsToPop && i < startSize; i++){
				data[size - 1].~T();
				size--;
			}

			GADGET_BASIC_ASSERT(size >= 0);
			GADGET_BASIC_ASSERT(size <= capacity);
		}

		constexpr void RemoveAt(int64_t index_){
			GADGET_BASIC_ASSERT(index_ < size);
			if(index_ >= size){
				return;
			}

			data[index_].~T();
			ShiftElements(index_ + 1, -1);
		}

		constexpr void RemoveAt(int64_t startIndex_, int64_t rangeSize_){
			if(startIndex_ >= size || rangeSize_ == 0){
				return;
			}

			if(startIndex_ + rangeSize_ > size){
				Pop(size - startIndex_);
				return;
			}

			GADGET_BASIC_ASSERT(rangeSize_ < size);
			for(int64_t i = startIndex_; i < startIndex_ + rangeSize_; i++){
				data[i].~T();
			}

			ShiftElements(startIndex_, -static_cast<int64_t>(rangeSize_));
		}

		constexpr void Remove(const T& value_){
			for(int64_t i = 0; i < size; i++){
				if(data[i] == value_){
					RemoveAt(i);
					return;
				}
			}
		}

		constexpr void RemoveAll(const T& value_){
			for(int64_t i = 0; i < size; i++){
				if(data[i] == value_){
					RemoveAt(i);
				}
			}
		}

		constexpr void Clear(){
			Pop(size);
		}

		void Reserve(int64_t newCapacity_){
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

		constexpr int64_t Find(const T& value_, int64_t startPos_ = 0) const{
			for(int64_t i = startPos_; static_cast<int64_t>(i) < size; i++){
				if(data[i] == value_){
					return i;
				}
			}

			return -1;
		}

		constexpr bool Contains(const T& value_) const{ return Find(value_) != -1; }

		Array SubRange(int64_t startIndex_, int64_t endIndex_) const{
			GADGET_BASIC_ASSERT(startIndex_ != endIndex_);
			if(startIndex_ >= size){
				return Array();
			}

			if(endIndex_ > size){
				endIndex_ = size;
			}

			int64_t subRangeSize = endIndex_ - startIndex_;
			Array arr = Array(subRangeSize);
			for(int64_t i = startIndex_; i < endIndex_; i++){
				arr.Add(data[i]);
			}

			return arr;
		}

		constexpr const T& operator[](int64_t i_) const{
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

		constexpr T& operator[](int64_t i_){
			GADGET_BASIC_ASSERT(i_ < size);
			return data[i_];
		}

		constexpr bool operator==(const Array<T>& array_) const{
			GADGET_BASIC_ASSERT(array_.Capacity() > 0);
			if(size != array_.size){
				return false;
			}

			for(int64_t i = 0; i < size; i++){
				if(data[i] != array_[i]){
					return false;
				}
			}

			return true;
		}

		constexpr bool IsSorted(){
			for(int64_t i = 0; i < size - 1; i++){
				if(data[i] > data[i + 1]){
					return false;
				}
			}

			return true;
		}

		constexpr inline int64_t Size() const{ return size; }
		constexpr inline size_t SizeInBytes() const{ return sizeof(Array<T>) + (sizeof(T) * capacity); }
		constexpr inline int64_t IsEmpty() const{ return size == 0; }
		constexpr inline int64_t Capacity() const{ return capacity; }

		constexpr Iterator begin(){ return Iterator(*this, 0); }
		constexpr const ConstIterator begin() const{ return ConstIterator(*this, 0); }
		constexpr Iterator end(){ return Iterator(*this, size); }
		constexpr const ConstIterator end() const{ return ConstIterator(*this, size); }

	private:
		T* data;
		int64_t size;
		int64_t capacity;

		void Reallocate(){
			GADGET_BASIC_ASSERT(size <= capacity);

			T* newData = nullptr;
			if(capacity > 0){
				newData = static_cast<T*>(::operator new[](capacity * sizeof(T)));
				GADGET_BASIC_ASSERT(newData != nullptr);
				if(newData == nullptr){
					//TODO - Out of Memory is probably recoverable, we just need a way to trigger "garbage collection"
					Debug::ThrowFatalError(SID("DATA"), "Array::Reallocate failed!", ErrorCode::Out_Of_Memory, __FILE__, __LINE__);
				}

				std::memcpy(newData, data, size * sizeof(T));
			}

			::operator delete[](data);
			data = newData;
		}

		void ShiftElements(int64_t startIndex_, int64_t amountToShift_){
			if(amountToShift_ == 0){
				return;
			}

			if(size + amountToShift_ > capacity){
				Reserve(size + amountToShift_);
			}

			int64_t srcIndex = startIndex_;
			int64_t destIndex = startIndex_ + amountToShift_;
			while(destIndex < 0){
				//To avoid memmoving outside the buffer - ignore the first N elements if amount to shift is -N
				srcIndex++;
				destIndex++;
			}

			const int64_t numElementsToShift = size - srcIndex;
			const int64_t numBytesToShift = Math::Abs(numElementsToShift * sizeof(T));
			const int64_t shiftEndPos = srcIndex + numElementsToShift + amountToShift_;

			GADGET_BASIC_ASSERT(numElementsToShift <= static_cast<int64_t>(size));
			GADGET_BASIC_ASSERT(startIndex_ < size);
			GADGET_BASIC_ASSERT(shiftEndPos <= static_cast<int64_t>(capacity));

			std::memmove(&data[destIndex], &data[srcIndex], numBytesToShift);
			size += amountToShift_;
		}

		constexpr void QuickSort(int64_t low_, int64_t high_){
			if(low_ >= 0 && high_ >= 0 && low_ < high_){
				int64_t pivot = QuickSortPartition(low_, high_);
				QuickSort(low_, pivot);
				QuickSort(pivot + 1, high_);
			}
		}

		//Hoare Partition Scheme
		constexpr int64_t QuickSortPartition(int64_t low_, int64_t high_){
			const T pivot = data[low_];

			int64_t leftIndex = low_ - 1;
			int64_t rightIndex = high_ + 1;

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