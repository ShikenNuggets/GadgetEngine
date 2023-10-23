#include "Component.h"

#include "GameObject.h"

using namespace Gadget;

StringID Component::GetParentName() const{
	return parent->GetName();
}

bool Component::HasTag(StringID tag_) const{
	return parent->HasTag(tag_);
}

std::vector<StringID> Component::GetTags() const{
	return parent->GetTags();
}

Vector3 Component::GetPosition() const{
	return parent->GetPosition();
}

Quaternion Component::GetRotation() const{
	return parent->GetRotation();
}

Vector3 Component::GetScale() const{
	return parent->GetScale();
}

const Transform& Component::GetTransform() const{
	return parent->GetTransform();
}

Matrix4 Component::GetTransformMatrix() const{
	return parent->GetTransformMatrix();
}