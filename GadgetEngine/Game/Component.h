#ifndef GADGET_COMPONENT_H
#define GADGET_COMPONENT_H

#include "Debug.h"
#include "Math/Quaternion.h"
#include "Utils/GUID.h"
#include "Utils/Utils.h"

namespace Gadget{
	//Forward Declarations
	struct Transform;
	class GameObject;

	class Component{
	public:
		Component(GameObject* parent_) : guid(GUID::Generate()), parent(parent_), isActivated(false){ GADGET_BASIC_ASSERT(parent_ != nullptr); }
		virtual ~Component(){}

		//Runs the first frame that the object is activated
		virtual void OnActivated(){
			isActivated = true;
		}

		//Runs when the parent GameObject's transform is modified (i.e. translated, rotated, scale set to a new value)
		virtual void OnTransformModified(){}

		GUID GetGUID() const{ return guid; }
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
		GUID guid;
		GameObject* parent;
		bool isActivated;
	};

	//TODO - Thread safety
	template <class T>
	class ComponentCollection{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		private:
			std::map<GUID, T*> guidMap;

		public:
			ComponentCollection(){}

			void Add(T* element_){
				static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
				GADGET_BASIC_ASSERT(element_ != nullptr);
				guidMap.emplace(element_->GetParent()->GetGUID(), element_);
			}

			void Remove(GUID objectGuid_){
				guidMap.erase(objectGuid_);
			}

			void Remove(T* element_){
				GADGET_BASIC_ASSERT(Utils::ContainsValue(guidMap, element_));
				for(const auto& e : guidMap){
					if(e.second == element_){
						guidMap.erase(e.first);
						return;
					}
				}

				return;
			}

			T* Get(GUID objectGuid_) const{
				return nullptr;
				return Utils::GetValue(guidMap, objectGuid_);
			}
	};
}

#endif //!GADGET_COMPONENT_H