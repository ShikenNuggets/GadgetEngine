#ifndef GADGET_UTILS_H
#define GADGET_UTILS_H

//----- ONLY include standard library headers here!!! DO NOT include GadgetEngine code!!! -----
//A LOT of engine code depends on this header, so it would quickly become circular dependency city

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <map>
#include <vector>

//--------------------------------------------------------------------
//Undefine some "standard" macros that tend to cause problems
#undef min
#undef max
//--------------------------------------------------------------------

#ifndef DISABLE_COPY
#define DISABLE_COPY(T)						\
			explicit T(const T&) = delete;	\
			T& operator=(const T&) = delete;
#endif //!DISABLE_COPY

#ifndef DISABLE_MOVE
#define DISABLE_MOVE(T)					\
			explicit T(T&&) = delete;	\
			T& operator=(T&&) = delete;
#endif //!DISABLE_MOVE

#ifndef DISABLE_COPY_AND_MOVE
#define DISABLE_COPY_AND_MOVE(T) DISABLE_COPY(T) DISABLE_MOVE(T)
#endif //!DISABLE_COPY_AND_MOVE

#ifndef DISABLE_DEFAULT_CONSTRUCTOR
#define DISABLE_DEFAULT_CONSTRUCTOR(T) T() = delete
#endif //!DISABLE_DEFAULT_CONSTRUCTOR

#ifndef STATIC_CLASS
#define STATIC_CLASS(T) DISABLE_COPY_AND_MOVE(T) DISABLE_DEFAULT_CONSTRUCTOR(T)
#endif //!STATIC_CLASS

namespace Gadget{
	namespace Utils{
		//----------------------------------------------------------------------------------------------------//
		//---------- Containers ------------------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//
		template <class T>
		inline constexpr bool Contains(const std::vector<T>& container_, const T& value_){
			for(const auto& v : container_){
				if(v == value_){
					return true;
				}
			}

			return false;
		}

		template <class T, class Y>
		inline constexpr bool ContainsKey(const std::map<T, Y>& container_, const T& key_){
			return !container_.empty() && container_.find(key_) != container_.end();
		}

		template <class T, class Y>
		inline constexpr bool ContainsValue(const std::map<T, Y>& container_, const Y& value_){
			for(const auto& v : container_){
				if(v.second == value_){
					return true;
				}
			}

			return false;
		}

		template <class T, class Y>
		inline constexpr std::vector<T> GetAllKeys(const std::map<T, Y>& container_){
			std::vector<T> output;

			for(const auto& i : container_){
				output.push_back(i.first);
			}

			return output;
		}

		//----------------------------------------------------------------------------------------------------//
		//---------- Strings ---------------------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//
		//TODO - Pretty much all of these functions assume ASCII. Make sure they work for UTF-8 too!
		inline constexpr bool ContainsChar(const std::string& str_, char char_){
			return str_.find(char_) != std::string::npos;
		}

		inline std::string ToUpper(std::string str_){
			std::transform(str_.begin(), str_.end(), str_.begin(), [](char c){ return static_cast<char>(std::toupper(c)); });
			return str_;
		}

		inline std::string ToLower(std::string str_){
			std::transform(str_.begin(), str_.end(), str_.begin(), [](char c){ return static_cast<char>(std::tolower(c)); });
			return str_;
		}

		inline constexpr std::string ExtractString(const std::string& str_, char beginAfterDelimeter, char endBeforeDelimeter){
			size_t first = str_.find_first_of(beginAfterDelimeter);
			size_t last = str_.find_last_of(endBeforeDelimeter);
			return str_.substr(first + 1, last - first - 1);
		}

		inline constexpr std::string Trim(const std::string& str_){
			std::string ret = str_;
			ret.erase(ret.begin(), std::find_if(ret.begin(), ret.end(), [](unsigned char ch){ return !std::isspace(ch); }));
			ret.erase(std::find_if(ret.rbegin(), ret.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), ret.end());

			return ret;
		}

		inline bool StringIsNumber(const std::string& str_){
			try{
				[[maybe_unused]] double value = std::stod(str_);
				return true;
			}catch(const std::invalid_argument&){
				return false;
			}catch(const std::out_of_range&){
				return false;
			}
		}

		inline double StringToNumber(const std::string& str_){
			try{
				return std::stod(str_);
			}catch(const std::invalid_argument&){
				return 0.0;
			}catch(const std::out_of_range&){
				return 0.0;
			}
		}

		inline constexpr std::wstring ToWString(const std::string& s_){ return { s_.begin(), s_.end() }; }
		inline constexpr std::wstring ToWString(const char* s_){ return ToWString(std::string(s_)); }

		//----------------------------------------------------------------------------------------------------//
		//---------- Binary ----------------------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//
		constexpr inline uint16_t MergeBytes(uint8_t a_, uint8_t b_, bool littleEndian_ = (std::endian::native == std::endian::little)){
			if(littleEndian_){
				return (b_ << 8) | a_;
			}else{
				return (a_ << 8) | b_;
			}
		}

		constexpr inline uint32_t MergeBytes(uint8_t a_, uint8_t b_, uint8_t c_, uint8_t d_, bool littleEndian_ = (std::endian::native == std::endian::little)){
			if(littleEndian_){
				return (d_ << 24) | (c_ << 16) | (b_ << 8) | a_;
			}else{
				return (a_ << 24) | (b_ << 16) | (c_ << 8) | d_;
			}
		}

		constexpr inline uint64_t MergeBytes(uint8_t a_, uint8_t b_, uint8_t c_, uint8_t d_, uint8_t e_, uint8_t f_, uint8_t g_, uint8_t h_, bool littleEndian_ = (std::endian::native == std::endian::little)){
			if(littleEndian_){
				return (
					(static_cast<uint64_t>(h_) << 56)
					| (static_cast<uint64_t>(g_) << 48)
					| (static_cast<uint64_t>(f_) << 40)
					| (static_cast<uint64_t>(e_) << 32)
					| (static_cast<uint64_t>(d_) << 24)
					| (static_cast<uint64_t>(c_) << 16)
					| (static_cast<uint64_t>(b_) << 8)
					| static_cast<uint64_t>(a_)
				);
			}else{
				return (
					(static_cast<uint64_t>(a_) << 56)
					| (static_cast<uint64_t>(b_) << 48)
					| (static_cast<uint64_t>(c_) << 40)
					| (static_cast<uint64_t>(d_) << 32)
					| (static_cast<uint64_t>(e_) << 24)
					| (static_cast<uint64_t>(f_) << 16)
					| (static_cast<uint64_t>(g_) << 8)
					| static_cast<uint64_t>(h_)
				);
			}
		}

		constexpr inline uint16_t MergeTwoBytes(const std::vector<uint8_t>& buffer_, size_t offset_, bool littleEndian_ = (std::endian::native == std::endian::little)){
			if(offset_ >= buffer_.size()){
				return 0;
			}

			return MergeBytes(buffer_[offset_], buffer_[offset_ + 1], littleEndian_);
		}

		constexpr inline uint32_t MergeFourBytes(const std::vector<uint8_t>& buffer_, size_t offset_, bool littleEndian_ = (std::endian::native == std::endian::little)){
			if(offset_ >= buffer_.size()){
				return 0;
			}

			return MergeBytes(buffer_[offset_], buffer_[offset_ + 1], buffer_[offset_ + 2], buffer_[offset_ + 3], littleEndian_);
		}

		constexpr inline uint64_t MergeEightBytes(const std::vector<uint8_t>& buffer_, size_t offset_, bool littleEndian_ = (std::endian::native == std::endian::little)){
			if(offset_ >= buffer_.size()){
				return 0;
			}

			return MergeBytes(buffer_[offset_], buffer_[offset_ + 1], buffer_[offset_ + 2], buffer_[offset_ + 3], buffer_[offset_ + 4], buffer_[offset_ + 5], buffer_[offset_ + 6], buffer_[offset_ + 7], littleEndian_);
		}

		constexpr inline bool HasFlag(uint8_t byte_, uint8_t flag_){
			return (byte_ & flag_);
		}

		//----------------------------------------------------------------------------------------------------//
		//---------- Memory ----------------------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//
		template<uint64_t alignment>
		constexpr uint64_t AlignSizeUp(uint64_t size_){
			static_assert(alignment > 0, "Alignment must be non-zero");
			constexpr uint64_t mask = alignment - 1;
			static_assert(!(alignment & mask), "Alignment must be ^2");

			return (size_ + mask) & ~mask;
		}

		template<uint64_t alignment>
		constexpr uint64_t AlignSizeDown(uint64_t size_){
			static_assert(alignment > 0, "Alignment must be non-zero");
			constexpr uint64_t mask = alignment - 1;
			static_assert(!(alignment & mask), "Alignment must be ^2");

			return size_ & ~mask;
		}

		//----------------------------------------------------------------------------------------------------//
		//---------- Misc ------------------------------------------------------------------------------------//
		//----------------------------------------------------------------------------------------------------//
		inline std::string GetCurrentDateAndTimeString(){
			return std::format("{:%Y-%m-%d %X}", std::chrono::system_clock::now());
		}
	};
}

#endif //!GADGET_UTILS_H