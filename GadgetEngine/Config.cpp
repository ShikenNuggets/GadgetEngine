#include "Config.h"

#include "ConfigParser.h"
#include "LocManager.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Utils/Utils.h"

using namespace Gadget;

Config* Config::instance = nullptr;

Config::Config() : vars(){
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowMoved, &OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, &OnEvent);

	LocManager* locMan = LocManager::GetInstance(); //Initialize Localization Manager
	locMan->AddLanguage(SID("ENG")); //TODO - Pull available languages from a config file

	ConfigParser::ParseConfigFile(engineConfigFile, vars);
	locMan->SetCurrentLanguage(GetOptionsString(EngineVars::Core::languageKey));
}

Config::~Config(){
	SaveConfigs();
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

double Config::GetOptionFloat(StringID key_) const{
	return vars.GetValue(key_).ToNumber();
}

bool Config::GetOptionsBool(StringID key_) const{
	return vars.GetValue(key_).ToBool();
}

StringID Config::GetOptionsString(StringID key_) const{
	return vars.GetValue(key_).ToStr();
}

void Config::SetOption(StringID section_, StringID key_, int32_t value_){ SetOption(section_, key_, static_cast<double>(value_)); }
void Config::SetOption(StringID section_, StringID key_, int64_t value_){ SetOption(section_, key_, static_cast<double>(value_)); }
void Config::SetOption(StringID section_, StringID key_, uint32_t value_){ SetOption(section_, key_, static_cast<double>(value_)); }
void Config::SetOption(StringID section_, StringID key_, uint64_t value_){ SetOption(section_, key_, static_cast<double>(value_)); }

void Config::SetOption(StringID section_, StringID key_, double value_){
	vars.SetValue(section_, key_, value_);
}

void Config::SetOption(StringID section_, StringID key_, bool value_){
	vars.SetValue(section_, key_, value_);
}

void Config::SetOption(StringID section_, StringID key_, StringID value_){
	vars.SetValue(section_, key_, value_);
}

void Config::OnEvent(const Event& e_){
	switch(e_.GetEventType()){
		case EventType::WindowResize:
			GetInstance()->SetOption(EngineVars::Display::sectionName, EngineVars::Display::displayWidthKey, dynamic_cast<const WindowResizedEvent&>(e_).GetWidth());
			GetInstance()->SetOption(EngineVars::Display::sectionName, EngineVars::Display::displayHeightKey, dynamic_cast<const WindowResizedEvent&>(e_).GetHeight());
			break;
		case EventType::WindowMoved:
			GetInstance()->SetOption(EngineVars::Display::sectionName, EngineVars::Display::lastWindowXKey, dynamic_cast<const WindowMovedEvent&>(e_).GetX());
			GetInstance()->SetOption(EngineVars::Display::sectionName, EngineVars::Display::lastWindowYKey, dynamic_cast<const WindowMovedEvent&>(e_).GetY());
			break;
	}
}

void Config::SaveConfigs(){
	ConfigParser::SerializeConfigs(engineConfigFile, vars);
}

void Config::ResetAllOptionsToDefault(){
	vars = EngineVars();
}