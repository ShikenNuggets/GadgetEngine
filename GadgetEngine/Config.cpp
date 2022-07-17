#include "Config.h"

#include "LocManager.h"

using namespace Gadget;

Config::Config() : floatOptions(), stringOptions(){
	LocManager* locMan = LocManager::GetInstance(); //Initialize Localization Manager
	locMan->AddLanguage(StringID::InternString("ENG")); //TODO - Pull this from a config file

	//TODO - Load string/float options from config file
}

Config::~Config(){}

Config* Config::GetInstance(){
	if(instance == nullptr){
		instance = new Config();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void Config::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

float Config::GetOptionFloat(StringID key_) const{
	auto find = floatOptions.find(key_);
	if(find != floatOptions.end()){
		return find->second;
	}

	return 0.0f;
}

StringID Config::GetOptionsString(StringID key_) const{
	auto find = stringOptions.find(key_);
	if(find != stringOptions.end()){
		return find->second;
	}

	return StringID::InternString("");
}