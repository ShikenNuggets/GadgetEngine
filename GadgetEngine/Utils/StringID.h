#ifndef GADGET_STRING_ID_H
#define GADGET_STRING_ID_H

#include <string>
#include <unordered_map>

#include "Hash.h"

namespace Gadget{
	struct StringID{
		uint64_t id;

		explicit constexpr StringID(uint64_t id_) : id(id_){}
		void operator =(StringID a_){ id = a_.id; }

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
}

//User-defined literal - DON'T USE THIS DIRECTLY, USE THE SID() MACRO
constexpr inline Gadget::StringID operator "" _sid(const char* str_, size_t len_){ return Gadget::StringID(Gadget::Hash::MurmurHash64A(str_, len_)); }

//Use this to create a hashed string ID and add it to the string database
#define SID(str) Gadget::StringID::InternString(str ""_sid, str)

#endif //!GADGET_STRING_ID_H