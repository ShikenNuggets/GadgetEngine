#include "PhysManager.h"

#include "App.h"
#include "BulletHelper.h"
#include "Config.h"
#include "Collider.h"
#include "CollisionSystem.h"
#include "Rigidbody.h"
#include "Game/GameLogicComponent.h"

using namespace Gadget;

PhysManager::PhysManager() : bulletDynamicsWorld(nullptr){
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadphase = new btDbvtBroadphase(); //TODO - Other broadphase classes may have better performance characteristics
	solver = new btSequentialImpulseConstraintSolver();

	bulletDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	bulletDynamicsWorld->setGravity(btVector3(0.0f, static_cast<float>(App::GetConfig().GetOptionFloat(EngineVars::Physics::gravityConstantKey)), 0.0f));
}

PhysManager::~PhysManager(){
	for(auto& col : cachedCollisionShapes){
		delete col;
		col = nullptr;
	}
	cachedCollisionShapes.clear();

	delete bulletDynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfig;
}

void PhysManager::Update(Scene* scene_, float deltaTime_){
	GADGET_BASIC_ASSERT(scene_ != nullptr);
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);

	const float fixedTimeStep = 1.0f / static_cast<float>(App::GetConfig().GetOptionFloat(EngineVars::Physics::physicsUpdatesKey));

	bulletDynamicsWorld->stepSimulation(deltaTime_, 4, fixedTimeStep);

	const auto rbs = scene_->GetAllComponentsInScene<Rigidbody>(); //TODO - This is slow
	for(const auto& rb : rbs){
		rb->Update(deltaTime_);
	}

	bulletDynamicsWorld->performDiscreteCollisionDetection();

	std::vector<std::pair<btRigidBody*, btRigidBody*>> collisionPairs;
	const auto& pairArray = bulletDynamicsWorld->getPairCache()->getOverlappingPairArray();
	for(int i = 0; i < pairArray.size(); i++){
		if(pairArray[i].m_pProxy0 == nullptr || pairArray[i].m_pProxy0->m_clientObject == nullptr || pairArray[i].m_pProxy1 == nullptr || pairArray[i].m_pProxy1->m_clientObject == nullptr){
			collisionPairs.push_back(std::make_pair<btRigidBody*, btRigidBody*>(nullptr, nullptr)); //Maintains index continuity
			continue;
		}

		collisionPairs.push_back(std::make_pair<btRigidBody*, btRigidBody*>((btRigidBody*)pairArray[i].m_pProxy0->m_clientObject, (btRigidBody*)pairArray[i].m_pProxy1->m_clientObject));
	}

	const auto cls = scene_->GetAllComponentsInScene<Collider>(); //TODO - This is slow
	//TODO - This is *really* slow. It checks every collider in the scene against every other collider
	//This is bad enough in a small scene but it'll get exponentially worse as more colliders are added
	for(size_t i = 0; i < cls.size(); i++){
		for(size_t j = i + 1; j < cls.size(); j++){
			GADGET_BASIC_ASSERT(cls[i] != nullptr);
			GADGET_BASIC_ASSERT(cls[j] != nullptr);

			if(cls[i] == nullptr || cls[j] == nullptr || cls[i]->bulletRb == nullptr || cls[j]->bulletRb == nullptr){
				continue;
			}

			for(size_t k = 0; k < collisionPairs.size(); k++){
				if((collisionPairs[k].first == cls[i]->bulletRb && collisionPairs[k].second == cls[j]->bulletRb)
					|| (collisionPairs[k].first == cls[j]->bulletRb && collisionPairs[k].second == cls[i]->bulletRb)){
					HandleCollisionResponse(pairArray[(int)k], cls[i], cls[j]);
					HandleCollisionResponse(pairArray[(int)k], cls[j], cls[i]);
					break;
				}
			}
		}
	}
}

btRigidBody* PhysManager::AddToSimulation(const Collider* col_, const Rigidbody* rb_){
	GADGET_BASIC_ASSERT(bulletDynamicsWorld != nullptr);
	GADGET_BASIC_ASSERT(col_ != nullptr);
	GADGET_BASIC_ASSERT(col_->GetShape() != ColliderShape::None);
	GADGET_ASSERT(rb_ == nullptr || !Math::IsNearZero(rb_->GetMass()), "Rigidbody with mass of 0 is not supported!");

	btCollisionShape* shape = CreateCollisionShape(col_);
	if(shape == nullptr){
		Debug::Log("An error occured while creating the collision shape of type [" + std::to_string((int)col_->GetShape()) + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	cachedCollisionShapes.push_back(shape); //TODO - Actually check the cached shapes before making a new one

	btTransform startTransform = btTransform(BulletHelper::ConvertQuaternion(col_->GetParent()->GetTransform().rotation), BulletHelper::ConvertVector3(col_->GetParent()->GetTransform().position));
	btVector3 localInertia = btVector3(0.0f, 0.0f, 0.0f);
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBody* body = nullptr;
	if(rb_ == nullptr){
		//Static object
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, shape, localInertia);
		body = new btRigidBody(rbInfo);

		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}else{
		//Dynamic object
		shape->calculateLocalInertia(rb_->GetMass(), localInertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(rb_->GetMass(), motionState, shape, localInertia);
		body = new btRigidBody(rbInfo);

		switch(rb_->GetFreezeRotation()){
			case FreezeRotationType::None:
				body->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeX:
				body->setAngularFactor(btVector3(0.0f, 1.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeY:
				body->setAngularFactor(btVector3(1.0f, 0.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeZ:
				body->setAngularFactor(btVector3(1.0f, 1.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeXY:
				body->setAngularFactor(btVector3(0.0f, 0.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeXZ:
				body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeYZ:
				body->setAngularFactor(btVector3(1.0f, 0.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeAll:
				body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
				break;
			default:
				GADGET_ASSERT_NOT_IMPLEMENTED;
				break;
		}
	}

	if(col_->IsTrigger()){
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	GADGET_BASIC_ASSERT(body != nullptr);

	//add the body to the dynamics world
	bulletDynamicsWorld->addRigidBody(body);

	//Gravity must be set *after* adding the rigidbody to the world
	if(rb_ == nullptr || !rb_->UseGravity()){
		body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
	}else{
		body->applyGravity();
	}

	return body;
}

void PhysManager::RemoveFromSimulation(btRigidBody* brb_){
	bulletDynamicsWorld->removeRigidBody(brb_);
	delete brb_;
}

void PhysManager::HandleCollisionResponse([[maybe_unused]] btBroadphasePair collisionPair_, Collider* collider_, Collider* other_){
	Collision collision;
	collision.otherTags = other_->GetParent()->GetTags();
	collision.otherPos = other_->GetParent()->GetPosition();
	collision.otherScale = other_->GetColliderSize();
	collision.collisionVector = CollisionSystem::CalculateContactPoint(collider_, other_) - collider_->GetParent()->GetPosition();

	if(!collision.collisionVector.IsNear(Vector3::Zero())){
		collision.overlapAmount = CollisionSystem::CalculateOverlapAmount(collision.collisionVector.Normalized(), collider_, other_);
	}

	collision.isTrigger = collider_->IsTrigger() || other_->IsTrigger();

	auto lgs = collider_->GetParent()->GetComponents<GameLogicComponent>();
	for(const auto& lg : lgs){
		lg->AddCollisionToHandle(collision);
	}
}

btCollisionShape* PhysManager::CreateCollisionShape(const Collider* col_){
	switch(col_->GetShape())
	{
		case ColliderShape::Box2D:
		case ColliderShape::Cube:
			return new btBoxShape(BulletHelper::ConvertVector3(col_->GetColliderSize()));
		case ColliderShape::Circle2D:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
		case ColliderShape::Sphere:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
		default:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
	}

	return nullptr;
}