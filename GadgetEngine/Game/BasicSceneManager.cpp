#include "BasicSceneManager.h"

#include "Utils/Utils.h"

using namespace Gadget;

BasicSceneManager::BasicSceneManager() : scenes(), sceneIndex(0){}

BasicSceneManager::~BasicSceneManager(){
	for(auto& s : scenes){
		delete s;
		s = nullptr;
	}
}

void BasicSceneManager::AddScene(Scene* scene_){
	GADGET_BASIC_ASSERT(scene_ != nullptr);
	if(scene_ == nullptr){
		return;
	}

	GADGET_ASSERT(!Utils::Contains(scenes, scene_), "Scene \"" + scene_->GetName().GetString() + "\" is being added to the scene list for this object multiple times!");
	scenes.push_back(scene_);
}

void BasicSceneManager::LoadScene(size_t index_){
	CurrentScene()->SetToDefaultState();

	GADGET_BASIC_ASSERT(index_ < scenes.size());
	if(index_ >= scenes.size()){
		sceneIndex = scenes.size() - 1;
	}else{
		sceneIndex = index_;
	}
}

void BasicSceneManager::LoadScene(StringID name_){
	CurrentScene()->SetToDefaultState();
	for(int i = 0; i < scenes.size(); i++){
		if(scenes[i]->name == name_){
			sceneIndex = i;
		}
	}

	Debug::Log("Tried to load invalid Scene \"" + name_.GetString() + "\"!", Debug::Error);
}

Scene* BasicSceneManager::CurrentScene(){
	GADGET_BASIC_ASSERT(sceneIndex < scenes.size());
	GADGET_BASIC_ASSERT(scenes[sceneIndex] != nullptr);
	return scenes[sceneIndex];
}