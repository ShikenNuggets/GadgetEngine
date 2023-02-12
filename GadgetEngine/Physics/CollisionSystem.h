#ifndef GADGET_COLLISION_SYSTEM_H
#define GADGET_COLLISION_SYSTEM_H

#include "BoxCollider2D.h"
#include "Math/Vector.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct Collision{
		Collision() : didCollide(false), otherTags(), otherPos(), otherScale(), collisionVector(), overlapAmount(0.0f), isTrigger(false){}

		bool didCollide;
		std::vector<StringID> otherTags;
		Vector3 otherPos;
		Vector3 otherScale;
		Vector3 collisionVector;
		float overlapAmount;
		bool isTrigger;

		constexpr bool HasTag(StringID tag_) const{ return Utils::Contains(otherTags, tag_); }
	};

	namespace CollisionSystem{
		bool TestCollision(const BoxCollider2D& cl1, const BoxCollider2D& cl2);
		Vector3 CalculateContactPoint(const BoxCollider2D& cl1, const BoxCollider2D& cl2);
		float CalculateOverlapAmount(const Vector3& dir_, const BoxCollider2D& cl1, const BoxCollider2D& cl2);
	};
}

#endif //!GADGET_COLLISION_SYSTEM_H