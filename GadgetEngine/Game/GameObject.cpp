#include "GameObject.h"

#include "App.h"
#include "Utils/Utils.h"

using namespace Gadget;

std::map<GUID, GameObject*> GameObjectCollection::guidMap = std::map<GUID, GameObject*>();

Transform::Transform(const Vector3& pos_, const Euler& euler_, const Vector3& scale_) : position(pos_), rotation(euler_.ToQuaternion()), scale(scale_){
	GADGET_BASIC_ASSERT(position.IsValid());
	GADGET_BASIC_ASSERT(euler_.IsValid());
	GADGET_BASIC_ASSERT(rotation.IsValid());
	GADGET_BASIC_ASSERT(scale.IsValid());
}

Matrix4 Transform::GetTransformMatrix() const{
	Matrix4 positionMatrix = Matrix4::Translate(position);
	Matrix4 rotationMatrix = rotation.ToMatrix4();
	Matrix4 scaleMatrix = Matrix4::Scale(scale);

	return (positionMatrix * (rotationMatrix * scaleMatrix));
}

GameObject::GameObject(StringID name_) : guid(GUID::Generate()), name(name_), transform(Vector3::Zero(), Quaternion::Identity(), Vector3::Fill(1.0f)), components(){
	GADGET_BASIC_ASSERT(name_ != StringID::None);
	GADGET_BASIC_ASSERT(guid != GUID::Invalid);

	GameObjectCollection::Add(this);

	GADGET_BASIC_ASSERT(GameObjectCollection::Get(guid) == this);
}

GameObject::GameObject(const GameObjectProperties& properties_) : guid(properties_.guid.ToNumber<uint64_t>()), name(properties_.name.ToStr()), tags(), transform(properties_.transform), components(){
	GADGET_BASIC_ASSERT(properties_.name.Value() != StringID::None);
	GADGET_BASIC_ASSERT(properties_.transform.position.IsValid());
	GADGET_BASIC_ASSERT(properties_.transform.rotation.IsValid());
	GADGET_BASIC_ASSERT(properties_.transform.scale.IsValid());
	for(const auto& tag : properties_.tags.Value()){
		GADGET_BASIC_ASSERT(tag.GetType() == Var::Type::String && tag.ToStr() != StringID::None);

		if(tag.GetType() == Var::Type::String){
			tags.push_back(tag.ToStr());
		}
	}

	if(properties_.guid.Value().ToNumber() == GUID::Invalid){
		guid = GUID::Generate();
	}

	GADGET_ASSERT(GameObjectCollection::Get(properties_.guid.ToNumber<uint64_t>()) == nullptr, "GameObject being deserialized with a GUID that's already in use!");

	GameObjectCollection::Add(this);

	GADGET_BASIC_ASSERT(GameObjectCollection::Get(guid) == this);
}

void GameObject::Update([[maybe_unused]] float deltaTime_){
	GADGET_BASIC_ASSERT(deltaTime_ >= 0.0f);

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
	components.clear();

	GADGET_BASIC_ASSERT(GameObjectCollection::Get(guid) == this);
	GameObjectCollection::Remove(this);
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
	GADGET_BASIC_ASSERT(tag_ != StringID::None);
	return Utils::Contains(tags, tag_);
}

void GameObject::AddTag(StringID tag_){
	GADGET_BASIC_ASSERT(tag_ != StringID::None);
	GADGET_ASSERT(!HasTag(tag_), "Tried adding tag [" + tag_.GetString() + "] to a GameObject that already has that tag!");
	tags.push_back(tag_);
}

GameObject* GameObject::FindWithTag(StringID tag_){
	GADGET_BASIC_ASSERT(tag_ != StringID::None);
	return App::GetInstance().GetSceneManager().CurrentScene()->FindWithTag(tag_);
}

std::vector<GameObject*> GameObject::FindObjectsWithTag(StringID tag_){
	GADGET_BASIC_ASSERT(tag_ != StringID::None);
	return App::GetInstance().GetSceneManager().CurrentScene()->FindObjectsWithTag(tag_);
}

void GameObject::OnTransformModified(){
	for(auto& c : components){
		GADGET_BASIC_ASSERT(c != nullptr);
		c->OnTransformModified();
	}

	GADGET_BASIC_ASSERT(transform.position.IsValid());
	GADGET_BASIC_ASSERT(transform.rotation.IsValid());
	GADGET_BASIC_ASSERT(transform.scale.IsValid());
}