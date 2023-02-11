#ifndef GADGET_COLLIDER_H
#define GADGET_COLLIDER_H

#include "Game/Component.h"

namespace Gadget{
	class Collider : public Component{
	public:
		Collider(GameObject* parent_, bool isTrigger_) : Component(parent_), isTrigger(isTrigger_){}

		bool IsTrigger() const{ return isTrigger; }

	protected:
		bool isTrigger;
	};
}

#endif //!GADGET_COLLIDER_H