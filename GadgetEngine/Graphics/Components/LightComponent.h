#ifndef GADGET_LIGHT_COMPONENT_H
#define GADGET_LIGHT_COMPONENT_H

#include "Game/Component.h"
#include "Graphics/LightSource.h"

namespace Gadget{
	class PointLightComponent : public Component{
	public:
		PointLightComponent(GameObject* parent_) : Component(parent_), lightSource(){}
		virtual ~PointLightComponent() override{}

		static PointLightComponent* Get(GUID objectGuid_){ return componentCollection.Get(objectGuid_); }
		static std::vector<PointLightComponent*> GetComponents(GUID objectGuid_){ return componentCollection.GetComponents(objectGuid_); }

		const PointLight& GetLightSource() const{ return lightSource; }

	private:
		PointLight lightSource;

		static ComponentCollection<PointLightComponent> componentCollection;
	};
}

#endif //!GADGET_LIGHT_COMPONENT_H