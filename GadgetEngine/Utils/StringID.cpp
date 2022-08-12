#include "StringID.h"

using namespace Gadget;

IDTable StringID::stringIdTable;

std::string StringID::GetString() const{
	return GetStringFromID(*this);
}