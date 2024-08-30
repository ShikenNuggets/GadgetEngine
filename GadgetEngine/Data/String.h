#ifndef GADGET_STRING_H
#define GADGET_STRING_H

#include "Data/Array.h"
#include "Math/Math.h"

namespace Gadget{
	class String{
	public:
		String(const char* str_ = "") : data(Math::Clamp<int64_t>(16, std::numeric_limits<int64_t>::max(), std::strlen(str_))){
			data.Add('\0');
			Append(str_);
		}

		String(const Array<char>& strArray_) : data(strArray_.Size()){
			data.Add('\0');
			Append(strArray_);
		}

		explicit String(int64_t capacity_) : data(capacity_ + 1){
			data.Add('\0');
		}

		void Append(const char* str_){
			GADGET_BASIC_ASSERT(HasNullTerminator());

			data.Reserve(data.Size() + 1);

			data.Pop(); //Pop the null terminator
			data.Add(str_, std::strlen(str_));
			data.Add('\0');
		}

		void Append(char c){
			GADGET_BASIC_ASSERT(HasNullTerminator());

			data.Reserve(data.Size() + 1);

			data.Pop();
			data.Add(c);
			data.Add('\0');
		}

		void Append(const String& str_){
			GADGET_BASIC_ASSERT(str_.HasNullTerminator());
			Append(str_.Value());
		}

		void Append(const Array<char>& strArray_){
			GADGET_BASIC_ASSERT(HasNullTerminator());

			data.Pop();
			data.Add(strArray_);
			data.Add('\0');
		}

		void Append(int32_t number){
			char buffer[10]{};
			itoa(number, buffer, 10);
			Append(buffer);
		}

		void Append(const Array<String>& strs_){
			//This technically has higher time complexity than just doing one loop,
			//but doing only one memory allocation is worth it if we're appending lots of strings/large strings
			int64_t newLength = Length();
			for(int64_t i = 0; i < strs_.Size(); i++){
				newLength += strs_[i].Length();
			}

			data.Reserve(newLength);

			for(int64_t i = 0; i < strs_.Size(); i++){
				Append(strs_[i].Value());
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

		void operator+=(const String& str_){ *this = *this + str_; }
		void operator+=(char c_){ *this = *this + c_; }
		void operator+=(int32_t number_){ *this = *this + number_; }

		void InsertAt(int64_t index_, char c){
			if(index_ >= Length()){
				Append(c);
				return;
			}

			data.InsertAt(index_, c);
		}

		constexpr void RemoveAt(int64_t index_){
			if(index_ >= Length()){
				return;
			}

			data.RemoveAt(index_);
		}

		constexpr void Remove(char value_){
			GADGET_BASIC_ASSERT(value_ != '\0'); //Do not remove the null terminator
			data.Remove(value_);
		}

		constexpr void RemoveAll(char value_){
			GADGET_BASIC_ASSERT(value_ != '\0'); //Do not remove the null terminator
			data.RemoveAll(value_);
		}

		void Reserve(int64_t capacity){
			data.Reserve(capacity + 1); //Make sure there's room for the null terminator!
		}

		void ShrinkToFit(){
			data.ShrinkToFit();
		}

		static constexpr inline bool IsWhitespace(char c){
			return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
		}
		
		void Trim(){
			int64_t whitespaceAtStart;
			for(whitespaceAtStart = 0; whitespaceAtStart < Length(); whitespaceAtStart++){
				if(!IsWhitespace(data[whitespaceAtStart])){
					break;
				}
			}

			data.RemoveAt(0, whitespaceAtStart);

			GADGET_BASIC_ASSERT(data.Size() >= 1);

			if(Length() > 0){
				GADGET_BASIC_ASSERT(data.Size() < std::numeric_limits<int64_t>::max()); //I doubt this will ever be an issue, but just in case
				int64_t indexToEndWhitespace;
				for(indexToEndWhitespace = data.Size() - 2; indexToEndWhitespace >= 0; indexToEndWhitespace--){
					if(!IsWhitespace(data[indexToEndWhitespace])){
						break;
					}
				}

				data.RemoveAt(indexToEndWhitespace + 1, data.Size());
				data.Add('\0');
			}
		}

		void FindAndReplace(char find_, const String& replace_){
			GADGET_BASIC_ASSERT(find_ != '\0'); //Do not try to replace the null terminator

			for(int64_t i = 0; i < Length(); i++){
				if(data[i] == find_){
					RemoveAt(i);

					for(int64_t j = 0; j < replace_.Length(); j++){
						InsertAt(i + j, replace_[j]);
					}
				}
			}
		}

		constexpr void ToLower(){
			for(int64_t i = 0; i < Length(); i++){
				data[i] = static_cast<char>(std::tolower(data[i]));
			}
		}

		constexpr void ToUpper(){
			for(int64_t i = 0; i < Length(); i++){
				data[i] = static_cast<char>(std::toupper(data[i]));
			}
		}

		constexpr inline int64_t Find(char c_, int64_t startPos_ = 0) const{ return data.Find(c_, startPos_); }

		constexpr inline bool Contains(char c) const{ return data.Contains(c); }

		//TODO - Naive brute force implementation. Consider other options: https://en.wikipedia.org/wiki/String-searching_algorithm#Single-pattern_algorithms
		bool Contains(const String& str_) const{
			if(data.Size() < str_.Length()){
				return false;
			}

			for(int64_t i = 0; i < data.Size(); i++){
				bool isEqual = true;

				int64_t j = i;
				for(int64_t k = 0; k < str_.Length(); j++, k++){
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

		String SubString(int64_t startIndex_, int64_t endIndex_) const{
			GADGET_BASIC_ASSERT(startIndex_ != endIndex_);
			return String(data.SubRange(startIndex_, endIndex_));
		}

		void QuickSort(){
			GADGET_BASIC_ASSERT(HasNullTerminator());

			data.Pop(); //We typically don't want to sort the null terminator
			data.QuickSort();
			data.Add('\0');
		}

		constexpr const char* Value() const{
			if(Length() == 0){
				return "";
			}

			return &data[0];
		}

		bool operator==(const char* str_) const{
			if(Length() != static_cast<int64_t>(std::strlen(str_))){
				return false;
			}

			for(int64_t i = 0; i < Length(); i++){
				if(data[i] != str_[i]){
					return false;
				}
			}

			return true;
		}

		constexpr bool operator==(const String& str_) const{
			return data == str_.data;
		}

		constexpr const char& operator[](int64_t i_) const{
			GADGET_BASIC_ASSERT(i_ < data.Size());
			return data[i_];
		}

		constexpr char& operator[](int64_t i_){
			GADGET_BASIC_ASSERT(i_ < data.Size());
			return data[i_];
		}

		constexpr int64_t Length() const{ return data.Size() - 1; }

		constexpr bool IsEmpty() const{
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

		constexpr inline bool HasNullTerminator() const{
			return !data.IsEmpty() && data[Length()] == '\0';
		}
	};
}

inline std::ostream& operator<<(std::ostream& os_, const Gadget::String& str_){
	os_ << str_.Value();
	return os_;
}

#endif //!GADGET_STRING_H