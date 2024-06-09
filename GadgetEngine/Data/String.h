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