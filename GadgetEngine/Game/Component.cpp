#include "Component.h"

#include "Game/GameObject.h"

using namespace Gadget;

Component::Component(StringID typeName_, GUID parentGUID_) : typeName(typeName_), guid(GUID::Generate()), isActivated(false){
	GADGET_BASIC_ASSERT(typeName_ != StringID::None);
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(guid != GUID::Invalid);

	parent = GameObjectCollection::Get(parentGUID_);
	GADGET_BASIC_ASSERT(parent != nullptr);
}

Component::Component(const ComponentProperties& props_) : typeName(props_.typeName), guid(GUID::Invalid), isActivated(false){
	Component::Deserialize(props_);
}

StringID Component::GetParentName() const{
	GADGET_BASIC_ASSERT(parent != nullptr);

	return parent->GetName();
}

bool Component::HasTag(StringID tag_) const{
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(tag_ != StringID::None);

	return parent->HasTag(tag_);
}

std::vector<StringID> Component::GetTags() const{
	GADGET_BASIC_ASSERT(parent != nullptr);

	return parent->GetTags();
}

Vector3 Component::GetPosition() const{
	GADGET_BASIC_ASSERT(parent != nullptr);

	return parent->GetPosition();
}

Quaternion Component::GetRotation() const{
	GADGET_BASIC_ASSERT(parent != nullptr);

	return parent->GetRotation();
}

Vector3 Component::GetScale() const{
	GADGET_BASIC_ASSERT(parent != nullptr);

	return parent->GetScale();
}

const Transform& Component::GetTransform() const{
	GADGET_BASIC_ASSERT(parent != nullptr);

	return parent->GetTransform();
}

Matrix4 Component::GetTransformMatrix() const{
	GADGET_BASIC_ASSERT(parent != nullptr);

	return parent->GetTransformMatrix();
}

ComponentProperties Component::Serialize() const{
	GADGET_BASIC_ASSERT(typeName != StringID::None);
	GADGET_BASIC_ASSERT(guid != GUID::Invalid);
	GADGET_BASIC_ASSERT(parent != nullptr && parent->GetGUID() != GUID::Invalid);

	ComponentProperties props = ComponentProperties(typeName, guid, parent->GetGUID());
	props.isActivated = isActivated;

	return props;
}

void Component::Deserialize(const ComponentProperties& props_){
	guid = props_.guid;
	isActivated = props_.isActivated;

	GADGET_BASIC_ASSERT(props_.typeName != StringID::None);
	GADGET_BASIC_ASSERT(props_.parentGuid != GUID::Invalid);

	if(props_.guid == GUID::Invalid){
		guid = GUID::Generate();
	}
	GADGET_BASIC_ASSERT(guid != GUID::Invalid);

	parent = GameObjectCollection::Get(props_.parentGuid);
	GADGET_BASIC_ASSERT(parent != nullptr);
}