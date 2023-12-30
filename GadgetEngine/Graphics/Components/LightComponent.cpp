#include "LightComponent.h"

using namespace Gadget;

const StringID PointLightComponent::type = SID("PointLightComponent");
ComponentCollection<PointLightComponent> PointLightComponent::componentCollection = ComponentCollection<PointLightComponent>();