#ifndef GADGET_STRING_ID_H
#define GADGET_STRING_ID_H

#include <string>
#include <unordered_map>

#include "Debug.h"
#include "Hash.h"

namespace Gadget{
	struct StringID{
		const uint64_t id;

		explicit constexpr StringID(uint64_t id_) : id(id_){}
		std::string GetString() const;

		constexpr inline bool operator ==(StringID a_) const{ return id == a_.id; }
		constexpr inline bool operator !=(StringID a_) const{ return id != a_.id; }
		constexpr inline bool operator >(StringID a_) const{ return id > a_.id; }
		constexpr inline bool operator <(StringID a_) const{ return id < a_.id; }
		constexpr inline bool operator >=(StringID a_) const{ return id >= a_.id; }
		constexpr inline bool operator <=(StringID a_) const{ return id <= a_.id; }

		static std::unordered_map<uint64_t, const char*> stringIdTable;

		//DON'T USE THIS DIRECTLY, USE THE SID() MACRO
		static StringID InternString(StringID sid_, const char* str_);
		static const char* GetStringFromID(StringID id_);
	};

	//User-defined literal - DON'T USE THIS DIRECTLY, USE THE SID() MACRO
	constexpr inline StringID operator "" _sid(const char* str_, size_t len_){ return StringID(Hash::CRC32(str_, len_)); }

	#define SID(str) StringID::InternString(str ""_sid, str)
}

#endif //!GADGET_STRING_ID_H