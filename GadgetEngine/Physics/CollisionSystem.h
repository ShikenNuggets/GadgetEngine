#ifndef GADGET_COLLISION_SYSTEM_H
#define GADGET_COLLISION_SYSTEM_H

#include "BoxCollider2D.h"

namespace Gadget{
	struct Collision{
		Collision(std::vector<StringID> otherTags_, bool isTriggerCollision_) : otherTags(otherTags_), isTrigger(isTriggerCollision_){}

		//TODO - We'll need more collision info here for physics response
		std::vector<StringID> otherTags;
		bool isTrigger;
	};

	namespace CollisionSystem{
		bool TestCollision(const BoxCollider2D& cl1, const BoxCollider2D& cl2);
	};
}

#endif //!GADGET_COLLISION_SYSTEM_H