#ifndef GADGET_UTILS_H
#define GADGET_UTILS_H

#include <map>
#include <vector>

namespace Gadget{
	class Utils{
	public:
		template <class T>
		static inline constexpr bool Contains(const std::vector<T>& container_, const T& value_){
			for(const auto& v : container_){
				if(v == value_){
					return true;
				}
			}

			return false;
		}

		template <class T, class Y>
		static inline constexpr bool ContainsKey(const std::map<T, Y>& container_, const T& key_){
			return container_.find(key_) != container_.end();
		}

		template <class T, class Y>
		static inline constexpr bool ContainsValue(const std::map<T, Y>& container_, const Y& value_){
			for(const auto& v : container_){
				if(v.second == value_){
					return true;
				}
			}

			return false;
		}
	};
}

#endif //!GADGET_UTILS_H