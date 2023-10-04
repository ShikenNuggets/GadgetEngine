#ifndef GADGET_COMPONENT_H
#define GADGET_COMPONENT_H

#include "Debug.h"

namespace Gadget{
	//Forward Declarations
	class GameObject;

	class Component{
	public:
		Component(GameObject* parent_) : parent(parent_), isActivated(false){ GADGET_BASIC_ASSERT(parent_ != nullptr); }
		virtual ~Component(){}

		//Runs the first frame that the object is activated
		virtual void OnActivated(){
			isActivated = true;
		}

		GameObject* GetParent() const{ return parent; }
		bool IsActivated() const{ return isActivated; }

	protected:
		GameObject* parent;
		bool isActivated;
	};
}

#endif //!GADGET_COMPONENT_H