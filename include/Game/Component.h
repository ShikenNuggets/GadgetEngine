#ifndef GADGET_COMPONENT_H
#define GADGET_COMPONENT_H

#include "Debug.h"
#include "Data/Array.h"
#include "Data/HashTable.h"
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
		virtual ~Component() = default;

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
		static Component* DeserializeToNewComponent(const ComponentProperties& props_){
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
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
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		private:
			HashTable<GUID, Array<T*>> guidMap;

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
				if(!guidMap.Contains(objectGuid)){
					guidMap.Add(element_->GetParent()->GetGUID(), {});
				}

				guidMap[objectGuid].Add(element_);
			}

			void Remove(GUID objectGuid_){
				GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
				guidMap.RemoveAt(objectGuid_);
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
				GADGET_BASIC_ASSERT(guidMap.Contains(element_->GetParent()->GetGUID()));

				auto& arr = guidMap[element_->GetParent()->GetGUID()];
				arr.Remove(element_);
				if(arr.IsEmpty()){
					Remove(element_->GetParent()->GetGUID());
				}
			}

			T* GetAny() const{
				for(const auto& arr : guidMap){
					for(auto* c : arr.value){
						return c;
					}
				}

				return nullptr;
			}

			T* Get(GUID objectGuid_) const{
				GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);

				if(!guidMap.Contains(objectGuid_)){
					return nullptr;
				}

				const auto& arr = guidMap[objectGuid_];
				if(arr.IsEmpty()){
					return nullptr;
				}

				return arr[0];
			}

			//TODO - Could we avoid this copy?
			Array<T*> GetComponents(GUID objectGuid_) const{
				GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
				
				if(!guidMap.Contains(objectGuid_)){
					return Array<T*>();
				}

				return guidMap[objectGuid_];
			}

			Array<T*> GetAllComponents() const{
				Array<T*> allComps;
				allComps.Reserve(guidMap.Size());
				for(const auto& arr : guidMap){
					allComps.Reserve(allComps.Size() + arr.value.Size());
					for(auto* comp : arr.value){
						allComps.Add(comp);
					}
				}

				return allComps;
			}

			void GetAllComponents(Array<T*>& inArray_) const{
				inArray_.Reserve(guidMap.Size());
				for(const auto& arr : guidMap){
					inArray_.Reserve(inArray_.Size() + arr.value.Size());
					for(auto* comp : arr.value){
						inArray_.Add(comp);
					}
				}
			}

			size_t Count() const{ return guidMap.Size(); }
	};

	template <typename T>
	concept HasComponentCollection = std::is_base_of_v<Component, T> &&
		requires{{ T::GetCollection() } -> std::same_as<const ComponentCollection<T>&>; };
}

#endif //!GADGET_COMPONENT_H