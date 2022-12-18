#include "ConfigParser.h"

#include <algorithm>

#include "Debug.h"
#include "Core/FileSystem.h"
#include "Math/Math.h"
#include "Utils/Utils.h"

using namespace Gadget;

void ConfigParser::ParseConfigFile(const std::string& path_, EngineVars& vars_){
	if(!FileSystem::FileExists(path_)){
		return;
	}

	StringID currentSection = StringID::None;
	
	const auto& file = FileSystem::ReadFile(path_);
	for(const auto& line : file){
		if(Utils::ContainsChar(line, '[')){
			currentSection = StringID::ProcessString(Utils::ExtractString(line, '[', ']'));
		}

		if(currentSection == StringID::None || !Utils::ContainsChar(line, '=')){
			continue;
		}

		std::string keyStr = line.substr(0, line.find('='));
		std::string valStr = line.substr(line.find('=') + 1, line.length());

		StringID keyID = StringID::ProcessString(keyStr);
		Var value = Var::StringToVar(valStr);

		vars_.SetValue(currentSection, keyID, value);
	}
}

void ConfigParser::SerializeConfigs(const std::string& path_, const EngineVars& vars_){
	std::string output = "";
	output += SerializeSection(EngineVars::Core::sectionName, vars_.core.vars);
	output += SerializeSection(EngineVars::Display::sectionName, vars_.display.vars);

	FileSystem::WriteToFile(path_, Utils::Trim(output), FileSystem::WriteType::Overwrite);
}

std::string ConfigParser::SerializeSection(StringID section_, const std::map<StringID, Var>& vars_){
	std::vector<std::string> outputStrs;
	outputStrs.reserve(vars_.size());

	for(const auto& v : vars_){
		switch(v.second.GetType()){
			case Var::Type::String:
				outputStrs.push_back(v.first.GetString() + "=" + v.second.ToStr().GetString());
				break;
			case Var::Type::Bool:
				if(v.second.ToBool() == true){
					outputStrs.push_back(v.first.GetString() + "=" + "TRUE");
				}else{
					outputStrs.push_back(v.first.GetString() + "=" + "FALSE");
				}
				break;
			case Var::Type::Number:
				if(Math::IsInteger(v.second.ToNumber())){
					outputStrs.push_back(v.first.GetString() + "=" + std::to_string(static_cast<int64_t>(v.second.ToNumber())));
				}else{
					outputStrs.push_back(v.first.GetString() + "=" + std::to_string(v.second.ToNumber()));
				}
				break;
			default:
				GADGET_ASSERT(false, "Unhandled Var type!");
				break;
		}
	}

	std::sort(outputStrs.begin(), outputStrs.end(), [](const std::string& a, const std::string& b){return a < b; });

	std::string output;
	output += "[" + section_.GetString() + "]" + "\n";
	
	for(const auto& str : outputStrs){
		output += str + "\n";
	}

	output += "\n";
	return output;
}