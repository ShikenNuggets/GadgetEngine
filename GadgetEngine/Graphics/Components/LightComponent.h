#ifndef GADGET_LIGHT_COMPONENT_H
#define GADGET_LIGHT_COMPONENT_H

#include "Game/Component.h"
#include "Graphics/LightSource.h"

namespace Gadget{
	class PointLightComponent : public Component{
	public:
		PointLightComponent(GameObject* parent_) : Component(parent_), lightSource(){}
		virtual ~PointLightComponent() override{}

		const PointLight& GetLightSource() const{ return lightSource; }

	private:
		PointLight lightSource;
	};
}

#endif //!GADGET_LIGHT_COMPONENT_H