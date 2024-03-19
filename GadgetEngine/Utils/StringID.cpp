#include "StringID.h"

#include "Debug.h"

using namespace Gadget;

const StringID StringID::None = SID("");

std::string StringID::GetString() const{
	return GetStringFromID(*this);
}

StringID StringID::InternString(StringID sid_, const char* str_){
	auto& stringIdTable = StringIDTable();

	if(stringIdTable.find(sid_.id) == stringIdTable.end()){
		stringIdTable[sid_.id] = str_;
	}

	GADGET_ASSERT(stringIdTable[sid_.id] == str_, "StringID [" + std::to_string(sid_.id) + "] has encountered a conflict: \"" + stringIdTable[sid_.id] + "\" and \"" + str_ + "\" have the same hash!");

	return sid_;
}

std::string StringID::GetStringFromID(StringID id_){
	return GetStringFromID(id_.GetID());
}

std::string StringID::GetStringFromID(uint64_t id_){
	auto& stringIdTable = StringIDTable();

	auto find = stringIdTable.find(id_);
	if(find != stringIdTable.end()){
		return stringIdTable[id_];
	}

	return "";
}