#include "CollisionSystem.h"

#include "Game/GameObject.h"
#include "Math/MathObjects.h"

using namespace Gadget;

bool CollisionSystem::TestCollision(const BoxCollider2D& cl1_, const BoxCollider2D& cl2_){
	GADGET_BASIC_ASSERT(cl1_.GetParent() != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(cl1_.GetWidth()));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(cl1_.GetHeight()));

	GADGET_BASIC_ASSERT(cl2_.GetParent() != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(cl2_.GetWidth()));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(cl2_.GetHeight()));

	//This assumes axis-aligned bounding boxes... TODO - allow oriented bounding boxes
	const float c1Left =	cl1_.GetParent()->GetPosition().x - (cl1_.GetParent()->GetScale().x * (cl1_.GetWidth() / 2.0f));
	const float c1Right =	cl1_.GetParent()->GetPosition().x + (cl1_.GetParent()->GetScale().x * (cl1_.GetWidth() / 2.0f));
	const float c1Bottom =	cl1_.GetParent()->GetPosition().y - (cl1_.GetParent()->GetScale().y * (cl1_.GetHeight() / 2.0f));
	const float c1Top =		cl1_.GetParent()->GetPosition().y + (cl1_.GetParent()->GetScale().y * (cl1_.GetHeight() / 2.0f));

	const float c2Left =	cl2_.GetParent()->GetPosition().x - (cl2_.GetParent()->GetScale().x * (cl2_.GetWidth() / 2.0f));
	const float c2Right =	cl2_.GetParent()->GetPosition().x + (cl2_.GetParent()->GetScale().x * (cl2_.GetWidth() / 2.0f));
	const float c2Bottom =	cl2_.GetParent()->GetPosition().y - (cl2_.GetParent()->GetScale().y * (cl2_.GetHeight() / 2.0f));
	const float c2Top =		cl2_.GetParent()->GetPosition().y + (cl2_.GetParent()->GetScale().y * (cl2_.GetHeight() / 2.0f));

	GADGET_BASIC_ASSERT(Math::IsValidNumber(c1Left));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(c1Right));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(c1Bottom));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(c1Top));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(c2Left));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(c2Right));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(c2Bottom));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(c2Top));

	return (c1Left <= c2Right && c1Right >= c2Left && c1Bottom <= c2Top && c1Top >= c2Bottom);
}

Vector3 CollisionSystem::CalculateContactPoint(const Collider* col1_, const Collider* col2_){
	GADGET_BASIC_ASSERT(col1_->GetParent() != nullptr);
	GADGET_BASIC_ASSERT(col1_->GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(col1_->GetShape() < ColliderShape::ColliderShape_MAX);
	GADGET_BASIC_ASSERT(col2_->GetParent() != nullptr);
	GADGET_BASIC_ASSERT(col2_->GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(col2_->GetShape() < ColliderShape::ColliderShape_MAX);

	switch(col1_->GetShape()){
		case ColliderShape::None:
			break;
		case ColliderShape::Box2D:
			GADGET_BASIC_ASSERT(dynamic_cast<const BoxCollider2D*>(col1_) != nullptr);
			GADGET_BASIC_ASSERT(dynamic_cast<const BoxCollider2D*>(col2_) != nullptr);
			return CalculateContactPoint(*dynamic_cast<const BoxCollider2D*>(col1_), *dynamic_cast<const BoxCollider2D*>(col2_));
		case ColliderShape::Cube:
			break; //TODO
		case ColliderShape::Circle2D:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
		//TODO
		case ColliderShape::Sphere: [[fallthrough]];
		case ColliderShape::ConcaveMesh: [[fallthrough]];
		case ColliderShape::ConvexMesh: [[fallthrough]];
		default:
			break;
	}

	return Vector3::Zero();
}

float CollisionSystem::CalculateOverlapAmount(const Vector3& dir_, const Collider* col1_, const Collider* col2_){
	GADGET_BASIC_ASSERT(dir_.IsValid());
	GADGET_BASIC_ASSERT(col1_->GetParent() != nullptr);
	GADGET_BASIC_ASSERT(col1_->GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(col1_->GetShape() < ColliderShape::ColliderShape_MAX);
	GADGET_BASIC_ASSERT(col2_->GetParent() != nullptr);
	GADGET_BASIC_ASSERT(col2_->GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(col2_->GetShape() < ColliderShape::ColliderShape_MAX);

	switch(col1_->GetShape()){
		case ColliderShape::None:
			break;
		case ColliderShape::Box2D:
			GADGET_BASIC_ASSERT(dynamic_cast<const BoxCollider2D*>(col1_) != nullptr);
			GADGET_BASIC_ASSERT(dynamic_cast<const BoxCollider2D*>(col2_) != nullptr);
			return CalculateOverlapAmount(dir_, *dynamic_cast<const BoxCollider2D*>(col1_), *dynamic_cast<const BoxCollider2D*>(col2_));
		case ColliderShape::Cube:
			break; //TODO
		case ColliderShape::Circle2D:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
		//TODO
		case ColliderShape::Sphere: [[fallthrough]];
		case ColliderShape::ConcaveMesh: [[fallthrough]];
		case ColliderShape::ConvexMesh: [[fallthrough]];
		default:
			break;
	}

	return 0.0f;
}

Vector3 CollisionSystem::CalculateContactPoint(const BoxCollider2D& cl1_, const BoxCollider2D& cl2_){
	GADGET_BASIC_ASSERT(cl1_.GetParent() != nullptr);
	GADGET_BASIC_ASSERT(cl1_.GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(cl1_.GetShape() < ColliderShape::ColliderShape_MAX);
	GADGET_BASIC_ASSERT(cl2_.GetParent() != nullptr);
	GADGET_BASIC_ASSERT(cl2_.GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(cl2_.GetShape() < ColliderShape::ColliderShape_MAX);

	const float c1Left = cl1_.GetParent()->GetPosition().x - (cl1_.GetParent()->GetScale().x * (cl1_.GetWidth() / 2.0f));
	const float c1Right = cl1_.GetParent()->GetPosition().x + (cl1_.GetParent()->GetScale().x * (cl1_.GetWidth() / 2.0f));
	const float c1Bottom = cl1_.GetParent()->GetPosition().y - (cl1_.GetParent()->GetScale().y * (cl1_.GetHeight() / 2.0f));
	const float c1Top = cl1_.GetParent()->GetPosition().y + (cl1_.GetParent()->GetScale().y * (cl1_.GetHeight() / 2.0f));

	const Rect c1Rect = Rect(
		cl1_.GetParent()->GetPosition().x,
		cl1_.GetParent()->GetPosition().y,
		cl1_.GetParent()->GetScale().x * cl1_.GetWidth(),
		cl1_.GetParent()->GetScale().y * cl1_.GetHeight()
	);

	const Rect c2Rect = Rect(
		cl2_.GetParent()->GetPosition().x,
		cl2_.GetParent()->GetPosition().y,
		cl2_.GetParent()->GetScale().x * cl2_.GetWidth(),
		cl2_.GetParent()->GetScale().y * cl2_.GetHeight()
	);

	const bool topLeft = c2Rect.Intersects(c1Left, c1Top);
	const bool topRight = c2Rect.Intersects(c1Right, c1Top);
	const bool bottomLeft = c2Rect.Intersects(c1Left, c1Bottom);
	const bool bottomRight = c2Rect.Intersects(c1Right, c1Bottom);

	if((topLeft && topRight && bottomLeft && bottomRight)
		|| (!topLeft && !topRight && !bottomLeft && !bottomRight)){
		return cl1_.GetParent()->GetPosition();
	}

	if(topLeft && !topRight && !bottomLeft){
		return Vector3(c1Left, c1Top);
	}else if(topRight && !topLeft && !bottomRight){
		return Vector3(c1Right, c1Top);
	}else if(bottomLeft && !topLeft && !bottomRight){
		return Vector3(c1Left, c1Bottom);
	}else if(bottomRight && !topRight && !bottomLeft){
		return Vector3(c1Right, c1Bottom);
	}

	if(topLeft && topRight){
		return Vector3(c1Rect.x, c1Top);
	}else if(topRight && bottomRight){
		return Vector3(c1Right, c1Rect.y);
	}else if(bottomRight && bottomLeft){
		return Vector3(c1Rect.x, c1Bottom);
	}else if(bottomLeft && topLeft){
		return Vector3(c1Left, c1Rect.y);
	}

	return cl1_.GetParent()->GetPosition();
}

float CollisionSystem::CalculateOverlapAmount(const Vector3& dir_, const BoxCollider2D& cl1_, const BoxCollider2D& cl2_){
	GADGET_BASIC_ASSERT(dir_.IsValid());
	GADGET_BASIC_ASSERT(cl1_.GetParent() != nullptr);
	GADGET_BASIC_ASSERT(cl1_.GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(cl1_.GetShape() < ColliderShape::ColliderShape_MAX);
	GADGET_BASIC_ASSERT(cl2_.GetParent() != nullptr);
	GADGET_BASIC_ASSERT(cl2_.GetColliderSize().IsValid());
	GADGET_BASIC_ASSERT(cl2_.GetShape() < ColliderShape::ColliderShape_MAX);

	const float c1Left = cl1_.GetParent()->GetPosition().x - (cl1_.GetParent()->GetScale().x * (cl1_.GetWidth() / 2.0f));
	const float c1Right = cl1_.GetParent()->GetPosition().x + (cl1_.GetParent()->GetScale().x * (cl1_.GetWidth() / 2.0f));
	const float c1Bottom = cl1_.GetParent()->GetPosition().y - (cl1_.GetParent()->GetScale().y * (cl1_.GetHeight() / 2.0f));
	const float c1Top = cl1_.GetParent()->GetPosition().y + (cl1_.GetParent()->GetScale().y * (cl1_.GetHeight() / 2.0f));

	const float c2Left = cl2_.GetParent()->GetPosition().x - (cl2_.GetParent()->GetScale().x * (cl2_.GetWidth() / 2.0f));
	const float c2Right = cl2_.GetParent()->GetPosition().x + (cl2_.GetParent()->GetScale().x * (cl2_.GetWidth() / 2.0f));
	const float c2Bottom = cl2_.GetParent()->GetPosition().y - (cl2_.GetParent()->GetScale().y * (cl2_.GetHeight() / 2.0f));
	const float c2Top = cl2_.GetParent()->GetPosition().y + (cl2_.GetParent()->GetScale().y * (cl2_.GetHeight() / 2.0f));

	float xOverlap = 0.0f;
	float yOverlap = 0.0f;

	if(dir_.x < 0.0f){
		xOverlap = Math::Abs(c1Left - c2Right);
	}else if(dir_.x > 0.0f){
		xOverlap = Math::Abs(c1Right - c2Left);
	}

	if(dir_.y < 0.0f){
		yOverlap = Math::Abs(c1Bottom - c2Top);
	}else if(dir_.y > 0.0f){
		yOverlap = Math::Abs(c1Top - c2Bottom);
	}

	return Math::Sqrt((xOverlap * xOverlap) + (yOverlap * yOverlap));
}