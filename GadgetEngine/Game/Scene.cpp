#include "Scene.h"
#include "Utils/Utils.h"

using namespace Gadget;

Scene::Scene(StringID name_) : name(name_), gameObjects(){
	SetToDefaultState();
}

Scene::~Scene(){
	for(auto go : gameObjects){
		delete go;
		go = nullptr;
	}
}

void Scene::CreateObject(GameObject* gameObject_){
	GADGET_BASIC_ASSERT(gameObject_ != nullptr);
	if(gameObject_ == nullptr){
		return;
	}

	GADGET_ASSERT(!Utils::Contains(gameObjects, gameObject_), "GameObject \"" + gameObject_->GetName().GetString() + "\" is being added to the GameObject list for this scene multiple times!");
	gameObjects.push_back(gameObject_);
}

void Scene::SetToDefaultState(){}