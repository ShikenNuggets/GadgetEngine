#ifndef GADGET_COLLISION_SYSTEM_H
#define GADGET_COLLISION_SYSTEM_H

#include "Math/Vector.h"
#include "Physics/BoxCollider2D.h"
#include "Utils/Utils.h"

namespace Gadget{
	struct Collision{
		Collision() : didCollide(false), otherName(0), otherTags(), otherPos(), otherScale(), collisionVector(), overlapAmount(0.0f), isTrigger(false){}

		bool didCollide;
		StringID otherName;
		std::vector<StringID> otherTags;
		Vector3 otherPos;
		Vector3 otherScale;
		Vector3 collisionVector;
		float overlapAmount;
		bool isTrigger;

		constexpr bool HasTag(StringID tag_) const{
			GADGET_BASIC_ASSERT(tag_ != StringID::None);
			return Utils::Contains(otherTags, tag_);
		}
	};

	namespace CollisionSystem{
		bool TestCollision(const BoxCollider2D& cl1, const BoxCollider2D& cl2);

		Vector3 CalculateContactPoint(const Collider* col1_, const Collider* col2_);
		float CalculateOverlapAmount(const Vector3& dir_, const Collider* col1_, const Collider* col2_);

		Vector3 CalculateContactPoint(const BoxCollider2D& cl1, const BoxCollider2D& cl2);
		float CalculateOverlapAmount(const Vector3& dir_, const BoxCollider2D& cl1, const BoxCollider2D& cl2);
	};
}

#endif //!GADGET_COLLISION_SYSTEM_H