#ifndef GADGET_DATA_STATIC_ARRAY_H
#define GADGET_DATA_STATIC_ARRAY_H

#include <cstdint>

#include "Debug.h"

namespace Gadget{
	template <class T, int64_t Size>
	struct StaticArray{
		static_assert(Size > 0);

		class Iterator{
		public:
			constexpr Iterator(StaticArray<T, Size>& data_, int64_t index_) : data(data_), index(index_){}

			constexpr inline const T& operator*() const{ return data[index]; }
			constexpr inline T& operator*(){ return data[index]; }

			constexpr inline Iterator& operator++(){ index++; return *this; }
			constexpr inline bool operator!=(const Iterator& it_) const{ return index != it_.index; }

		private:
			StaticArray<T, Size>& data;
			int64_t index;
		};

		class ConstIterator{
		public:
			constexpr ConstIterator(const StaticArray<T, Size>& data_, int64_t index_) : data(data_), index(index_){}

			constexpr inline const T& operator*() const{ return data[index]; }

			constexpr inline ConstIterator& operator++(){ index++; return *this; }
			constexpr inline bool operator!=(const ConstIterator& it_) const{ return index != it_.index; }

		private:
			const StaticArray<T, Size>& data;
			int64_t index;
		};

		constexpr void QuickSort(){
			if constexpr(Size < 2){
				return;
			}

			QuickSort(0, Size - 1);

			GADGET_BASIC_ASSERT(IsSorted());
		}

		constexpr int64_t Find(const T& value_, int64_t startPos_ = 0) const{
			GADGET_BASIC_ASSERT(startPos_ < Size);
			for(int64_t i = startPos_; i < Size; i++){
				if(data[i] == value_){
					return i;
				}
			}

			return -1;
		}

		constexpr bool Contains(const T& value_) const{ return Find(value_) != -1; }

		template <int SubSize>
		StaticArray<T, SubSize> SubRange(int64_t startIndex_) const{
			static_assert(SubSize <= Size);

			StaticArray<T, SubSize> newArray;
			for(int64_t i = startIndex_; i < Size && i - startIndex_ < SubSize; i++){
				newArray[i] = data[i];
			}

			return newArray;
		}

		constexpr const T& operator[](int64_t i_) const{
			GADGET_BASIC_ASSERT(i_ >= 0);
			GADGET_BASIC_ASSERT(i_ < Size);
			return data[i_];
		}

		constexpr T& operator[](int64_t i_){
			GADGET_BASIC_ASSERT(i_ >= 0);
			GADGET_BASIC_ASSERT(i_ < Size);
			return data[i_];
		}

		constexpr bool operator==(const StaticArray<T, Size>& array_) const{
			for(int64_t i = 0; i < Size; i++){
				if(data[i] != array_[i]){
					return false;
				}
			}

			return true;
		}

		constexpr StaticArray<T, Size>& operator=(const StaticArray<T, Size>& other_){
			GADGET_BASIC_ASSERT(&other_ != this);
			if(&other_ == this){
				return *this;
			}

			for(int64_t i = 0; i < Size; i++){
				data[i] = other_[i];
			}
		}

		constexpr bool IsSorted() const{
			for(int64_t i = 0; i < Size - 1; i++){
				if(data[i] > data[i + 1]){
					return false;
				}
			}

			return true;
		}

		constexpr int64_t GetSize() const{ return Size; }

		constexpr Iterator begin(){ return Iterator(*this, 0); }
		constexpr const ConstIterator begin() const{ return ConstIterator(*this, 0); }
		constexpr Iterator end(){ return Iterator(*this, Size); }
		constexpr const ConstIterator end() const{ return ConstIterator(*this, Size); }

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

		T data[Size];
	};
}

#endif //!GADGET_DATA_STATIC_ARRAY_H