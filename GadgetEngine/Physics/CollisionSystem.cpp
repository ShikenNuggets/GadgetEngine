#include "CollisionSystem.h"

#include "Game/GameObject.h"

using namespace Gadget;

bool CollisionSystem::TestCollision(const BoxCollider2D& cl1, const BoxCollider2D& cl2){
	GADGET_BASIC_ASSERT(cl1.GetParent() != nullptr);
	GADGET_BASIC_ASSERT(cl2.GetParent() != nullptr);

	//This assumes axis-aligned bounding boxes... TODO - allow oriented bounding boxes
	float c1Left =		cl1.GetParent()->GetPosition().x - (cl1.GetParent()->GetScale().x * (cl1.GetWidth() / 2.0f));
	float c1Right =		cl1.GetParent()->GetPosition().x + (cl1.GetParent()->GetScale().x * (cl1.GetWidth() / 2.0f));
	float c1Bottom =	cl1.GetParent()->GetPosition().y - (cl1.GetParent()->GetScale().y * (cl1.GetHeight() / 2.0f));
	float c1Top =		cl1.GetParent()->GetPosition().y + (cl1.GetParent()->GetScale().y * (cl1.GetHeight() / 2.0f));

	float c2Left =		cl2.GetParent()->GetPosition().x - (cl2.GetParent()->GetScale().x * (cl2.GetWidth() / 2.0f));
	float c2Right =		cl2.GetParent()->GetPosition().x + (cl2.GetParent()->GetScale().x * (cl2.GetWidth() / 2.0f));
	float c2Bottom =	cl2.GetParent()->GetPosition().y - (cl2.GetParent()->GetScale().y * (cl2.GetHeight() / 2.0f));
	float c2Top =		cl2.GetParent()->GetPosition().y + (cl2.GetParent()->GetScale().y * (cl2.GetHeight() / 2.0f));

	return (c1Left <= c2Right && c1Right >= c2Left && c1Bottom <= c2Top && c1Top >= c2Bottom);
}

float CollisionSystem::CalculateOverlapAmount(const Vector3& dir_, const BoxCollider2D& cl1, const BoxCollider2D& cl2){
	float c1Left = cl1.GetParent()->GetPosition().x - (cl1.GetParent()->GetScale().x * (cl1.GetWidth() / 2.0f));
	float c1Right = cl1.GetParent()->GetPosition().x + (cl1.GetParent()->GetScale().x * (cl1.GetWidth() / 2.0f));
	float c1Bottom = cl1.GetParent()->GetPosition().y - (cl1.GetParent()->GetScale().y * (cl1.GetHeight() / 2.0f));
	float c1Top = cl1.GetParent()->GetPosition().y + (cl1.GetParent()->GetScale().y * (cl1.GetHeight() / 2.0f));

	float c2Left = cl2.GetParent()->GetPosition().x - (cl2.GetParent()->GetScale().x * (cl2.GetWidth() / 2.0f));
	float c2Right = cl2.GetParent()->GetPosition().x + (cl2.GetParent()->GetScale().x * (cl2.GetWidth() / 2.0f));
	float c2Bottom = cl2.GetParent()->GetPosition().y - (cl2.GetParent()->GetScale().y * (cl2.GetHeight() / 2.0f));
	float c2Top = cl2.GetParent()->GetPosition().y + (cl2.GetParent()->GetScale().y * (cl2.GetHeight() / 2.0f));

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