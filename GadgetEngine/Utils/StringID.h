#ifndef GADGET_STRING_ID_H
#define GADGET_STRING_ID_H

#include <string>
#include <unordered_map>

#include "Hash.h"

namespace Gadget{
	class StringID{
	public:
		static StringID None;

		explicit constexpr StringID(uint64_t id_) : id(id_){}
		void operator =(StringID a_){ id = a_.id; }

		std::string GetString() const;

		constexpr inline bool operator ==(StringID a_) const{ return id == a_.id; }
		constexpr inline bool operator !=(StringID a_) const{ return id != a_.id; }
		constexpr inline bool operator >(StringID a_) const{ return id > a_.id; }
		constexpr inline bool operator <(StringID a_) const{ return id < a_.id; }
		constexpr inline bool operator >=(StringID a_) const{ return id >= a_.id; }
		constexpr inline bool operator <=(StringID a_) const{ return id <= a_.id; }

		//DON'T USE THIS DIRECTLY, USE THE SID() MACRO
		static StringID InternString(StringID sid_, const char* str_);
		static const char* GetStringFromID(StringID id_);

		//Only use this if the SID() macro cannot be used
		static inline StringID ProcessString(const std::string& str_){
			return InternString(StringID(Hash::MurmurHash64A(str_.c_str(), str_.length())), str_.c_str());
		}

	private:
		static std::unordered_map<uint64_t, const char*> stringIdTable;

		uint64_t id;
	};
}

//User-defined literal - DON'T USE THIS DIRECTLY UNLESS YOU KNOW WHAT YOU'RE DOING, USE THE SID() MACRO
constexpr inline Gadget::StringID operator "" _sid(const char* str_, size_t len_){ return Gadget::StringID(Gadget::Hash::MurmurHash64A(str_, len_)); }

//Use this to create a hashed string ID and add it to the string database
#define SID(str) Gadget::StringID::InternString(str ""_sid, str)

#endif //!GADGET_STRING_ID_H