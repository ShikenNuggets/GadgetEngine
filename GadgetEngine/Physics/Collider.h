#ifndef GADGET_COLLIDER_H
#define GADGET_COLLIDER_H

#include "Game/Component.h"

namespace Gadget{
	class Collider : public Component{
	public:
		Collider(GameObject* parent_) : Component(parent_){}
	};
}

#endif //!GADGET_COLLIDER_H