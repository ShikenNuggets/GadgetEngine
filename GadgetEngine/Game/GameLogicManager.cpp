#include "GameLogicManager.h"

#include "GameLogicComponent.h"

using namespace Gadget;

GameLogicManager::GameLogicManager(){}

//Overly simplistic but it's fine for just a basic integration
void GameLogicManager::Update(const Scene* scene_, float deltaTime_){
	//TODO - This is very inefficient, find a better way to do this
	auto lcs = scene_->GetAllComponentsInScene<GameLogicComponent>();
	for(auto& lc : lcs){
		if(!lc->HasStarted()){
			lc->OnStart();
		}

		lc->OnUpdate(deltaTime_);
	}
}