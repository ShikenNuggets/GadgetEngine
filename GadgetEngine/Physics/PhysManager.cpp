#include "PhysManager.h"

#include "App.h"
#include "Config.h"
#include "Physics/CollisionSystem.h"
#include "Game/GameLogicComponent.h"

using namespace Gadget;

PhysManager::PhysManager() : bulletDynamicsWorld(nullptr), collisionConfig(nullptr), dispatcher(nullptr), broadphase(nullptr), solver(nullptr){
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadphase = new btDbvtBroadphase(); //TODO - Other broadphase classes may have better performance characteristics
	solver = new btSequentialImpulseConstraintSolver();

	bulletDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	bulletDynamicsWorld->setGravity(btVector3(0.0f, static_cast<float>(App::GetConfig().GetOptionFloat(EngineVars::Physics::gravityConstantKey)), 0.0f));

	GADGET_BASIC_ASSERT(bulletDynamicsWorld != nullptr);
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
	GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_));
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);

	bulletDynamicsWorld->stepSimulation(deltaTime_, 0); //TODO - We should eventually do proper interpolation w/ fixed time step

	scene_->GetAllComponentsInScene<Rigidbody>(rigidbodiesBuffer); //TODO - This is slow
	for(const auto& rb : rigidbodiesBuffer){
		rb->Update(deltaTime_);
	}

	scene_->GetAllComponentsInScene<Collider>(collidersBuffer); //TODO - This is slow
	for(const auto* cl : collidersBuffer){
		GADGET_BASIC_ASSERT(cl != nullptr);
		if(cl == nullptr || cl->bulletRb == nullptr){
			continue;
		}

		BulletCollisionResultCallback callback;
		bulletDynamicsWorld->contactTest(cl->bulletRb, callback);
	}

	//So we don't accidentally reuse these pointers later
	rigidbodiesBuffer.clear();
	collidersBuffer.clear();
}

btRigidBody* PhysManager::AddToSimulation(const Collider* col_, const Rigidbody* rb_){
	GADGET_BASIC_ASSERT(bulletDynamicsWorld != nullptr);
	GADGET_BASIC_ASSERT(col_ != nullptr);
	GADGET_BASIC_ASSERT(col_->GetShape() != ColliderShape::None);
	GADGET_BASIC_ASSERT(col_->GetShape() < ColliderShape::ColliderShape_MAX);
	GADGET_ASSERT(rb_ == nullptr || !Math::IsNearZero(rb_->GetMass()), "Rigidbody with mass of 0 is not supported!");

	btCollisionShape* shape = CreateCollisionShape(col_);
	if(shape == nullptr){
		Debug::Log("An error occured while creating the collision shape of type [" + std::to_string(static_cast<int>(col_->GetShape())) + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	cachedCollisionShapes.push_back(shape); //TODO - Actually check the cached shapes before making a new one

	const btTransform startTransform = btTransform(BulletHelper::ConvertQuaternion(col_->GetParent()->GetTransform().rotation), BulletHelper::ConvertVector3(col_->GetParent()->GetTransform().position));
	btVector3 localInertia = btVector3(0.0f, 0.0f, 0.0f);
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBody* body = nullptr;
	if(rb_ == nullptr){
		//Static object
		const btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, shape, localInertia);
		body = new btRigidBody(rbInfo);

		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}else{
		//Dynamic object
		shape->calculateLocalInertia(rb_->GetMass(), localInertia);

		const btRigidBody::btRigidBodyConstructionInfo rbInfo(rb_->GetMass(), motionState, shape, localInertia);
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
	GADGET_BASIC_ASSERT(brb_ != nullptr);

	bulletDynamicsWorld->removeRigidBody(brb_);
	delete brb_;
}

void PhysManager::HandleCollisionResponse(Collider* collider_, Collider* other_){
	GADGET_BASIC_ASSERT(collider_ != nullptr);
	GADGET_BASIC_ASSERT(collider_->GetParent() != nullptr);
	GADGET_BASIC_ASSERT(other_ != nullptr);
	GADGET_BASIC_ASSERT(other_->GetParent() != nullptr);

	Collision collision;
	collision.otherName = other_->GetParent()->GetName();
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
	GADGET_BASIC_ASSERT(col_ != nullptr);
	GADGET_BASIC_ASSERT(col_->GetShape() != ColliderShape::None);
	GADGET_BASIC_ASSERT(col_->GetShape() != ColliderShape::ColliderShape_MAX);
	GADGET_BASIC_ASSERT(col_->GetColliderSize().IsValid());

	switch(col_->GetShape()){
		case ColliderShape::Box2D:
		case ColliderShape::Cube:
			return new btBoxShape(BulletHelper::ConvertVector3(col_->GetColliderSize()));
		case ColliderShape::Circle2D:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
		case ColliderShape::Sphere:
			return new btSphereShape(col_->GetColliderSize().Magnitude());
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
		default:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
	}

	return nullptr;
}

//Return value of this function is not used by Bullet, in case you're wondering
btScalar BulletCollisionResultCallback::addSingleResult([[maybe_unused]] btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, [[maybe_unused]] int partId0, [[maybe_unused]] int index0, const btCollisionObjectWrapper* colObj1Wrap, [[maybe_unused]] int partId1, [[maybe_unused]] int index1){
	GADGET_BASIC_ASSERT(colObj0Wrap != nullptr && colObj1Wrap != nullptr);
	if(colObj0Wrap == nullptr || colObj1Wrap == nullptr){
		Debug::Log("Collision callback called with invalid collision object wrappers!", Debug::Error, __FILE__, __LINE__);
		return 0;
	}

	const btCollisionObject* obj0 = colObj0Wrap->getCollisionObject();
	const btCollisionObject* obj1 = colObj1Wrap->getCollisionObject();

	GADGET_BASIC_ASSERT(obj0 != nullptr && obj1 != nullptr && obj0->getUserPointer() != nullptr && obj1->getUserPointer() != nullptr);
	if(obj0 == nullptr || obj1 == nullptr){
		Debug::Log("Collision callback called with invalid collision objects!", Debug::Error, __FILE__, __LINE__);
		return 0;
	}

	if(obj0->getUserPointer() == nullptr || obj1->getUserPointer() == nullptr){
		Debug::Log("Collision callback called with invalid collider user pointers!", Debug::Error, __FILE__, __LINE__);
		return 0;
	}

	Collider* col0 = static_cast<Collider*>(obj0->getUserPointer());
	Collider* col1 = static_cast<Collider*>(obj1->getUserPointer());
	
	App::GetPhysics().HandleCollisionResponse(col0, col1);
	App::GetPhysics().HandleCollisionResponse(col1, col0);

	return 0;
}