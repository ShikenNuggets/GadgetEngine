#include "Scene.h"
#include "Utils/Utils.h"

using namespace Gadget;

Scene::Scene(StringID name_) : guid(GUID::Generate()), name(name_), gameObjects(){
	GADGET_BASIC_ASSERT(name_ != StringID::None);
	Scene::SetToDefaultState();
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
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);

	for(const auto& sc : sceneComponents){
		GADGET_BASIC_ASSERT(sc != nullptr);
		sc->OnUpdate(deltaTime_);
	}

	for(auto& go : gameObjects){
		GADGET_BASIC_ASSERT(go != nullptr);
		go->Update(deltaTime_);
		
		if(go->HasLifeTimeEnded()){
			delete go;
			go = nullptr;
		}
	}

	const auto [first, last] = std::ranges::remove(gameObjects, nullptr);
	gameObjects.erase(first, last);
}

GameObject* Scene::FindWithTag(StringID tag_){
	GADGET_BASIC_ASSERT(tag_ != StringID::None);

	for(const auto& go : gameObjects){
		if(go->HasTag(tag_)){
			return go;
		}
	}

	return nullptr;
}

std::vector<GameObject*> Scene::FindObjectsWithTag(StringID tag_){
	GADGET_BASIC_ASSERT(tag_ != StringID::None);

	std::vector<GameObject*> objs;
	for(const auto& go : gameObjects){
		if(go->HasTag(tag_)){
			objs.push_back(go);
		}
	}

	return objs;
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