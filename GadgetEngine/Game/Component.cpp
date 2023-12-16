#include "Component.h"

#include "GameObject.h"

using namespace Gadget;

Component::Component(StringID typeName_, GUID parentGUID_) : typeName(typeName_), guid(GUID::Generate()), isActivated(false){
	GADGET_BASIC_ASSERT(typeName_ != StringID::None);
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(guid != GUID::Invalid);

	parent = GameObjectCollection::Get(parentGUID_);
	GADGET_BASIC_ASSERT(parent != nullptr);
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