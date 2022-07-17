#include "StringID.h"

#include "Hash.h"

using namespace Gadget;

std::unordered_map<uint32_t, std::string> StringID::stringIdTable;

constexpr StringID::StringID(uint32_t id_) : id(id_){}

std::string StringID::GetString() const{
	return GetStringFromID(*this);
}

StringID StringID::InternString(const std::string& str_){
	StringID id = StringID(Hash::CRC32(str_.data(), str_.length()));

	if(stringIdTable.find(id.id) == stringIdTable.end()){
		stringIdTable[id.id] = str_;
	}

	return id;
}

std::string StringID::GetStringFromID(StringID id_){
	auto find = stringIdTable.find(id_.id);
	if(find != stringIdTable.end()){
		return find->second;
	}

	return "";
}

bool StringID::operator ==(StringID a_) const{ return id == a_.id; }
bool StringID::operator !=(StringID a_) const{ return id != a_.id; }
bool StringID::operator >(StringID a_) const{ return id > a_.id; }
bool StringID::operator <(StringID a_) const{ return id < a_.id; }
bool StringID::operator >=(StringID a_) const{ return id >= a_.id; }
bool StringID::operator <=(StringID a_) const{ return id <= a_.id; }