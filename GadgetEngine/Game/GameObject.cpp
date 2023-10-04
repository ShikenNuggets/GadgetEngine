#include "GameObject.h"

#include "Utils/Utils.h"

using namespace Gadget;

Transform::Transform(const Vector3& pos_, const Euler& euler_, const Vector3& scale_) : position(pos_), rotation(euler_.ToQuaternion()), scale(scale_){}

Matrix4 Transform::GetTransformMatrix() const{
	Matrix4 positionMatrix = Matrix4::Translate(position);
	Matrix4 rotationMatrix = rotation.ToMatrix4();
	Matrix4 scaleMatrix = Matrix4::Scale(scale);

	return (positionMatrix * (rotationMatrix * scaleMatrix));
}

GameObject::GameObject(StringID name_) : transform(Vector3::Zero(), Quaternion::Identity(), Vector3::Fill(1.0f)), components(), name(name_){}

void GameObject::Update(float deltaTime_){
	for(const auto& component : components){
		//TODO - There is probably a better approach to this than checking every component every frame...
		if(!component->IsActivated()){
			component->OnActivated();
		}
	}
}

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

bool GameObject::HasTag(StringID tag_) const{
	return Utils::Contains(tags, tag_);
}

void GameObject::AddTag(StringID tag_){
	GADGET_ASSERT(!HasTag(tag_), "Tried adding tag [" + tag_.GetString() + "] to a GameObject that already has that tag!");
	tags.push_back(tag_);
}