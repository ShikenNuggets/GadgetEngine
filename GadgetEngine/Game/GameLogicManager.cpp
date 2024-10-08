#include "GameLogicManager.h"

#include "Game/GameLogicComponent.h"

using namespace Gadget;

//Overly simplistic but it's fine for just a basic integration
void GameLogicManager::Update(const Scene* scene_, float deltaTime_){
	GADGET_BASIC_ASSERT(scene_ != nullptr);
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);
	
	scene_->GetAllComponentsInScene<GameLogicComponent>(gameLogicsBuffer); //TODO - This is very inefficient, find a better way to do this
	for(auto& lc : gameLogicsBuffer){
		GADGET_BASIC_ASSERT(lc != nullptr);
		if(!lc->HasStarted()){
			lc->OnStart();
		}

		lc->OnUpdate(deltaTime_);

		while(lc->HasCollisionsToHandle()){
			lc->OnCollision(lc->PopCollisionToHandle());
		}

		GADGET_BASIC_ASSERT(lc->HasStarted());
	}

	//So we don't accidentally reuse these pointers later
	gameLogicsBuffer.Clear();
}