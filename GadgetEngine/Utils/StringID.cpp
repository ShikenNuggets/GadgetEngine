#include "StringID.h"

#include "Debug.h"

using namespace Gadget;

std::unordered_map<uint64_t, const char*> StringID::stringIdTable;
StringID StringID::None = SID("");

std::string StringID::GetString() const{
	return GetStringFromID(*this);
}

StringID StringID::InternString(StringID sid_, const char* str_){
	if(stringIdTable.find(sid_.id) == stringIdTable.end()){
		stringIdTable[sid_.id] = str_;
	}

	GADGET_ASSERT(strcmp(stringIdTable[sid_.id], str_) == 0, "StringID [" + std::to_string(sid_.id) + "] has encountered a conflict: \"" + stringIdTable[sid_.id] + "\" and \"" + str_ + "\" have the same hash!");

	return sid_;
}

const char* StringID::GetStringFromID(StringID id_){
	auto find = stringIdTable.find(id_.id);
	if(find != stringIdTable.end()){
		return stringIdTable[id_.id];
	}

	return "";
}