#include "PhysManager.h"

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

void PhysManager::Update(float deltaTime_){}