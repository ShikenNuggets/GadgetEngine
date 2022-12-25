#include "PhysManager.h"

#include "Config.h"
#include "Physics/Rigidbody.h"

using namespace Gadget;

PhysManager* PhysManager::instance = nullptr;

PhysManager::PhysManager(){}

PhysManager::~PhysManager(){}

PhysManager* PhysManager::GetInstance(){
	if(instance == nullptr){
		instance = new PhysManager();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void PhysManager::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

void PhysManager::Update(Scene* scene_, float deltaTime_){
	const float gravityConstant = static_cast<float>(Config::GetInstance()->GetOptionFloat(EngineVars::Physics::gravityConstantKey));

	const auto rbs = scene_->GetAllComponentsInScene<Rigidbody>();
	for(const auto& rb : rbs){
		if(rb->UseGravity()){
			rb->SetVelocity(rb->GetVelocity() + (Vector3(0.0f, gravityConstant,  0.0f) * deltaTime_));
		}

		rb->Update(deltaTime_);
	}
}