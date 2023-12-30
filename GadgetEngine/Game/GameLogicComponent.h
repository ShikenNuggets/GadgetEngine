#ifndef GADGET_GAME_LOGIC_COMPONENT_H
#define GADGET_GAME_LOGIC_COMPONENT_H

#include "Component.h"
#include "Physics/CollisionSystem.h"

namespace Gadget{
	class GameLogicComponent : public Component{
	public:
		GameLogicComponent(StringID typeName_, GameObject* parent_) : Component(typeName_, parent_), hasStarted(false), collisionsToHandle(){
			GADGET_BASIC_ASSERT(parent_ != nullptr);
			GADGET_BASIC_ASSERT(parent_->GetGUID() != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		GameLogicComponent(StringID typeName_, GUID parentGUID_) : Component(typeName_, parentGUID_), hasStarted(false), collisionsToHandle(){
			GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);

			componentCollection.Add(this);

			GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
		}

		GameLogicComponent(const ComponentProperties& props_) : Component(props_), hasStarted(false), collisionsToHandle(){}

		virtual ~GameLogicComponent() override{
			OnDestroy();
			componentCollection.Remove(this);
		}

		static GameLogicComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<GameLogicComponent*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		virtual void OnStart(){ hasStarted = true; }
		virtual void OnUpdate([[maybe_unused]] float deltaTime_){}
		virtual void OnDestroy(){}

		virtual void OnCollision([[maybe_unused]] const Collision& col_){}

		bool HasStarted() const{ return hasStarted; }

		bool HasCollisionsToHandle() const{ return !collisionsToHandle.empty(); }

		Collision PopCollisionToHandle(){
			GADGET_BASIC_ASSERT(HasCollisionsToHandle());
			Collision val = collisionsToHandle.front();
			collisionsToHandle.pop();
			return val;
		}

		void AddCollisionToHandle(const Collision& col_){ collisionsToHandle.push(col_); }

	private:
		bool hasStarted;
		std::queue<Collision> collisionsToHandle;

		static ComponentCollection<GameLogicComponent> componentCollection;
	};
}

#endif //!GADGET_GAME_LOGIC_COMPONENT_H
