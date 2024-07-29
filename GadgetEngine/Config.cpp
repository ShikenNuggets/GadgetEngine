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
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowMoved, [&](const Event& e){ OnWindowMovedEvent(e); });
	EventHandler::GetInstance()->SetEventCallback(EventType::WindowResize, [&](const Event& e){ OnWindowResizedEvent(e); });

	LocManager* locMan = LocManager::GetInstance(); //Initialize Localization Manager
	GADGET_BASIC_ASSERT(locMan != nullptr);
	locMan->AddLanguage(SID("ENG")); //TODO - Pull available languages from a config file

	ConfigParser::ParseConfigFile(engineConfigPath, vars);
	locMan->SetCurrentLanguage(GetOptionString(EngineVars::Core::languageKey));

	SaveConfigs(); //Force a save in case the file doesn't exist yet
}

Config::~Config(){
	SaveConfigs();

#ifdef GADGET_DEBUG
	LocManager::DeleteInstance();
#endif //GADGET_DEBUG
}

Var Config::GetOption(StringID key_) const{
	return vars.GetValue(key_);
}

double Config::GetOptionFloat(StringID key_) const{
	return vars.GetValue(key_).ToNumber();
}

bool Config::GetOptionBool(StringID key_) const{
	return vars.GetValue(key_).ToBool();
}

StringID Config::GetOptionString(StringID key_) const{
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

void Config::OnWindowMovedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() < EventType::Count);
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const WindowMovedEvent* eventPtr = dynamic_cast<const WindowMovedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		SetOption(EngineVars::Display::sectionName, EngineVars::Display::lastWindowXKey, eventPtr->GetX());
		SetOption(EngineVars::Display::sectionName, EngineVars::Display::lastWindowYKey, eventPtr->GetY());
	}
}

void Config::OnWindowResizedEvent(const Event& e_){
	GADGET_BASIC_ASSERT(e_.GetEventType() == WindowResizedEvent::Type());
	GADGET_BASIC_ASSERT(e_.GetName() != StringID::None);

	const WindowResizedEvent* eventPtr = dynamic_cast<const WindowResizedEvent*>(&e_);
	GADGET_BASIC_ASSERT(eventPtr != nullptr);
	if(eventPtr != nullptr){
		SetOption(EngineVars::Display::sectionName, EngineVars::Display::displayWidthKey, eventPtr->GetWidth());
		SetOption(EngineVars::Display::sectionName, EngineVars::Display::displayHeightKey, eventPtr->GetHeight());
	}
}

void Config::SaveConfigs() const{
	ConfigParser::SerializeConfigs(engineConfigPath, vars);
}

void Config::ResetOptionToDefault(StringID option_){
	const auto eVarsDefault = EngineVars();
	for(const auto& s : vars.sections){
		if(s == nullptr){
			Debug::Log(SID("CONFIG"), "nullptr found in config sections!", Debug::Error, __FILE__, __LINE__);
			continue;
		}

		if(Utils::ContainsKey(s->vars, option_)){
			vars.SetValue(s->name, option_, eVarsDefault.GetValue(option_));
			return;
		}
	}
}

void Config::ResetSectionToDefault(StringID section_){
	const auto eVarsDefault = EngineVars();
	for(const auto& s : eVarsDefault.sections){
		if(s == nullptr){
			Debug::Log(SID("CONFIG"), "nullptr found in config sections!", Debug::Error, __FILE__, __LINE__);
			continue;
		}

		if(s->name == section_){
			for(const auto& o : s->vars){
				vars.SetValue(o.first, o.second);
			}
		}	
	}
}

void Config::ResetAllOptionsToDefault(){
	vars = EngineVars();
}

std::string Config::CreateEngineConfigPath(){
#ifdef GADGET_DEBUG
	return engineConfigFileName; //Storing the config file in the same directory makes debugging much easier
#else
	return FileSystem::ConstructFilePath(FileSystem::GetPersistentDataDir(), App::GetGameName(), engineConfigFileName);
#endif //GADGET_DEBUG
}