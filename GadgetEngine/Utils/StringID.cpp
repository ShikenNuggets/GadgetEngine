#include "StringID.h"

#include "Hash.h"

using namespace Gadget;

std::unordered_map<uint32_t, std::string> StringID::stringIdTable;

constexpr StringID::StringID(uint32_t id_) : id(id_){}

StringID StringID::InternString(const std::string& str_){
	StringID id = Hash::CRC32(str_.data(), str_.length());

	if(stringIdTable.find(id.id) == stringIdTable.end()){
		stringIdTable[id.id] = str_;
	}

	return id;
}