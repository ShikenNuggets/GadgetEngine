#include "Scene.h"
#include "Utils/Utils.h"

using namespace Gadget;

Scene::Scene(StringID name_) : name(name_), gameObjects(){
	SetToDefaultState();
}

Scene::~Scene(){
	DestroyAllGameObjects();
	DestroyAllSceneComponents();
}

void Scene::CreateObject(GameObject* gameObject_){
	GADGET_BASIC_ASSERT(gameObject_ != nullptr);
	if(gameObject_ == nullptr){
		return;
	}

	GADGET_ASSERT(!Utils::Contains(gameObjects, gameObject_), "GameObject \"" + gameObject_->GetName().GetString() + "\" is being added to the GameObject list for this scene multiple times!");
	gameObjects.push_back(gameObject_);
}

void Scene::AddSceneComponent(SceneComponent* sceneComp_){
	GADGET_BASIC_ASSERT(sceneComp_ != nullptr);
	if(sceneComp_ == nullptr){
		return;
	}

	GADGET_ASSERT(!Utils::Contains(sceneComponents, sceneComp_), "SceneComponent is being added to the GameObject list for this scene multiple times!");
	sceneComponents.push_back(sceneComp_);
}

void Scene::Update(float deltaTime_){
	for(const auto& sc : sceneComponents){
		sc->OnUpdate(deltaTime_);
	}

	for(auto& go : gameObjects){
		go->Update(deltaTime_);
	}
}

void Scene::DestroyAllGameObjects(){
	for(const auto& go : gameObjects){
		delete go;
	}

	gameObjects.clear();
}

void Scene::DestroyAllSceneComponents(){
	for(const auto& sc : sceneComponents){
		delete sc;
	}

	sceneComponents.clear();
}