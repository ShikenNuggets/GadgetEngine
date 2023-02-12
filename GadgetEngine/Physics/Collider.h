#ifndef GADGET_COLLIDER_H
#define GADGET_COLLIDER_H

#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Math/Vector.h"

namespace Gadget{
	class Collider : public Component{
	public:
		Collider(GameObject* parent_, bool isTrigger_) : Component(parent_), isTrigger(isTrigger_){}

		bool IsTrigger() const{ return isTrigger; }

		virtual Vector3 GetColliderSize() const{ return parent->GetScale(); }

	protected:
		bool isTrigger;
	};
}

#endif //!GADGET_COLLIDER_H