#include "ConfigParser.h"

#include <algorithm>

#include "Core/FileSystem.h"
#include "Utils/Utils.h"

using namespace Gadget;

void ConfigParser::ParseConfigFile(const std::string& path_, std::map<StringID, float>& floats_, std::map<StringID, bool>& bools_, std::map<StringID, StringID>& strings_){
	if(!FileSystem::FileExists(path_)){
		return;
	}
	
	const auto& file = FileSystem::ReadFile(path_);
	for(const auto& line : file){
		if(!Utils::ContainsChar(line, '=')){
			continue;
		}

		std::string key = line.substr(0, line.find('='));
		std::string value = line.substr(line.find('=') + 1, line.length());

		StringID keyID = StringID::ProcessString(key);

		//Bool
		std::string upper = Utils::ToUpper(value);
		if(upper == "TRUE"){
			bools_[keyID] = true;
		}else if(upper == "FALSE"){
			bools_[keyID] = false;
		}

		//Number
		//TODO - This might be a problem if we have a string option later that contains a number
		else if(std::find_if(value.begin(), value.end(), ::isdigit) != value.end()){
			floats_[keyID] = std::stof(value);
		}

		//If all else fails, just treat it as a string
		else{
			//Can't use the [] operator since StringID has no default constructor
			if(strings_.find(keyID) != strings_.end()){
				strings_.at(keyID) = StringID::ProcessString(value);
			}else{
				strings_.emplace(keyID, StringID::ProcessString(value));
			}
		}
	}
}

void ConfigParser::SerializeConfigs(const std::string& path_, const std::map<StringID, float>& floats_, const std::map<StringID, bool>& bools_, const std::map<StringID, StringID>& strings_){
	std::string output = "";
	for(const auto& f : floats_){
		output += f.first.GetString() + "=" + std::to_string(f.second) + "\n";
	}

	for(const auto& b : bools_){
		std::string value = "";
		if(b.second){
			value = "TRUE";
		}else{
			value = "FALSE";
		}

		output += b.first.GetString() + "=" + value + "\n";
	}

	for(const auto& s : strings_){
		output += s.first.GetString() + "=" + s.second.GetString() + "\n";
	}

	FileSystem::WriteToFile(path_, output, FileSystem::WriteType::Overwrite);
}