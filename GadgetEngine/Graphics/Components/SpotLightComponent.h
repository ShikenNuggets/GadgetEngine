#ifndef GADGET_GRAPHICS_COMPONENTS_SPOT_LIGHT_COMPONENT_H
#define GADGET_GRAPHICS_COMPONENTS_SPOT_LIGHT_COMPONENT_H

#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Graphics/LightSource.h"

namespace Gadget{
	class SpotLightComponent : public Component{
	public:
		static const StringID type;

		SpotLightComponent(GameObject* parent_) : Component(type, parent_), lightSource(Vector3::Forward()){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			if(parent_ == nullptr){
				Debug::ThrowFatalError(SID("RENDER"), "SpotLightComponent created with null parent!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(parent_->GetGUID() != GUID::Invalid);

			lightSource.SetDirection(parent->GetTransform().Forward());

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		SpotLightComponent(GUID parentGUID_) : Component(type, parentGUID_), lightSource(Vector3::Forward()){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);

			lightSource.SetDirection(parent->GetTransform().Forward());

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		SpotLightComponent(const ComponentProperties& props_) : Component(props_), lightSource(Vector3::Forward()){ GADGET_BASIC_ASSERT(props_.typeName == SpotLightComponent::type); }

		virtual ~SpotLightComponent() override{
			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

			componentCollection.Remove(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
		}

		static SpotLightComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static const ComponentCollection<SpotLightComponent>& GetCollection(){ return componentCollection; }

		const SpotLight& GetLightSource() const{ return lightSource; }
		SpotLight& GetLightSource(){ return lightSource; }

		virtual void OnTransformModified() override{
			GADGET_BASIC_ASSERT(parent != nullptr);
			lightSource.SetDirection(parent->GetTransform().Forward());
		}

		virtual ComponentProperties Serialize() const override{ return Component::Serialize(); } //TODO

	protected:
		virtual void Deserialize([[maybe_unused]] const ComponentProperties& props_) override{} //TODO

	private:
		SpotLight lightSource;

		static ComponentCollection<SpotLightComponent> componentCollection;
	};
}

#endif //!GADGET_GRAPHICS_COMPONENTS_SPOT_LIGHT_COMPONENT_H