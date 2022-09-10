#include "LocManager.h"

#include "Debug.h"

using namespace Gadget;

LocManager* LocManager::instance = nullptr;

LocManager::LocManager() : currentLanguage(0), languages(){
	languages.reserve(8); //This feels like a good number
}

LocManager::~LocManager(){}

LocManager* LocManager::GetInstance(){
	if(instance == nullptr){
		instance = new LocManager();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void LocManager::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

StringID LocManager::GetCurrentLanguage() const{
	GADGET_ASSERT(currentLanguage < languages.size(), "Current Language set to an invalid value!");
	return languages[currentLanguage];
}

void LocManager::AddLanguage(StringID language_){
	GADGET_ASSERT(languages.size() < std::numeric_limits<uint8_t>::max(), "LocManager cannot support more than 256 languages!");
	GADGET_ASSERT(std::find(languages.begin(), languages.end(), language_) == languages.end(), language_.GetString() + " added to the language list multiple times!");
	languages.push_back(language_);
}

void LocManager::SetCurrentLanguage(StringID language_){
	for(size_t i = 0; i < languages.size(); i++){
		if(languages[i] == language_){
			currentLanguage = static_cast<uint8_t>(i);
			return;
		}
	}

	GADGET_ASSERT(false, "Tried to set language to invalid value [" + language_.GetString() + "]!");
}