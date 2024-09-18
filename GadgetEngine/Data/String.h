#ifndef GADGET_STRING_H
#define GADGET_STRING_H

#include <cstdint>

#include "Data/Array.h"
#include "Data/StaticArray.h"
#include "Math/Math.h"

namespace Gadget{
	class String{
	public:
		String(const char* str_ = "") : size(0), capacity(gStackStrSize), data(){
			GADGET_BASIC_ASSERT(std::strlen(str_) < std::numeric_limits<int32_t>::max());
			Append(str_);
			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		String(const std::string& str_) : size(0), capacity(gStackStrSize), data(){
			Append(str_.data(), str_.size());
			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		explicit String(const Array<char>& strArray_) : size(0), capacity(gStackStrSize), data(){
			GADGET_BASIC_ASSERT(strArray_.Size() < std::numeric_limits<int32_t>::max());
			Append(strArray_);
			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		explicit String(int32_t capacity_) : size(0), capacity(gStackStrSize), data(){
			Reserve(capacity_);
			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		String(const String& other_) : size(0), capacity(gStackStrSize), data(){
			Append(other_);
			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		String(String&& other_) noexcept : size(other_.size), capacity(other_.capacity), data(other_.data){
			other_.size = 0;
			other_.capacity = gStackStrSize;
			other_.data.stackStr.Fill('\0');

			GADGET_BASIC_ASSERT(HasNullTerminator());
			GADGET_BASIC_ASSERT(other_.HasNullTerminator());
		}

		constexpr String& operator=(const String& other_){
			if(&other_ == this){
				return *this;
			}

			if(!IsStackStr()){
				delete[] data.heapStr;
				data.heapStr = nullptr;
			}
			size = 0;
			capacity = gStackStrSize;
			data.stackStr.Fill('\0');

			Append(other_);
			GADGET_BASIC_ASSERT(HasNullTerminator());
			return *this;
		}

		constexpr String& operator=(String&& other_) noexcept{
			if(&other_ == this){
				return *this;
			}

			if(!IsStackStr()){
				delete[] data.heapStr;
				data.heapStr = nullptr;
			}

			size = other_.size;
			capacity = other_.capacity;
			data = other_.data;

			other_.size = 0;
			other_.capacity = gStackStrSize;
			other_.data.stackStr.Fill('\0');

			GADGET_BASIC_ASSERT(HasNullTerminator());
			GADGET_BASIC_ASSERT(other_.HasNullTerminator());
			return *this;
		}

		constexpr ~String(){
			if(!IsStackStr()){
				delete[] data.heapStr;
			}
		}

		void Append(const char* str_){
			GADGET_BASIC_ASSERT(std::strlen(str_) < std::numeric_limits<int32_t>::max());
			Append(str_, static_cast<int32_t>(std::strlen(str_)));
		}

		void Append(const char* str_, int32_t len_){
			GADGET_BASIC_ASSERT(len_ >= 0);
			GADGET_BASIC_ASSERT(len_ == std::strlen(str_));
			if(len_ == 0){
				Value()[size] = '\0';
				return;
			}

			const int32_t desiredSize = len_ + size;
			Reserve(desiredSize);

			char* strData = Value();
			GADGET_BASIC_ASSERT(strData != nullptr);
			for(int32_t i = 0; i < len_; i++){
				strData[size] = str_[i];
				size++;
			}
			
			GADGET_BASIC_ASSERT(size < capacity);
			strData[size] = '\0';
		}

		constexpr void Append(char c_){
			const int32_t desiredSize = size + 1;
			Reserve(desiredSize);

			char* strData = Value();
			strData[size] = c_;
			size++;

			GADGET_BASIC_ASSERT(size < capacity);
			strData[size] = '\0';
		}

		void Append(const String& str_){ Append(str_.Value(), str_.Length()); }

		void Append(const Array<char>& strArray_){
			GADGET_BASIC_ASSERT(strArray_.Size() < std::numeric_limits<int32_t>::max());
			Append(&strArray_[0], static_cast<int32_t>(strArray_.Size()));
		}

		void Append(int32_t number){
			StaticArray<char, 16> buffer{};
			itoa(number, &buffer[0], 10);
			Append(&buffer[0]);
		}

		void Append(const Array<String>& strs_){
			int32_t newLength = size;
			for(int32_t i = 0; i < strs_.Size(); i++){
				newLength += strs_[i].Length();
			}

			Reserve(newLength);

			for(int64_t i = 0; i < strs_.Size(); i++){
				Append(strs_[i].Value(), strs_[i].Length());
			}
		}

		String operator+(const String& str_) const{
			String finalStr = *this;
			finalStr.Append(str_);
			return finalStr;
		}

		String operator+(char c_) const{
			String finalStr = *this;
			finalStr.Append(c_);
			return finalStr;
		}

		String operator+(int32_t number_) const{
			String finalStr = *this;
			finalStr.Append(number_);
			return finalStr;
		}

		void operator+=(const String& str_){ Append(str_); }
		constexpr void operator+=(char c_){ Append(c_); }
		void operator+=(int32_t number_){ Append(number_); }

		constexpr void Pop(int32_t elementsToRemove_ = 1){
			GADGET_BASIC_ASSERT(elementsToRemove_ >= 0);
			GADGET_BASIC_ASSERT(elementsToRemove_ <= size);

			size = Math::Clamp<int32_t>(0, std::numeric_limits<int32_t>::max(), size - elementsToRemove_);
			Value()[size] = '\0';

			GADGET_BASIC_ASSERT(size >= 0);
			GADGET_BASIC_ASSERT(size < capacity);
			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		constexpr void InsertAt(int32_t index_, char c_){
			if(index_ >= Length()){
				Append(c_);
				return;
			}

			Reserve(size + 1);
			ShiftElements(index_, 1);

			char* strData = Value();
			strData[index_] = c_;
			
			GADGET_BASIC_ASSERT(size < capacity);
			strData[size] = '\0';
		}

		constexpr void RemoveAt(int32_t index_){
			if(index_ >= Length()){
				return;
			}

			ShiftElements(index_ + 1, -1);
			char* strData = Value();
			
			GADGET_BASIC_ASSERT(size < capacity);
			strData[size] = '\0';
		}

		constexpr void RemoveAt(int32_t startIndex_, int32_t rangeSize_){
			if(startIndex_ >= size || rangeSize_ == 0){
				return;
			}

			if(startIndex_ + rangeSize_ > size){
				size -= (size - startIndex_);
				Pop(size - startIndex_);
				return;
			}

			GADGET_BASIC_ASSERT(rangeSize_ < size);
			ShiftElements(startIndex_, -rangeSize_);
		}

		constexpr void Remove(char value_){
			GADGET_BASIC_ASSERT(value_ != '\0'); //Don't try to remove the null terminator
			char* strData = Value();
			for(int32_t i = 0; i < Length(); i++){
				if(strData[i] == value_){
					RemoveAt(i);
					return;
				}
			}
		}

		constexpr void RemoveAll(char value_){
			GADGET_BASIC_ASSERT(value_ != '\0'); //Don't try to remove the null terminator
			char* strData = Value();
			for(int32_t i = 0; i < Length(); i++){
				if(strData[i] == value_){
					RemoveAt(i);
					i--;
				}
			}
		}

		//Generous, allocates more memory than you asked for
		constexpr void Reserve(int32_t capacity_){
			if(capacity > capacity_){
				return;
			}

			GADGET_BASIC_ASSERT(capacity_ > 0);
			int32_t newCapacity = capacity;
			while(newCapacity <= capacity_){
				GADGET_BASIC_ASSERT(newCapacity > 0);
				newCapacity *= 2;
			}

			ReserveExact(newCapacity);
		}

		//Allocates exactly what you asked for (well, +1 for the null terminator)
		constexpr void ReserveExact(int32_t capacity_){
			if(capacity_ < capacity){
				return;
			}

			ReallocateHeapStr(capacity_);
		}

		constexpr void ShrinkToFit(){
			if(IsStackStr() || size == capacity){
				return;
			}

			if(size < gStackStrSize){
				MoveHeapStrToStack();
			}else{
				ReallocateHeapStr(size);
			}
		}

		static constexpr inline bool IsWhitespace(char c){ return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f'; }

		constexpr void Trim(){
			char* str = Value();
			int32_t whitespaceAtStart = 0;
			for(whitespaceAtStart = 0; whitespaceAtStart < size; whitespaceAtStart++){
				if(!IsWhitespace(str[whitespaceAtStart])){
					break;
				}
			}

			RemoveAt(0, whitespaceAtStart);

			GADGET_BASIC_ASSERT(size >= 1);

			if(size > 0){
				int32_t indexToEndWhitespace = 0;
				for(indexToEndWhitespace = size - 1; indexToEndWhitespace >= 0; indexToEndWhitespace--){
					if(!IsWhitespace(str[indexToEndWhitespace])){
						break;
					}
				}

				RemoveAt(indexToEndWhitespace + 1, size);
				str[size] = '\0';
			}

			GADGET_BASIC_ASSERT(size < capacity);
		}

		constexpr void FindAndReplace(char find_, const String& replace_){
			GADGET_BASIC_ASSERT(find_ != '\0'); //Do not try to replace the null terminator
			
			char* str = Value();
			for(int32_t i = 0; i < size; i++){
				if(str[i] == find_){
					RemoveAt(i);

					for(int32_t j = 0; j < replace_.size; j++){
						InsertAt(i + j, replace_[j]);
					}

					str = Value();
				}
			}

			GADGET_BASIC_ASSERT(size < capacity);
		}

		constexpr void ToLower(){
			char* str = Value();
			for(int32_t i = 0; i < size; i++){
				str[i] = static_cast<char>(std::tolower(str[i]));
			}
		}

		constexpr void ToUpper(){
			char* str = Value();
			for(int32_t i = 0; i < size; i++){
				str[i] = static_cast<char>(std::toupper(str[i]));
			}
		}

		constexpr inline int32_t Find(char c_, int32_t startPos_ = 0) const{
			const char* str = Value();
			for(int32_t i = startPos_; i < size; i++){
				if(str[i] == c_){
					return i;
				}
			}

			return -1;
		}

		constexpr inline bool Contains(char c_) const{ return Find(c_) != -1; }

		//TODO - Naive brute force implementation. Consider other options: https://en.wikipedia.org/wiki/String-searching_algorithm#Single-pattern_algorithms
		constexpr bool Contains(const String& str_) const{
			const char* str = Value();
			if(size < str_.Length()){
				return false;
			}

			for(int32_t i = 0; i < size; i++){
				bool isEqual = true;

				int32_t j = i;
				for(int32_t k = 0; k < str_.Length(); j++, k++){
					if(str[j] != str_[k]){
						GADGET_ASSERT(str_[k] != '\0', "String comparison failing due to null terminator!");

						isEqual = false;
						break;
					}
				}

				if(isEqual){
					return true;
				}
			}

			return false;
		}

		String SubString(int32_t startIndex_, int32_t endIndex_) const{
			GADGET_BASIC_ASSERT(startIndex_ != endIndex_);
			if(startIndex_ >= size){
				return String();
			}

			if(endIndex_ > size){
				endIndex_ = size;
			}

			const int32_t subRangeSize = endIndex_ - startIndex_;
			String result;
			result.Reserve(subRangeSize);

			const char* str = Value();
			for(int32_t i = startIndex_; i < endIndex_; i++){
				result.Append(str[i]);
			}

			return result;
		}

		constexpr void QuickSort(){
			if(size < 2){
				return;
			}

			QuickSort(0, size - 1);

			GADGET_BASIC_ASSERT(IsSorted());
			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		constexpr inline const char* Value() const{
			if(IsStackStr()){
				return &data.stackStr[0];
			}

			return &data.heapStr[0];
		}

		bool operator==(const char* str_) const{
			GADGET_BASIC_ASSERT(std::strlen(str_) < std::numeric_limits<int32_t>::max());
			if(size != static_cast<int32_t>(std::strlen(str_))){
				return false;
			}

			const char* str = Value();
			for(int64_t i = 0; i < size; i++){
				if(str[i] != str_[i]){
					return false;
				}
			}

			return true;
		}

		bool operator==(const String& str_) const{ return *this == str_.Value(); }

		constexpr const char& operator[](int32_t i_) const{
			GADGET_BASIC_ASSERT(i_ < Length());
			return Value()[i_];
		}

		constexpr char& operator[](int32_t i_){
			GADGET_BASIC_ASSERT(i_ < size);
			return Value()[i_];
		}

		constexpr inline int32_t Length() const{ return size; }

		constexpr bool IsEmpty() const{ return size > 0; }

	private:
		static inline constexpr int gStackStrSize = 8;

		union StrData{
			StrData() : stackStr(){ stackStr.Fill('\0'); }

			StaticArray<char, gStackStrSize> stackStr;
			char* heapStr;
		};

		int32_t size;
		int32_t capacity;
		StrData data;

		constexpr void ReallocateHeapStr(int32_t newCapacity_){
			if(newCapacity_ < gStackStrSize || newCapacity_ < capacity){
				return;
			}

			char* strRaw = Value();
			Array<char> oldData;
			oldData.Reserve(size);
			for(int32_t i = 0; i < size; i++){
				oldData.Add(strRaw[i]);
			}

			if(!IsStackStr()){
				delete[] data.heapStr;
				data.heapStr = nullptr;
			}

			capacity = newCapacity_ + 1;
			data.heapStr = new char[capacity];
			for(int32_t i = 0; i < capacity; i++){
				if(oldData.Size() > i){
					data.heapStr[i] = oldData[i];
				}else{
					data.heapStr[i] = '\0';
				}
			}

			GADGET_BASIC_ASSERT(HasNullTerminator());
			GADGET_BASIC_ASSERT(size < capacity);
		}

		constexpr void MoveHeapStrToStack(){
			GADGET_BASIC_ASSERT(size < gStackStrSize);
			GADGET_BASIC_ASSERT(!IsStackStr());
			if(size >= gStackStrSize || IsStackStr()){
				return;
			}

			Array<char> oldData;
			oldData.Reserve(size);
			for(int32_t i = 0; i < size; i++){
				oldData.Add(data.heapStr[i]);
			}

			delete[] data.heapStr;
			data.heapStr = nullptr;

			capacity = gStackStrSize;
			data.stackStr.Fill('\0');
			for(int32_t i = 0; i < Length(); i++){
				data.stackStr[i] = oldData[i];
			}

			GADGET_BASIC_ASSERT(HasNullTerminator());
			GADGET_BASIC_ASSERT(size < capacity);
		}

		constexpr void ShiftElements(int32_t startIdx_, int32_t amount_){
			if(amount_ == 0){
				return;
			}

			if(size + amount_ > capacity){
				Reserve(size + amount_);
			}

			int32_t srcIndex = startIdx_;
			int32_t destIndex = startIdx_ + amount_;
			while(destIndex < 0){
				//To avoid memmoving outside the buffer - ignore the first N elements if amount to shift is -N
				srcIndex++;
				destIndex++;
			}

			const int32_t numElementsToShift = size - srcIndex;
			const int32_t numBytesToShift = Math::Abs<int32_t>(numElementsToShift /* * sizeof(char) */);
			const int32_t shiftEndPos = srcIndex + numElementsToShift + amount_;

			GADGET_BASIC_ASSERT(numElementsToShift <= static_cast<int32_t>(size));
			GADGET_BASIC_ASSERT(startIdx_ <= size);
			GADGET_BASIC_ASSERT(shiftEndPos <= static_cast<int32_t>(capacity));

			char* str = Value();
			std::memmove(&str[destIndex], &str[srcIndex], numBytesToShift);
			size += amount_;

			GADGET_BASIC_ASSERT(size < capacity);
			str[size] = '\0';

			GADGET_BASIC_ASSERT(HasNullTerminator());
		}

		constexpr inline bool HasNullTerminator() const{
			const char* str = Value();
			GADGET_BASIC_ASSERT(str != nullptr);
			GADGET_BASIC_ASSERT(capacity > size);
			return str[size] == '\0';
		}

		constexpr inline char* Value(){
			if(IsStackStr()){
				return &data.stackStr[0];
			}

			return &data.heapStr[0];
		}

		constexpr inline bool IsStackStr() const{ return capacity == gStackStrSize; }

		constexpr void QuickSort(int32_t low_, int32_t high_){
			if(low_ >= 0 && high_ >= 0 && low_ < high_){
				const int32_t pivot = QuickSortPartition(low_, high_);
				QuickSort(low_, pivot);
				QuickSort(pivot + 1, high_);
			}
		}

		//Hoare Partition Scheme
		constexpr int32_t QuickSortPartition(int32_t low_, int32_t high_){
			char* str = Value();

			const char pivot = str[low_];

			int32_t leftIndex = low_ - 1;
			int32_t rightIndex = high_ + 1;

			while(true){
				do{
					leftIndex++;
				} while(str[leftIndex] < pivot);

				do{
					rightIndex--;
				} while(str[rightIndex] > pivot);

				if(leftIndex >= rightIndex){
					return rightIndex;
				}

				const char tmp = str[leftIndex];
				str[leftIndex] = str[rightIndex];
				str[rightIndex] = tmp;
			}
		}

		constexpr bool IsSorted() const{
			const char* str = Value();
			for(int32_t i = 0; i < size - 1; i++){
				if(str[i] > str[i + 1]){
					return false;
				}
			}

			return true;
		}
	};
}

inline std::ostream& operator<<(std::ostream& os_, const Gadget::String& str_){
	os_ << str_.Value();
	return os_;
}

#endif //!GADGET_STRING_H