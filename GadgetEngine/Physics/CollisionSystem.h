#ifndef GADGET_COLLISION_SYSTEM_H
#define GADGET_COLLISION_SYSTEM_H

#include "BoxCollider2D.h"

namespace Gadget{
	namespace CollisionSystem{
		bool TestCollision(const BoxCollider2D& cl1, const BoxCollider2D& cl2);
	};
}

#endif //!GADGET_COLLISION_SYSTEM_H