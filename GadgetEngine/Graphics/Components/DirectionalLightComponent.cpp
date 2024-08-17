#include "DirectionalLightComponent.h"

using namespace Gadget;

const StringID DirectionalLightComponent::type = SID("DirectionalLightComponent");
ComponentCollection<DirectionalLightComponent> DirectionalLightComponent::componentCollection = ComponentCollection<DirectionalLightComponent>();