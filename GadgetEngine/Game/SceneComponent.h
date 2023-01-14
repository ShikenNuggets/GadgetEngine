#ifndef GADGET_SCENE_COMPONENT_H
#define GADGET_SCENE_COMPONENT_H

#include "Debug.h"

namespace Gadget{
	//Forward declaration
	class Scene;

	class SceneComponent{
	public:
		SceneComponent(Scene* parent_) : parent(parent_){ GADGET_BASIC_ASSERT(parent_ != nullptr); }
		virtual ~SceneComponent(){}

		const Scene* GetParent() const{ return parent; }

	protected:
		Scene* parent;
	};
}

#endif //!GADGET_SCENE_COMPONENT_H