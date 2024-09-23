#ifndef GADGET_LIGHT_COMPONENT_H
#define GADGET_LIGHT_COMPONENT_H

#include "Game/Component.h"
#include "Game/GameObject.h"
#include "Graphics/LightSource.h"

namespace Gadget{
	class PointLightComponent : public Component{
	public:
		static const StringID type;

		PointLightComponent(GameObject* parent_) : Component(type, parent_), lightSource(){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			if(parent_ == nullptr){
				Debug::ThrowFatalError(SID("RENDER"), "PointLightComponent created with null parent!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
			}

			GADGET_BASIC_ASSERT(parent_->GetGUID() != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		PointLightComponent(GUID parentGUID_) : Component(type, parentGUID_), lightSource(){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		PointLightComponent(const ComponentProperties& props_) : Component(props_){ GADGET_BASIC_ASSERT(props_.typeName == PointLightComponent::type); }

		virtual ~PointLightComponent() override{
			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

			componentCollection.Remove(this);
			
			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
		}

		static PointLightComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static const ComponentCollection<PointLightComponent>& GetCollection(){ return componentCollection; }

		const PointLight& GetLightSource() const{ return lightSource; }
		PointLight& GetLightSource(){ return lightSource; }

		virtual ComponentProperties Serialize() const override{ return Component::Serialize(); }

	protected:
		virtual void Deserialize([[maybe_unused]] const ComponentProperties& props_) override{}

	private:
		PointLight lightSource;

		static ComponentCollection<PointLightComponent> componentCollection;
	};
}

#endif //!GADGET_LIGHT_COMPONENT_H