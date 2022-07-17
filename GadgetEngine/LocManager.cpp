#include "LocManager.h"

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
	_ASSERT(currentLanguage < languages.size());
	return languages[currentLanguage];
}

void LocManager::AddLanguage(StringID language_){
	_ASSERT(std::find(languages.begin(), languages.end(), language_) == languages.end()); //Already added this language
	languages.push_back(language_);
}

void LocManager::SetCurrentLanguage(StringID language_){
	for(size_t i = 0; i < languages.size(); i++){
		if(languages[i] == language_){
			currentLanguage = static_cast<uint8_t>(i);
			return;
		}
	}

	_ASSERT(false); //Could not find the language you tried to set
}