#include "Rigidbody.h"

#include "Game/GameObject.h"

using namespace Gadget;

ComponentCollection<Rigidbody> Rigidbody::componentCollection;

Rigidbody::Rigidbody(GameObject* parent_, float mass_, bool useGravity_, FreezeRotationType freezeType_) : Component(SID("Rigidbody"), parent_), mass(mass_), useGravity(useGravity_), freezeRotation(freezeType_), maxVelocity(Math::Infinity, Math::Infinity, Math::Infinity), brakingSpeed(0.0f), bulletRb(nullptr), hasCachedForce(false), hasCachedVelocity(false), cachedForces(), cachedVelocity(){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(mass_));
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);

	mass = Math::Clamp(0.0001f, Math::Infinity, mass); //0.0001 is completely arbitrary

	componentCollection.Add(this);
}

Rigidbody::Rigidbody(GUID parentGUID_, float mass_, bool useGravity_, FreezeRotationType freezeType_) : Component(SID("Rigidbody"), parentGUID_), mass(mass_), useGravity(useGravity_), freezeRotation(freezeType_), maxVelocity(Math::Infinity, Math::Infinity, Math::Infinity), brakingSpeed(0.0f), bulletRb(nullptr), hasCachedForce(false), hasCachedVelocity(false), cachedForces(), cachedVelocity(){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(mass_));
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);

	mass = Math::Clamp(0.0001f, Math::Infinity, mass); //0.0001 is completely arbitrary

	componentCollection.Add(this);
}

Rigidbody::Rigidbody(const ComponentProperties& props_) : Component(props_), mass(1.0f), useGravity(true), freezeRotation(FreezeRotationType::None), maxVelocity(Math::Infinity, Math::Infinity, Math::Infinity), brakingSpeed(0.0f), bulletRb(nullptr), hasCachedForce(false), hasCachedVelocity(false), cachedForces(), cachedVelocity(){
	Rigidbody::Deserialize(props_);
}

Rigidbody::~Rigidbody(){
	componentCollection.Remove(this);
}

void Rigidbody::Update([[maybe_unused]] float deltaTime_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_));
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);

	if(parent == nullptr){
		Debug::Log("Update called on Rigidbody with no parent!", Debug::Error, __FILE__, __LINE__);
		return;
	}

	if(bulletRb == nullptr){
		return; //If we're not currently part of the physics sim, do nothing
	}

	if(hasCachedForce){
		for(const auto& f : cachedForces){
			AddForce(f);
		}
		cachedForces.clear();
		hasCachedForce = false;
	}

	if(hasCachedVelocity){
		SetVelocity(cachedVelocity);
		hasCachedVelocity = false;
	}

	if(brakingSpeed > 0.0f){
		Vector3 currentVelocity = BulletHelper::ConvertVector3(bulletRb->getLinearVelocity());
		currentVelocity.x = ApplyBrakes(currentVelocity.x, deltaTime_);
		//currentVelocity.y = ApplyBrakes(currentVelocity.y, deltaTime_); //TODO - Hardcoding this to not overwrite gravity
		currentVelocity.z = ApplyBrakes(currentVelocity.z, deltaTime_);
		SetVelocity(currentVelocity);
	}

	btTransform bulletTransform;
	bulletRb->getMotionState()->getWorldTransform(bulletTransform);

	parent->SetPosition(BulletHelper::ConvertVector3(bulletTransform.getOrigin()));
	parent->SetRotation(BulletHelper::ConvertQuaternion(bulletTransform.getRotation()));
}

void Rigidbody::AddForce(const Vector3& force_){
	GADGET_BASIC_ASSERT(force_.IsValid());
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);
	GADGET_BASIC_ASSERT(bulletRb != nullptr);

	if(bulletRb == nullptr){
		//Apply force on next update
		cachedForces.push_back(force_);
		hasCachedForce = true;
		return;
	}

	bulletRb->applyCentralForce(BulletHelper::ConvertVector3(force_));
}

void Rigidbody::AddVelocity(const Vector3& vel_){
	GADGET_BASIC_ASSERT(vel_.IsValid());
	if(Math::IsNearZero(vel_.x) && Math::IsNearZero(vel_.y) && Math::IsNearZero(vel_.z)){
		return;
	}

	Vector3 newVelocity = BulletHelper::ConvertVector3(bulletRb->getLinearVelocity()) + vel_;
	newVelocity.x = Math::Clamp(-maxVelocity.x, maxVelocity.x, newVelocity.x);
	newVelocity.y = Math::Clamp(-maxVelocity.y, maxVelocity.y, newVelocity.y);
	newVelocity.z = Math::Clamp(-maxVelocity.z, maxVelocity.z, newVelocity.z);

	bulletRb->setLinearVelocity(BulletHelper::ConvertVector3(newVelocity));
}

void Rigidbody::AddVelocity(float x_, float y_, float z_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z_));

	AddVelocity(Vector3(x_, y_, z_));
}

Vector3 Rigidbody::GetVelocity() const{
	if(bulletRb == nullptr){
		return Vector3::Zero();
	}

	return BulletHelper::ConvertVector3(bulletRb->getLinearVelocity());
}

void Rigidbody::SetMass(float mass_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(mass_));
	GADGET_ASSERT(mass_ > 0.0f, "Mass <= 0 is not supported!");
	GADGET_ASSERT(!Math::IsNearZero(mass_), "Mass of 0 is not supported!");
	mass = mass_;

	if(bulletRb != nullptr && bulletRb->getCollisionShape() != nullptr){
		btVector3 inertia;
		bulletRb->getCollisionShape()->calculateLocalInertia(mass, inertia);
		bulletRb->setMassProps(mass, inertia);
	}
}

void Rigidbody::SetVelocity(const Vector3& vel_){
	GADGET_BASIC_ASSERT(vel_.IsValid());

	if(bulletRb == nullptr){
		//Set velocity on next update
		cachedVelocity = vel_;
		hasCachedVelocity = true;
		return;
	}

	if(!Math::IsNearZero(vel_.x) || !Math::IsNearZero(vel_.y) || !Math::IsNearZero(vel_.z)){
		bulletRb->activate(true); //Force activate the rigidbody if we're hard setting a non-zero velocity
	}

	bulletRb->setLinearVelocity(BulletHelper::ConvertVector3(vel_));
}

void Rigidbody::SetVelocity(float x_, float y_, float z_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z_));

	SetVelocity(Vector3(x_, y_, z_));
}

void Rigidbody::FreezeRotation(FreezeRotationType type_){
	GADGET_BASIC_ASSERT(type_ < FreezeRotationType::FreezeRotationType_MAX);
	freezeRotation = type_;

	if(bulletRb != nullptr){
		switch(freezeRotation){
			case FreezeRotationType::None:
				bulletRb->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeX:
				bulletRb->setAngularFactor(btVector3(0.0f, 1.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeY:
				bulletRb->setAngularFactor(btVector3(1.0f, 0.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeZ:
				bulletRb->setAngularFactor(btVector3(1.0f, 1.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeXY:
				bulletRb->setAngularFactor(btVector3(0.0f, 0.0f, 1.0f));
				break;
			case FreezeRotationType::FreezeXZ:
				bulletRb->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeYZ:
				bulletRb->setAngularFactor(btVector3(1.0f, 0.0f, 0.0f));
				break;
			case FreezeRotationType::FreezeAll:
				bulletRb->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
				break;
			default:
				GADGET_ASSERT_NOT_IMPLEMENTED;
				break;
		}
	}
}

void Rigidbody::SetMaxVelocity(const Vector3& maxVelocity_){
	maxVelocity = maxVelocity_;
}

void Rigidbody::SetBrakingSpeed(float brakingSpeed_){
	GADGET_ASSERT(brakingSpeed_ >= 0.0f, "Negative number entered for brake speed - Will be treated as 0");
	GADGET_BASIC_ASSERT(Math::IsValidNumber(brakingSpeed_));
	brakingSpeed = brakingSpeed_;
}

void Rigidbody::ClearForces(){
	if(bulletRb == nullptr){
		return;
	}

	bulletRb->clearForces();
}

//void Rigidbody::CollisionResponse(const Collision& collision_){
//	GADGET_BASIC_ASSERT(collision_.collisionVector.IsValid());
//	GADGET_BASIC_ASSERT(Math::IsValidNumber(collision_.overlapAmount));
//
//	//TODO - This is obviously not proper collision response
//	parent->Translate(-collision_.collisionVector.Normalized() * collision_.overlapAmount);
//}

void Rigidbody::Reset(){
	Collider* collider = parent->GetComponent<Collider>();
	GADGET_BASIC_ASSERT(collider != nullptr); //All rigidbodies must have colliders

	if(collider != nullptr){
		collider->Reset();
	}
}

float Rigidbody::ApplyBrakes(float curVelocity_, float deltaTime_){
	if(Math::Abs(curVelocity_) < (brakingSpeed * deltaTime_)){
		return 0.0f;
	}
	
	if(curVelocity_ < 0.0f){
		return curVelocity_ + brakingSpeed * deltaTime_;
	}else{
		return curVelocity_ - brakingSpeed * deltaTime_;
	}
}

ComponentProperties Rigidbody::Serialize() const{
	ComponentProperties props = Component::Serialize();

	props.variables.Add(SID("Mass"), mass);
	props.variables.Add(SID("UseGravity"), useGravity);
	props.variables.Add(SID("FreezeRotation"), static_cast<int>(freezeRotation));

	return props;
}

void Rigidbody::Deserialize(const ComponentProperties& props_){
	mass = props_.variables.GetValue(SID("Mass"), 1.0f).ToNumber<float>();
	useGravity = props_.variables.GetValue(SID("UseGravity"), true).ToBool();
	freezeRotation = static_cast<FreezeRotationType>(props_.variables.GetValue(SID("FreezeRotation"), 0).ToNumber<int>());

	GADGET_BASIC_ASSERT(Math::IsValidNumber(mass));
	GADGET_BASIC_ASSERT(!Math::IsNearZero(mass));
	GADGET_BASIC_ASSERT(mass > 0.0f);
	GADGET_BASIC_ASSERT(static_cast<int>(freezeRotation) >= 0);
	GADGET_BASIC_ASSERT(freezeRotation < FreezeRotationType::FreezeRotationType_MAX);
}