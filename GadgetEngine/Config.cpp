#include "Config.h"

#include "ConfigParser.h"
#include "LocManager.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Utils/Utils.h"

using namespace Gadget;

Config* Config::instance = nullptr;

const StringID Config::LanguageKey =			SID("Language");
const StringID Config::FullscreenKey =			SID("Fullscreen");
const StringID Config::WidthFullscreenKey =		SID("Width_Fullscreen");
const StringID Config::HeightFullscreenKey =	SID("Height_Fullscreen");
const StringID Config::WidthWindowedKey =		SID("Width_Windowed");
const StringID Config::HeightWindowedKey =		SID("Height_Windowed");
const StringID Config::LastWindowX =			SID("LastWindowX");
const StringID Config::LastWindowY =			SID("LastWindowY");

Config::Config() : floatOptions(), boolOptions(), stringOptions(){
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowMoved, &OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, &OnEvent);

	LocManager* locMan = LocManager::GetInstance(); //Initialize Localization Manager
	locMan->AddLanguage(SID("ENG")); //TODO - Pull available languages from a config file

	SetDefaultEngineConfigs();

	ConfigParser::ParseConfigFile(engineConfigFile, floatOptions, boolOptions, stringOptions);
	locMan->SetCurrentLanguage(GetOptionsString(LanguageKey));
}

Config::~Config(){
	ConfigParser::SerializeConfigs(engineConfigFile, floatOptions, boolOptions, stringOptions);
}

void Config::SetDefaultEngineConfigs(){
	SetOption(LanguageKey, SID("ENG"));
	SetOption(FullscreenKey, false);
	SetOption(WidthFullscreenKey, 1920);
	SetOption(HeightFullscreenKey, 1080);
	SetOption(WidthWindowedKey, 1920);
	SetOption(HeightWindowedKey, 1080);
	SetOption(LastWindowX, 0);
	SetOption(LastWindowY, 0);
}

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

bool Config::GetOptionsBool(StringID key_) const{
	auto find = floatOptions.find(key_);
	if(find != floatOptions.end()){
		return find->second;
	}

	return false;
}

StringID Config::GetOptionsString(StringID key_) const{
	auto find = stringOptions.find(key_);
	if(find != stringOptions.end()){
		return find->second;
	}

	return SID("");
}

void Config::SetOption(StringID key_, int value_){
	SetOption(key_, (float)value_);
}

void Config::SetOption(StringID key_, float value_){
	floatOptions[key_] = value_;
}

void Config::SetOption(StringID key_, bool value_){
	boolOptions[key_] = value_;
}

void Config::SetOption(StringID key_, StringID value_){
	//Need to do this more explicitly, [] operator does not work since StringID doesn't have a default constructor
	if(stringOptions.find(key_) != stringOptions.end()){
		stringOptions.at(key_) = value_;
	}else{
		stringOptions.emplace(key_, value_);
	}
}

void Config::OnEvent(const Event& e_){
	switch(e_.GetEventType()){
		case EventType::WindowResize:
			GetInstance()->SetOption(WidthWindowedKey, dynamic_cast<const WindowResizedEvent&>(e_).GetWidth());
			GetInstance()->SetOption(HeightWindowedKey, dynamic_cast<const WindowResizedEvent&>(e_).GetHeight());
			break;
		case EventType::WindowMoved:
			GetInstance()->SetOption(LastWindowX, dynamic_cast<const WindowMovedEvent&>(e_).GetX());
			GetInstance()->SetOption(LastWindowY, dynamic_cast<const WindowMovedEvent&>(e_).GetY());
			break;
	}
}