#ifndef GADGET_GAME_OBJECT_H
#define GADGET_GAME_OBJECT_H

#include "Component.h"
#include "Math/Euler.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

namespace Gadget{
	struct Transform{
		constexpr Transform(const Vector3& pos_, const Quaternion& rot_ = Quaternion::Identity(), const Vector3& scale_ = Vector3::Fill(1.0f)) : position(pos_), rotation(rot_), scale(scale_){}
		Transform(const Vector3& pos_, const Euler& euler_, const Vector3& scale_ = Vector3::Fill(1.0f));

		Matrix4 GetTransformMatrix() const;

		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
	};

	class GameObject{
	public:
		GameObject(StringID name_ = SID("GameObject"));
		~GameObject();

		void Update(float deltaTime_);

		void AddComponent(Component* component_);

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache the result when possible
		template <class T> T* GetComponent() const{
			static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
			T* comp = nullptr;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(Component* c : components){
				comp = dynamic_cast<T*>(c);
				if(comp != nullptr){
					return comp;
				}
			}

			return nullptr;
		}

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache the results when possible
		template <class T> std::vector<T*> GetComponents() const{
			static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
			std::vector<T*> comps;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis
			for(Component* c : components){
				auto comp = dynamic_cast<T*>(c);
				if(comp != nullptr){
					comps.push_back(comp);
				}
			}

			return comps;
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

		void SetPosition(const Vector3& pos_){ transform.position = pos_; OnTransformModified(); }
		void SetPosition(float x_, float y_, float z_){ transform.position = Vector3(x_, y_, z_); OnTransformModified(); }

		void SetRotation(const Quaternion& rot_){ transform.rotation = rot_; OnTransformModified(); }
		void SetRotation(const Euler& rot_){ transform.rotation = rot_.ToQuaternion(); OnTransformModified(); }
		void SetRotation(float x_, float y_, float z_){ transform.rotation = Euler(x_, y_, z_).ToQuaternion(); OnTransformModified(); }

		void SetScale(float scale_){ transform.scale = Vector3::Fill(scale_); OnTransformModified(); }
		void SetScale(const Vector3& scale_){ transform.scale = scale_; OnTransformModified(); }

		void Translate(const Vector3& translation_){ transform.position += translation_; OnTransformModified(); }
		void Translate(float x_, float y_, float z_){ transform.position += Vector3(x_, y_, z_); OnTransformModified(); }

		void Rotate(const Quaternion& rotation_){ transform.rotation *= rotation_; OnTransformModified(); }
		void Rotate(const Euler& rotation_){ transform.rotation *= rotation_.ToQuaternion(); OnTransformModified(); }
		void Rotate(float x_, float y_, float z_){ transform.rotation *= Euler(x_, y_, z_).ToQuaternion(); OnTransformModified(); }

		static GameObject* FindWithTag(StringID tag_);
		static std::vector<GameObject*> FindObjectsWithTag(StringID tag_);

	protected:
		StringID name;
		std::vector<StringID> tags;
		Transform transform;
		std::vector<Component*> components;

		virtual void OnTransformModified();
	};
}

#endif //!GADGET_GAME_OBJECT_H