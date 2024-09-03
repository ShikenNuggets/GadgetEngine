#ifndef GADGET_COMPONENT_H
#define GADGET_COMPONENT_H

#include "Debug.h"
#include "Math/Quaternion.h"
#include "Utils/GUID.h"
#include "Utils/NamedVar.h"
#include "Utils/Utils.h"

namespace Gadget{
	//Forward Declarations
	struct Transform;
	class GameObject;

	struct ComponentProperties{
		ComponentProperties(StringID typeName_, GUID guid_, GUID parentGuid_) : typeName(typeName_), guid(guid_), parentGuid(parentGuid_), isActivated(true), variables(SID("Variables")){}

		StringID typeName;
		GUID guid;
		GUID parentGuid;
		bool isActivated;
		NamedVarList variables;
	};

	class Component{
	public:
		Component(StringID typeName_, GameObject* parent_) : typeName(typeName_), guid(GUID::Generate()), parent(parent_), isActivated(false){
			GADGET_BASIC_ASSERT(typeName_ != StringID::None);
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(guid != GUID::Invalid);
		}

		Component(StringID typeName_, GUID parentGUID_);
		Component(const ComponentProperties& props_);

		virtual ~Component(){}

		//Runs the first frame that the object is activated
		virtual void OnActivated(){
			isActivated = true;
		}

		//Runs when the parent GameObject's transform is modified (i.e. translated, rotated, scale set to a new value)
		virtual void OnTransformModified(){}

		StringID GetType() const{ return typeName; }
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

		virtual ComponentProperties Serialize() const;

		template <class T>
		static Component* DeserializeToNewComponent(const ComponentProperties& props_)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
			return new T(props_);
		}

	protected:
		const StringID typeName;

		GUID guid;
		GameObject* parent;
		bool isActivated;

		virtual void Deserialize(const ComponentProperties& props_);
	};

	//TODO - Thread safety
	template <class T>
	class ComponentCollection{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		private:
			std::map<GUID, std::vector<T*>> guidMap;

		public:
			ComponentCollection() noexcept = default;

			void Add(T* element_){
				GADGET_BASIC_ASSERT(element_ != nullptr);
				if(element_ == nullptr){
					GADGET_LOG_WARNING(SID("COMP"), "Tried to add nullptr to the ComponentCollection!");
					return;
				}

				GADGET_BASIC_ASSERT(element_->GetGUID() != GUID::Invalid);
				GADGET_BASIC_ASSERT(element_->GetParent() != nullptr);
				GADGET_BASIC_ASSERT(element_->GetParent()->GetGUID() != GUID::Invalid);

				GUID objectGuid = element_->GetParent()->GetGUID();
				if(Utils::ContainsKey(guidMap, objectGuid)){
					guidMap.at(objectGuid).push_back(element_);
				}else{
					guidMap.emplace(element_->GetParent()->GetGUID(), std::vector<T*>{ element_ });
				}
			}

			void Remove(GUID objectGuid_){
				GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
				guidMap.erase(objectGuid_);
			}

			void Remove(T* element_){
				GADGET_BASIC_ASSERT(element_ != nullptr);
				if(element_ == nullptr){
					GADGET_LOG_WARNING(SID("COMP"), "Tried to remove nullptr from the ComponentCollection!");
					return;
				}

				GADGET_BASIC_ASSERT(element_->GetGUID() != GUID::Invalid);
				GADGET_BASIC_ASSERT(element_->GetParent() != nullptr);
				GADGET_BASIC_ASSERT(element_->GetParent()->GetGUID() != GUID::Invalid);
				GADGET_BASIC_ASSERT(Utils::ContainsKey(guidMap, element_->GetParent()->GetGUID()));

				auto& vec = guidMap.at(element_->GetParent()->GetGUID());
				vec.erase(std::remove(vec.begin(), vec.end(), element_), vec.end());
				if(vec.empty()){
					Remove(element_->GetParent()->GetGUID());
				}
			}

			T* Get(GUID objectGuid_) const{
				GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);

				if(!Utils::ContainsKey(guidMap, objectGuid_)){
					return nullptr;
				}

				const auto& vec = guidMap.at(objectGuid_);
				if(vec.empty()){
					return nullptr;
				}

				return vec.front();
			}

			std::vector<T*> GetComponents(GUID objectGuid_) const{
				GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);

				if(!Utils::ContainsKey(guidMap, objectGuid_)){
					return std::vector<T*>();
				}

				return guidMap.at(objectGuid_);
			}

			size_t Count() const{ return guidMap.size(); }
	};
}

#endif //!GADGET_COMPONENT_H