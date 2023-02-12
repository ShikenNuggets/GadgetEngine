#include "CollisionSystem.h"

#include "Game/GameObject.h"
#include "Math/MathObjects.h"

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

Vector3 CollisionSystem::CalculateContactPoint(const BoxCollider2D& cl1, const BoxCollider2D& cl2){
	float c1Left = cl1.GetParent()->GetPosition().x - (cl1.GetParent()->GetScale().x * (cl1.GetWidth() / 2.0f));
	float c1Right = cl1.GetParent()->GetPosition().x + (cl1.GetParent()->GetScale().x * (cl1.GetWidth() / 2.0f));
	float c1Bottom = cl1.GetParent()->GetPosition().y - (cl1.GetParent()->GetScale().y * (cl1.GetHeight() / 2.0f));
	float c1Top = cl1.GetParent()->GetPosition().y + (cl1.GetParent()->GetScale().y * (cl1.GetHeight() / 2.0f));

	Rect c1Rect = Rect(
		cl1.GetParent()->GetPosition().x,
		cl1.GetParent()->GetPosition().y,
		cl1.GetParent()->GetScale().x * cl1.GetWidth(),
		cl1.GetParent()->GetScale().y * cl1.GetHeight()
	);

	Rect c2Rect = Rect(
		cl2.GetParent()->GetPosition().x,
		cl2.GetParent()->GetPosition().y,
		cl2.GetParent()->GetScale().x * cl2.GetWidth(),
		cl2.GetParent()->GetScale().y * cl2.GetHeight()
	);

	bool topLeft = c2Rect.Intersects(c1Left, c1Top);
	bool topRight = c2Rect.Intersects(c1Right, c1Top);
	bool bottomLeft = c2Rect.Intersects(c1Left, c1Bottom);
	bool bottomRight = c2Rect.Intersects(c1Right, c1Bottom);

	if(topLeft && topRight && bottomLeft && bottomRight
		|| !topLeft && !topRight && !bottomLeft && !bottomRight){
		return cl1.GetParent()->GetPosition();
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

	return cl1.GetParent()->GetPosition();
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