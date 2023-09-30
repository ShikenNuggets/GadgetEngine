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

void BasicSceneManager::Update(float deltaTime_){
	if(!sceneLoadRequests.empty()){
		LoadScene(sceneLoadRequests.front());
		sceneLoadRequests.pop();
		GADGET_ASSERT(sceneLoadRequests.empty(), "More than one scene load request was pending at a time!");
	}

	CurrentScene()->Update(deltaTime_);
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

	CurrentScene()->SetToDefaultState();
}

void BasicSceneManager::LoadScene(StringID name_){
	CurrentScene()->SetToDefaultState();
	for(size_t i = 0; i < scenes.size(); i++){
		if(scenes[i]->name == name_){
			sceneIndex = i;
			CurrentScene()->SetToDefaultState();
			return;
		}
	}

	Debug::Log("Tried to load invalid Scene \"" + name_.GetString() + "\"!", Debug::Error, __FILE__, __LINE__);
}

void BasicSceneManager::RequestSceneLoad(size_t index_){
	GADGET_BASIC_ASSERT(index_ < scenes.size());

	if(!sceneLoadRequests.empty()){
		Debug::Log("Multiple scene load requests received on the same frame, this may lead to unintended behaviour!", Debug::Warning, __FILE__, __LINE__);
		sceneLoadRequests.pop(); //There really shouldn't ever be more than one scene load request per frame, but if there is, we'll just take the last one
	}

	sceneLoadRequests.push(index_);
}

void BasicSceneManager::RequestSceneLoad(StringID name_){
	for(size_t i = 0; i < scenes.size(); i++){
		if(scenes[i]->name == name_){
			RequestSceneLoad(i);
		}
	}

	Debug::Log("Requested load of invalid Scene \"" + name_.GetString() + "\"!", Debug::Error, __FILE__, __LINE__);
}

Scene* BasicSceneManager::CurrentScene(){
	GADGET_BASIC_ASSERT(sceneIndex < scenes.size());
	GADGET_BASIC_ASSERT(scenes[sceneIndex] != nullptr);
	return scenes[sceneIndex];
}