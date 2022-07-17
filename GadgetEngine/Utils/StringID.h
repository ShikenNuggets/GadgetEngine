#ifndef GADGET_STRING_ID_H
#define GADGET_STRING_ID_H

#include <string>
#include <unordered_map>

namespace Gadget{
	struct StringID{
		const uint32_t id;

		explicit constexpr StringID(uint32_t id_);
		std::string GetString() const;

		static std::unordered_map<uint32_t, std::string> stringIdTable; //TODO - Replace this with a custom hash table with a constexpr Find function (would allow us to intern strings at compile time)

		static StringID InternString(const std::string& str_); //TODO - Once this can be constexpr, connect it to a user-defined literal
		static std::string GetStringFromID(StringID id_);

		constexpr inline bool operator ==(StringID a_) const{ return id == a_.id; }
		constexpr inline bool operator !=(StringID a_) const{ return id != a_.id; }
		constexpr inline bool operator >(StringID a_) const{ return id > a_.id; }
		constexpr inline bool operator <(StringID a_) const{ return id < a_.id; }
		constexpr inline bool operator >=(StringID a_) const{ return id >= a_.id; }
		constexpr inline bool operator <=(StringID a_) const{ return id <= a_.id; }
	};
}

#endif //!GADGET_STRING_ID_H