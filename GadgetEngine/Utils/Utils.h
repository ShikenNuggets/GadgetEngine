#ifndef GADGET_UTILS_H
#define GADGET_UTILS_H

#include <algorithm>
#include <map>
#include <vector>

#include "StringID.h"

namespace Gadget{
	namespace Utils{
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
			return container_.find(key_) != container_.end();
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

		inline bool ContainsChar(const std::string& str_, char char_){
			return str_.find(char_) != std::string::npos;
		}

		//TODO - These are not unicode safe
		inline std::string ToUpper(std::string str_){
			std::transform(str_.begin(), str_.end(), str_.begin(), [](char c){ return static_cast<char>(std::toupper(c)); });
			return str_;
		}

		inline std::string ToLower(std::string str_){
			std::transform(str_.begin(), str_.end(), str_.begin(), [](char c){ return static_cast<char>(std::tolower(c)); });
			return str_;
		}

		inline std::string ExtractString(const std::string& str_, char beginAfterDelimeter, char endBeforeDelimeter){
			size_t first = str_.find_first_of(beginAfterDelimeter);
			size_t last = str_.find_last_of(endBeforeDelimeter);
			return str_.substr(first + 1, last - first - 1);
		}

		inline std::string Trim(const std::string& str_){
			std::string ret = str_;
			ret.erase(ret.begin(), std::find_if(ret.begin(), ret.end(), [](unsigned char ch){ return !std::isspace(ch); }));
			ret.erase(std::find_if(ret.rbegin(), ret.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), ret.end());

			return ret;
		}
	};
}

#endif //!GADGET_UTILS_H