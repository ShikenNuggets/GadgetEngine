#ifndef GADGET_STRING_H
#define GADGET_STRING_H

#include "Data/Array.h"
#include "Math/Math.h"

namespace Gadget{
	class String{
	public:
		String(const char* str_ = "") : data(Math::Clamp<size_t>(16, std::numeric_limits<size_t>::max(), std::strlen(str_))){
			data.Add('\0');
			Append(str_);
		}

		String(const Array<char>& strArray_) : data(strArray_.Size()){
			data.Add('\0');
			Append(strArray_);
		}

		void Append(const char* str_){
			GADGET_BASIC_ASSERT(data[data.Size() - 1] == '\0');

			data.Pop(); //Pop the null terminator
			data.Add(str_, std::strlen(str_));
			data.Add('\0');
		}

		void Append(const String& str_){
			Append(str_.Value());
		}

		void Append(const Array<char>& strArray_){
			GADGET_BASIC_ASSERT(data[data.Size() - 1] == '\0');

			data.Pop();
			data.Add(strArray_);
			data.Add('\0');
		}

		void Append(const Array<String>& strs_){
			//This technically has higher time complexity than just doing one loop,
			//but doing only one memory allocation is worth it if we're appending lots of strings/large strings
			size_t newLength = Length();
			for(size_t i = 0; i < strs_.Size(); i++){
				newLength += strs_[i].Length();
			}

			data.Reserve(newLength);

			for(size_t i = 0; i < strs_.Size(); i++){
				Append(strs_[i].Value());
			}
		}

		void InsertAt(size_t index_, char c){
			data.InsertAt(index_, c);
		}

		void RemoveAt(size_t index_){
			data.RemoveAt(index_);
		}

		void Remove(char value_){
			data.Remove(value_);
		}

		void RemoveAll(char value_){
			data.RemoveAll(value_);
		}

		static bool IsWhitespace(char c){
			return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
		}
		
		void Trim(){
			size_t whitespaceAtStart;
			for(whitespaceAtStart = 0; whitespaceAtStart < Length(); whitespaceAtStart++){
				if(!IsWhitespace(data[whitespaceAtStart])){
					break;
				}
			}

			data.RemoveAt(0, whitespaceAtStart);

			size_t indexToEndWhitespace;
			for(indexToEndWhitespace = data.Size() - 2; indexToEndWhitespace >= 0; indexToEndWhitespace--){
				if(!IsWhitespace(data[indexToEndWhitespace])){
					break;
				}
			}

			data.RemoveAt(indexToEndWhitespace + 1, data.Size());
			data.Add('\0');
		}

		void FindAndReplace(char find_, const String& replace_){
			for(size_t i = 0; i < data.Size(); i++){
				if(data[i] == find_){
					RemoveAt(i);

					for(size_t j = 0; j < replace_.Length(); j++){
						InsertAt(i + j, replace_[j]);
					}
				}
			}
		}

		bool Contains(char c){
			return data.Contains(c);
		}

		//TODO - Naive brute force implementation. Consider other options: https://en.wikipedia.org/wiki/String-searching_algorithm#Single-pattern_algorithms
		bool Contains(const String& str_){
			if(data.Size() < str_.Length()){
				return false;
			}

			for(size_t i = 0; i < data.Size(); i++){
				bool isEqual = true;

				size_t j = i;
				for(size_t k = 0; k < str_.Length(); j++, k++){
					if(data[j] != str_[k]){
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

		String SubString(size_t startIndex_, size_t endIndex_) const{
			return String(data.SubRange(startIndex_, endIndex_));
		}

		void QuickSort(){
			if(!data.IsEmpty()){
				GADGET_BASIC_ASSERT(data[data.Size() - 1] == '\0');
			}

			data.Pop(); //We typically don't want to sort the null terminator

			data.QuickSort();

			data.Add('\0');
		}

		const char* Value() const{
			if(data.IsEmpty()){
				return "";
			}

			return &data[0];
		}

		const char* Value(){
			if(data.IsEmpty()){
				return "";
			}

			return &data[0];
		}

		bool operator==(const char* str_) const{
			if(data.Size() != std::strlen(str_)){
				return false;
			}

			for(size_t i = 0; i < data.Size(); i++){
				if(data[i] == str_[i]){
					return false;
				}
			}

			return true;
		}

		bool operator==(const String& str_) const{
			return data == str_.data;
		}

		constexpr const char& operator[](size_t i_) const{
			GADGET_BASIC_ASSERT(i_ < data.Size());
			return data[i_];
		}

		char& operator[](size_t i_){
			GADGET_BASIC_ASSERT(i_ < data.Size());
			return data[i_];
		}

		constexpr size_t Length() const{ return data.Size() - 1; }

		bool IsEmpty() const{
			GADGET_BASIC_ASSERT(data.Size() > 0);
			if(data.Size() == 1){
				GADGET_BASIC_ASSERT(data[0] == '\0');
			}else if(data.Size() > 1){
				GADGET_BASIC_ASSERT(data[0] != '\0');
				GADGET_BASIC_ASSERT(data[data.Size() - 1] == '\0');
			}

			return data.Size() == 1 || data[0] == '\0';
		}

	private:
		Array<char> data;
	};
}

#endif //!GADGET_STRING_H