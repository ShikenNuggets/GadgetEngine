#ifndef GADGET_GRAPHICS_COMPONENTS_DIR_LIGHT_H
#define GADGET_GRAPHICS_COMPONENTS_DIR_LIGHT_H

#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Graphics/LightSource.h"

namespace Gadget{
	class DirectionalLightComponent : public Component{
	public:
		static const StringID type;

		DirectionalLightComponent(GameObject* parent_, const Vector3& direction_) : Component(type, parent_), lightSource(direction_){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(parent_->GetGUID() != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		DirectionalLightComponent(GUID parentGUID_, const Vector3& direction_) : Component(type, parentGUID_), lightSource(direction_){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		DirectionalLightComponent(const ComponentProperties& props_) : Component(props_), lightSource(Vector3::Forward()){ GADGET_BASIC_ASSERT(props_.typeName == DirectionalLightComponent::type); }

		virtual ~DirectionalLightComponent() override{
			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

			componentCollection.Remove(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
		}

		static DirectionalLightComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<DirectionalLightComponent*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		const DirectionalLight& GetLightSource() const{ return lightSource; }

		virtual ComponentProperties Serialize() const override{ return Component::Serialize(); } //TODO

	protected:
		virtual void Deserialize([[maybe_unused]] const ComponentProperties& props_) override{} //TODO

	private:
		DirectionalLight lightSource;

		static ComponentCollection<DirectionalLightComponent> componentCollection;
	};
}

#endif //!GADGET_GRAPHICS_COMPONENTS_DIR_LIGHT_H