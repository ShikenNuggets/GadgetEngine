#include "CollisionSystem.h"

#include "Game/GameObject.h"

using namespace Gadget;

bool CollisionSystem::TestCollision(const BoxCollider2D& cl1, const BoxCollider2D& cl2){
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