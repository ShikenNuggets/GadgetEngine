#include "ComponentFactory.h"

#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Components/RenderComponent.h"
#include "Physics/BoxCollider2D.h"
#include "Physics/CubeCollider.h"
#include "Physics/Rigidbody.h"

using namespace Gadget;

std::map<StringID, ComponentDeserializeFunc> ComponentFactory::deserializers;

//This is pretty stupid but it's fine for now
//TODO - Need a proper solution here so that this can go away, and also so game code can declare serializable components in a sane way
void ComponentFactory::Init(){
	GADGET_DECLARE_SERIALIZABLE_COMPONENT(CameraComponent);
	GADGET_DECLARE_SERIALIZABLE_COMPONENT(PointLightComponent);
	GADGET_DECLARE_SERIALIZABLE_COMPONENT(RenderComponent);
	GADGET_DECLARE_SERIALIZABLE_COMPONENT(BoxCollider2D);
	GADGET_DECLARE_SERIALIZABLE_COMPONENT(CubeCollider);
	GADGET_DECLARE_SERIALIZABLE_COMPONENT(Rigidbody);
}