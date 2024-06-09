#ifndef GADGET_STRING_H
#define GADGET_STRING_H

#include "Data/Array.h"
#include "Math/Math.h"

namespace Gadget{
	class String{
	public:
		String(const char* str_ = "") : data(Math::Clamp<size_t>(16, std::numeric_limits<size_t>::max(), std::strlen(str_))){
			Append(str_);
		}

		void Append(const char* str_){
			if(!data.IsEmpty()){
				GADGET_BASIC_ASSERT(data[data.Size() - 1] == '\0');
			}

			data.Pop(); //Pop the null terminator

			for(size_t i = 0; i < std::strlen(str_); i++){
				data.Add(str_[i]);
			}

			data.Add('\0');
		}

		void Append(const String& str_){
			Append(str_.Value());
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
				for(size_t k = 0; k < str_.Length() - 1; j++, k++){ //Length() - 1 because of the null terminator
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

		constexpr const char& operator[](size_t i_) const{
			GADGET_BASIC_ASSERT(i_ < data.Size());
			return data[i_];
		}

		char& operator[](size_t i_){
			GADGET_BASIC_ASSERT(i_ < data.Size());
			return data[i_];
		}

		constexpr size_t Length() const{ return data.Size(); }

	private:
		Array<char> data;
	};
}

#endif //!GADGET_STRING_H