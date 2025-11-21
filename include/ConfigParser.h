#ifndef GADGET_CONFIG_PARSER_H
#define GADGET_CONFIG_PARSER_H

#include <map>
#include <string>

#include "EngineVars.h"
#include "GadgetEnums.h"
#include "Utils/StringID.h"

namespace Gadget{
	//Helper functions for reading from/writing to config files
	//Used by the Config system
	namespace ConfigParser{
		ErrorCode ParseConfigFile(const std::string& path_, EngineVars& vars_);
		ErrorCode SerializeConfigs(const std::string& path_, const EngineVars& vars_);

		std::string SerializeSection(StringID section_, const std::map<StringID, Var>& vars_);
	}
}

#endif //!GADGET_CONFIG_PARSER_H