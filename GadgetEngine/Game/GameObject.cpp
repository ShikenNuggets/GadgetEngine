#include "GameObject.h"

#include "Utils/Utils.h"

using namespace Gadget;

Transform::Transform(const Vector3& pos_, const Quaternion& rot_, const Vector3& scale_) : position(pos_), rotation(rot_), scale(scale_){}

Transform::Transform(const Vector3& pos_, const Euler& euler_, const Vector3& scale_) : position(pos_), rotation(euler_.ToQuaternion()), scale(scale_){}

Matrix4 Transform::GetTransform() const{
	Matrix4 positionMatrix = Matrix4::Translate(position);
	Matrix4 rotationMatrix = rotation.ToMatrix4();
	Matrix4 scaleMatrix = Matrix4::Scale(scale);

	return (positionMatrix * (rotationMatrix * scaleMatrix));
}

GameObject::GameObject(StringID name_) : transform(Vector3::Zero(), Quaternion::Identity(), Vector3::Fill(1.0f)), components(), name(name_){}

GameObject::~GameObject(){
	for(auto c : components){
		delete c;
		c = nullptr;
	}
}

void GameObject::AddComponent(Component* component_){
	GADGET_BASIC_ASSERT(component_ != nullptr);
	if(component_ == nullptr){
		return;
	}

	GADGET_ASSERT(!Utils::Contains(components, component_), "Component is being added to the component list for this object multiple times!");
	components.push_back(component_);
}