#ifndef GADGET_CONFIG_PARSER_H
#define GADGET_CONFIG_PARSER_H

#include <map>
#include <string>

#include "Utils/StringID.h"

namespace Gadget{
	namespace ConfigParser{
		void ParseConfigFile(const std::string& path_, std::map<StringID, float>& floats_, std::map<StringID, bool>& bools_, std::map<StringID, StringID>& strings_);
		void SerializeConfigs(const std::string& path_, const std::map<StringID, float>& floats_, const std::map<StringID, bool>& bools_, const std::map<StringID, StringID>& strings_);
	}
}

#endif //!GADGET_CONFIG_PARSER_H