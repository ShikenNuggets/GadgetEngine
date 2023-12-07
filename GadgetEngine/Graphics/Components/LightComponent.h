#ifndef GADGET_LIGHT_COMPONENT_H
#define GADGET_LIGHT_COMPONENT_H

#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Graphics/LightSource.h"

namespace Gadget{
	class PointLightComponent : public Component{
	public:
		PointLightComponent(GameObject* parent_) : Component(parent_), lightSource(){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(parent_->GetGUID() != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		virtual ~PointLightComponent() override{
			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

			componentCollection.Remove(this);
			
			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
		}

		static PointLightComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<PointLightComponent*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		const PointLight& GetLightSource() const{ return lightSource; }

	private:
		PointLight lightSource;

		static ComponentCollection<PointLightComponent> componentCollection;
	};
}

#endif //!GADGET_LIGHT_COMPONENT_H