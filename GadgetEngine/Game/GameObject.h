#ifndef GADGET_GAME_OBJECT_H
#define GADGET_GAME_OBJECT_H

#include "Data/Array.h"
#include "Game/Component.h"
#include "Math/Euler.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "Utils/GUID.h"
#include "Utils/NamedVar.h"

namespace Gadget{
	struct Transform{
		constexpr Transform(const Vector3& pos_, const Quaternion& rot_ = Quaternion::Identity(), const Vector3& scale_ = Vector3::Fill(1.0f)) : position(pos_), rotation(rot_), scale(scale_){}
		Transform(const Vector3& pos_, const Euler& euler_, const Vector3& scale_ = Vector3::Fill(1.0f));

		Matrix4 GetTransformMatrix() const;

		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		inline constexpr Vector3 Forward() const{ return rotation * Vector3::Forward(); }
		inline constexpr Vector3 Up() const{ return rotation * Vector3::Up(); }
		inline constexpr Vector3 Right() const{ return rotation * Vector3::Right(); }
	};

	struct GameObjectProperties{
		GameObjectProperties(GUID guid_, StringID name_, const std::vector<std::string>& tags_, const Transform& transform_)
			: guid(SID("GUID"), guid_.Id())
			, name(SID("Name"), name_)
			, tags(SID("Tags"), tags_)
			, transform(transform_){}

		NamedVar guid;
		NamedVar name;
		VarList tags;
		Transform transform;
	};

	class GameObject{
	public:
		GameObject(StringID name_ = SID("GameObject"));
		GameObject(const GameObjectProperties& properties_);
		virtual ~GameObject();

		void Update(float deltaTime_);

		void AddComponent(Component* component_);

		GameObjectProperties GetProperties() const{
			return GameObjectProperties(guid, name, StringID::ToStringList(tags), transform);
		}

		GUID GetGUID() const{ return guid; }

		//If T doesn't have a ComponentCollection, this function will be significantly slower
		template <class T> T* GetComponent() const{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
			if constexpr(HasComponentCollection<T>){
				return T::GetCollection().Get(GetGUID());
			}else{
				// Ironically, this logging is potentially very bad for performance, so best to only do it in debug builds
				#ifdef GADGET_DEBUG
				GADGET_LOG_WARNING(SID("PERFORMANCE"), typeid(T).name() + std::string(" does not have a Component Collection. You can optimize GameObject::GetComponent by giving it one"));
				#endif //GADGET_DEBUG 

				//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
				//This seems to be the simplest way to do this generically, but you should set up ComponentCollections to avoid this
				for(Component* c : components){
					T* comp = dynamic_cast<T*>(c);
					if(comp != nullptr){
						return comp;
					}
				}

				return nullptr;
			}
		}

		Component* GetComponent(GUID componentGuid_){
			for(const auto& c : components){
				if(c->GetGUID() == componentGuid_){
					return c;
				}
			}

			return nullptr;
		}

		//Unlike GetComponent, this function won't play nice with the whole class hierarchy
		//If T doesn't have a ComponentCollection, this function will be significantly slower
		template <class T>
		Array<T*> GetComponents() const{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
			if constexpr(HasComponentCollection<T>){
				return T::GetCollection().GetComponents(guid);
			}else{
				// Ironically, this logging is potentially very bad for performance, so best to only do it in debug builds
				#ifdef GADGET_DEBUG
				GADGET_LOG_WARNING(SID("PERFORMANCE"), typeid(T).name() + std::string(" does not have a Component Collection. You can optimize GameObject::GetComponents by giving it one"));
				#endif //GADGET_DEBUG

				//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
				//This seems to be the simplest way to do this generically, but you should set up ComponentCollections to avoid this
				Array<T*> comps;
				for(Component* c : components){
					auto comp = dynamic_cast<T*>(c);
					if(comp != nullptr){
						comps.Add(comp);
					}
				}

				comps.ShrinkToFit();
				return comps;
			}
		}

		StringID GetName() const{ return name; }
		bool HasTag(StringID tag_) const;
		std::vector<StringID> GetTags() const{ return tags; }
		Vector3 GetPosition() const{ return transform.position; }
		Quaternion GetRotation() const{ return transform.rotation; }
		Vector3 GetScale() const{ return transform.scale; }
		const Transform& GetTransform() const{ return transform; }
		Matrix4 GetTransformMatrix() const{ return transform.GetTransformMatrix(); }

		void SetName(StringID newName_){ name = newName_; }
		void AddTag(StringID tag_);

		void SetLifeTime(float lifeTime);
		inline bool HasLifeTimeEnded() const{ return lifeTime >= 0.0f && lifeTimeTimer >= lifeTime; }

		void Destroy();

		void SetPosition(const Vector3& pos_){ transform.position = pos_; OnTransformModified(); }
		void SetPosition(float x_, float y_, float z_){ transform.position = Vector3(x_, y_, z_); OnTransformModified(); }

		void SetRotation(const Quaternion& rot_){ transform.rotation = rot_; OnTransformModified(); }
		void SetRotation(const Euler& rot_){ transform.rotation = rot_.ToQuaternion(); OnTransformModified(); }
		void SetRotation(float x_, float y_, float z_){ transform.rotation = Euler(x_, y_, z_).ToQuaternion(); OnTransformModified(); }

		void SetScale(float scale_){ transform.scale = Vector3::Fill(scale_); OnTransformModified(); }
		void SetScale(float x_, float y_, float z_){ transform.scale = Vector3(x_, y_, z_); OnTransformModified(); }
		void SetScale(const Vector3& scale_){ transform.scale = scale_; OnTransformModified(); }

		void Translate(const Vector3& translation_){ transform.position += translation_; OnTransformModified(); }
		void Translate(float x_, float y_, float z_){ transform.position += Vector3(x_, y_, z_); OnTransformModified(); }

		void Rotate(const Quaternion& rotation_){ transform.rotation *= rotation_; OnTransformModified(); }
		void Rotate(const Euler& rotation_){ transform.rotation *= rotation_.ToQuaternion(); OnTransformModified(); }
		void Rotate(float x_, float y_, float z_){ transform.rotation *= Euler(x_, y_, z_).ToQuaternion(); OnTransformModified(); }

		static GameObject* FindWithTag(StringID tag_);
		static std::vector<GameObject*> FindObjectsWithTag(StringID tag_);

	protected:
		GUID guid;
		StringID name;
		std::vector<StringID> tags;
		Transform transform;
		std::vector<Component*> components;
		float lifeTime;
		float lifeTimeTimer;

		virtual void OnTransformModified();
	};

	//TODO - Thread safety
	class GameObjectCollection{
		private:
			static std::map<GUID, GameObject*> guidMap;

		public:
			GameObjectCollection() = default;

			static void Add(GameObject* element_){
				GADGET_BASIC_ASSERT(element_ != nullptr);
				if(element_ == nullptr){
					GADGET_LOG_WARNING(SID("GAMEOBJECT"), "Tried to add nullptr to the GameObjectCollection!");
					return;
				}

				guidMap.emplace(element_->GetGUID(), element_);
			}

			static void Remove(GUID objectGuid_){
				GADGET_BASIC_ASSERT(Utils::ContainsKey(guidMap, objectGuid_));
				guidMap.erase(objectGuid_);
			}

			static void Remove(GameObject* element_){
				GADGET_BASIC_ASSERT(element_ != nullptr);
				if(element_ == nullptr){
					GADGET_LOG_WARNING(SID("GAMEOBJECT"), "Tried to remove from the GameObjectCollection!");
					return;
				}

				GADGET_BASIC_ASSERT(Utils::ContainsKey(guidMap, element_->GetGUID()));

				Remove(element_->GetGUID());
			}

			static GameObject* Get(GUID objectGuid_){
				if(!Utils::ContainsKey(guidMap, objectGuid_)){
					return nullptr;
				}

				return guidMap.at(objectGuid_);
			}

			static inline size_t Size(){ return guidMap.size(); }
	};
}

#endif //!GADGET_GAME_OBJECT_H