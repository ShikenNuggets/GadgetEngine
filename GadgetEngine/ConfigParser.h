#ifndef GADGET_CONFIG_PARSER_H
#define GADGET_CONFIG_PARSER_H

#include <map>
#include <string>

#include "EngineVars.h"
#include "Utils/StringID.h"

namespace Gadget{
	namespace ConfigParser{
		void ParseConfigFile(const std::string& path_, EngineVars& vars_);
		void SerializeConfigs(const std::string& path_, const EngineVars& vars_);

		std::string SerializeSection(StringID section_, const std::map<StringID, Var>& vars_);
	}
}

#endif //!GADGET_CONFIG_PARSER_H