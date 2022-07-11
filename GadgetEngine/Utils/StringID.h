#ifndef GADGET_STRING_ID_H
#define GADGET_STRING_ID_H

#include <string>
#include <unordered_map>

namespace Gadget{
	struct StringID{
		uint32_t id;

		constexpr StringID(uint32_t id_);

		static std::unordered_map<uint32_t, std::string> stringIdTable; //TODO - Replace this with a custom hash table with a constexpr Find function (would allow us to intern strings at compile time)

		static StringID InternString(const std::string& str_);
	};
}

#endif //!GADGET_STRING_ID_H