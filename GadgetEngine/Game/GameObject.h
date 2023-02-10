#ifndef GADGET_GAME_OBJECT_H
#define GADGET_GAME_OBJECT_H

#include "Component.h"
#include "Math/Euler.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

namespace Gadget{
	struct Transform{
		Transform(const Vector3& pos_, const Quaternion& rot_ = Quaternion::Identity(), const Vector3& scale_ = Vector3::Fill(1.0f));
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

		void AddComponent(Component* component_);

		//THIS FUNCTION IS SLOW - Avoid calling it wherever possible
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

		StringID GetName() const{ return name; }
		Vector3 GetPosition() const{ return transform.position; }
		Quaternion GetRotation() const{ return transform.rotation; }
		Vector3 GetScale() const{ return transform.scale; }
		Matrix4 GetTransformMatrix() const{ return transform.GetTransformMatrix(); }

		void SetName(StringID newName_){ name = newName_; }

		void SetPosition(const Vector3& pos_){ transform.position = pos_; }
		void SetPosition(float x_, float y_, float z_){ transform.position += Vector3(x_, y_, z_); }

		void SetRotation(const Quaternion& rot_){ transform.rotation = rot_; }
		void SetRotation(const Euler& rot_){ transform.rotation = rot_.ToQuaternion(); }
		void SetRotation(float x_, float y_, float z_){ transform.rotation = Euler(x_, y_, z_).ToQuaternion(); }

		void SetScale(float scale_){ transform.scale = Vector3::Fill(scale_); }
		void SetScale(const Vector3& scale_){ transform.scale = scale_; }

		void Translate(const Vector3& translation_){ transform.position += translation_; }
		void Translate(float x_, float y_, float z_){ transform.position += Vector3(x_, y_, z_); }

		void Rotate(const Quaternion& rotation_){ transform.rotation *= rotation_; }
		void Rotate(const Euler& rotation_){ transform.rotation *= rotation_.ToQuaternion(); }
		void Rotate(float x_, float y_, float z_){ transform.rotation *= Euler(x_, y_, z_).ToQuaternion(); }

	protected:
		StringID name;
		Transform transform;
		std::vector<Component*> components;
	};
}

#endif //!GADGET_GAME_OBJECT_H