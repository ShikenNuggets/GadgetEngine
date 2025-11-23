#ifndef EXAMPLE_OBJECT_ROTATOR_H
#define EXAMPLE_OBJECT_ROTATOR_H

#include <Game/GameLogicComponent.h>
#include <Game/GameObject.h>

namespace Example{
	class ObjectRotator : public Gadget::GameLogicComponent{
	public:
		ObjectRotator(Gadget::GameObject* parent_) : GameLogicComponent(SID("ObjectRotator"), parent_){}

		ObjectRotator(const Gadget::ComponentProperties& props_) : GameLogicComponent(props_){
			Deserialize(props_);
		}

		virtual ~ObjectRotator() override{}

		virtual void OnUpdate(float deltaTime_) override{
			parent->Rotate(10.0f * deltaTime_, 10.0f * deltaTime_, 0.0f);
		}

		virtual Gadget::ComponentProperties Serialize() const override{ return GameLogicComponent::Serialize(); }

	protected:
		virtual void Deserialize([[maybe_unused]] const Gadget::ComponentProperties& props_) override{}
	};
}

#endif //!EXAMPLE_OBJECT_ROTATOR_H