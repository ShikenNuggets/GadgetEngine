#ifndef GADGET_UTILS_SID_ARRAY_CACHE_H
#define GADGET_UTILS_SID_ARRAY_CACHE_H

#include "Data/Array.h"
#include "Data/String.h"
#include "Utils/StringID.h"

namespace Gadget{
	class SIDArrayCache{
	public:
		SIDArrayCache(const std::string& prefix_, const std::string& postfix_) : prefix(prefix_), postfix(postfix_), sids(){}

		StringID Get(int64_t index_){
			while(index_ >= sids.Size()){
				sids.Add(StringID::ProcessString(prefix + std::to_string(sids.Size()) + postfix));
			}

			return sids[index_];
		}

	private:
		std::string prefix;
		std::string postfix;
		Array<StringID> sids;
	};
}

#endif //!GADGET_UTILS_SID_ARRAY_CACHE_H