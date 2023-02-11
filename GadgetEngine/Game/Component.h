#ifndef GADGET_COMPONENT_H
#define GADGET_COMPONENT_H

#include "Debug.h"

namespace Gadget{
	//Forward Declarations
	class GameObject;

	class Component{
	public:
		Component(GameObject* parent_) : parent(parent_){ GADGET_BASIC_ASSERT(parent_ != nullptr); }
		virtual ~Component(){}

		GameObject* GetParent() const{ return parent; }

	protected:
		GameObject* parent;
	};
}

#endif //!GADGET_COMPONENT_H