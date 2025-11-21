#ifndef GADGET_SCENE_COMPONENT_H
#define GADGET_SCENE_COMPONENT_H

#include "Debug.h"
#include "Utils/GUID.h"

namespace Gadget{
	//Forward declaration
	class Scene;

	class SceneComponent{
	public:
		SceneComponent(Scene* parent_) : guid(GUID::Generate()), parent(parent_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(guid != GUID::Invalid);
		}

		virtual ~SceneComponent() = default;

		virtual void OnUpdate([[maybe_unused]] float deltaTime_){}

		GUID GetGUID() const{ return guid; }
		const Scene* GetParent() const{ return parent; }

	protected:
		GUID guid;
		Scene* parent;
	};
}

#endif //!GADGET_SCENE_COMPONENT_H