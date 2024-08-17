#ifndef GADGET_GRAPHICS_COMPONENTS_SPOT_LIGHT_COMPONENT_H
#define GADGET_GRAPHICS_COMPONENTS_SPOT_LIGHT_COMPONENT_H

#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Graphics/LightSource.h"

namespace Gadget{
	class SpotLightComponent : public Component{
	public:
		static const StringID type;

		SpotLightComponent(GameObject* parent_, const Vector3& direction_) : Component(type, parent_), lightSource(direction_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(parent_->GetGUID() != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		SpotLightComponent(GUID parentGUID_, const Vector3& direction_) : Component(type, parentGUID_), lightSource(direction_){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		SpotLightComponent(const ComponentProperties& props_) : Component(props_), lightSource(Vector3::Forward){ GADGET_BASIC_ASSERT(props_.typeName == SpotLightComponent::type); }

		virtual ~SpotLightComponent() override{
			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

			componentCollection.Remove(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
		}

		static SpotLightComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<SpotLightComponent*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		const SpotLight& GetLightSource() const{ return lightSource; }

		virtual ComponentProperties Serialize() const override{ return Component::Serialize(); } //TODO

	protected:
		virtual void Deserialize([[maybe_unused]] const ComponentProperties& props_) override{} //TODO

	private:
		SpotLight lightSource;

		static ComponentCollection<SpotLightComponent> componentCollection;
	};
}

#endif //!GADGET_GRAPHICS_COMPONENTS_SPOT_LIGHT_COMPONENT_H