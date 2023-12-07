#include "GameLogicManager.h"

#include "GameLogicComponent.h"

using namespace Gadget;

GameLogicManager::GameLogicManager(){}

//Overly simplistic but it's fine for just a basic integration
void GameLogicManager::Update(const Scene* scene_, float deltaTime_){
	GADGET_BASIC_ASSERT(scene_ != nullptr);
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);

	//TODO - This is very inefficient, find a better way to do this
	auto lcs = scene_->GetAllComponentsInScene<GameLogicComponent>();
	for(auto& lc : lcs){
		GADGET_BASIC_ASSERT(lc != nullptr);
		if(!lc->HasStarted()){
			lc->OnStart();
		}

		lc->OnUpdate(deltaTime_);

		while(lc->HasCollisionsToHandle()){
			lc->OnCollision(lc->PopCollisionToHandle());
		}
	}
}