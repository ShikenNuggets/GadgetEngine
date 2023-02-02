#include "Config.h"

#include "App.h"
#include "ConfigParser.h"
#include "LocManager.h"
#include "Core/FileSystem.h"
#include "Events/AppEvent.h"
#include "Events/EventHandler.h"
#include "Utils/Utils.h"

using namespace Gadget;

Config::Config() : engineConfigPath(CreateEngineConfigPath()), vars(){
#ifdef GADGET_DEBUG
	engineConfigPath = engineConfigFileName; //Makes debugging easier to store the file in the same directory
#endif //GADGET_DEBUG

	EventHandler::GetInstance()->SetEventCallback(EventType::WindowMoved, &OnEvent);
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, &OnEvent);

	LocManager* locMan = LocManager::GetInstance(); //Initialize Localization Manager
	locMan->AddLanguage(SID("ENG")); //TODO - Pull available languages from a config file

	ConfigParser::ParseConfigFile(engineConfigPath, vars);
	locMan->SetCurrentLanguage(GetOptionsString(EngineVars::Core::languageKey));
}

Config::~Config(){
	SaveConfigs();
}

Var Config::GetOption(StringID key_) const{
	return vars.GetValue(key_);
}

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
			App::GetInstance().GetConfig().SetOption(EngineVars::Display::sectionName, EngineVars::Display::displayWidthKey, dynamic_cast<const WindowResizedEvent&>(e_).GetWidth());
			App::GetInstance().GetConfig().SetOption(EngineVars::Display::sectionName, EngineVars::Display::displayHeightKey, dynamic_cast<const WindowResizedEvent&>(e_).GetHeight());
			break;
		case EventType::WindowMoved:
			App::GetInstance().GetConfig().SetOption(EngineVars::Display::sectionName, EngineVars::Display::lastWindowXKey, dynamic_cast<const WindowMovedEvent&>(e_).GetX());
			App::GetInstance().GetConfig().SetOption(EngineVars::Display::sectionName, EngineVars::Display::lastWindowYKey, dynamic_cast<const WindowMovedEvent&>(e_).GetY());
			break;
	}
}

void Config::SaveConfigs(){
	ConfigParser::SerializeConfigs(engineConfigPath, vars);
}

void Config::ResetAllOptionsToDefault(){
	vars = EngineVars();
}

std::string Config::CreateEngineConfigPath(){
	return FileSystem::GetPersistentDataDir() + FileSystem::PathSeparator + App::GetInstance().GetGameName() + FileSystem::PathSeparator + engineConfigFileName;
}