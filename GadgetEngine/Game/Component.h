#ifndef GADGET_COMPONENT_H
#define GADGET_COMPONENT_H

#include "Debug.h"
#include "Math/Quaternion.h"

namespace Gadget{
	//Forward Declarations
	struct Transform;
	class GameObject;

	class Component{
	public:
		Component(GameObject* parent_) : parent(parent_), isActivated(false){ GADGET_BASIC_ASSERT(parent_ != nullptr); }
		virtual ~Component(){}

		//Runs the first frame that the object is activated
		virtual void OnActivated(){
			isActivated = true;
		}

		//Runs when the parent GameObject's transform is modified (i.e. translated, rotated, scale set to a new value)
		virtual void OnTransformModified(){}

		GameObject* GetParent() const{ return parent; }
		bool IsActivated() const{ return isActivated; }

		//GameObject Helper Functions
		StringID GetParentName() const;
		bool HasTag(StringID tag_) const;
		std::vector<StringID> GetTags() const;

		Vector3 GetPosition() const;
		Quaternion GetRotation() const;
		Vector3 GetScale() const;
		const Transform& GetTransform() const;
		Matrix4 GetTransformMatrix() const;

	protected:
		GameObject* parent;
		bool isActivated;
	};
}

#endif //!GADGET_COMPONENT_H