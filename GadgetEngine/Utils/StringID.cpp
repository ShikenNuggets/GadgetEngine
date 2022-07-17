#include "StringID.h"

#include "Hash.h"

using namespace Gadget;

std::unordered_map<uint32_t, std::string> StringID::stringIdTable;

constexpr StringID::StringID(uint32_t id_) : id(id_){}

StringID StringID::InternString(const std::string& str_){
	StringID id = StringID(Hash::CRC32(str_.data(), str_.length()));

	if(stringIdTable.find(id.id) == stringIdTable.end()){
		stringIdTable[id.id] = str_;
	}

	return id;
}

constexpr bool StringID::operator ==(StringID id_) const{ return id == id_.id; }
constexpr bool StringID::operator !=(StringID a_) const{ return id != a_.id; }
constexpr bool StringID::operator >(StringID a_) const{ return id > a_.id; }
constexpr bool StringID::operator <(StringID a_) const{ return id < a_.id; }
constexpr bool StringID::operator >=(StringID a_) const{ return id >= a_.id; }
constexpr bool StringID::operator <=(StringID a_) const{ return id <= a_.id; }