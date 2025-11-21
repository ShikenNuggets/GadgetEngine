#ifndef GADGET_CAMERA_COMPONENT_H
#define GADGET_CAMERA_COMPONENT_H

#include "Game/Component.h"
#include "Graphics/Camera.h"

namespace Gadget{
	class CameraComponent : public Component{
	public:
		static const StringID type;

		CameraComponent(GameObject* parent_, Camera::Projection projection_ = Camera::Projection::Perspective, const Rect& viewRect_ = ViewportRect::Fullscreen);
		CameraComponent(GUID parentGUID_, Camera::Projection projection_ = Camera::Projection::Perspective, const Rect& viewRect_ = ViewportRect::Fullscreen);
		CameraComponent(const ComponentProperties& props_);
		virtual ~CameraComponent() override;

		static CameraComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static const ComponentCollection<CameraComponent>& GetCollection(){ return componentCollection; }

		Matrix4 GetUpdatedViewMatrix();
		Matrix4 GetUpdatedProjectionMatrix();

		const Camera& GetCamera(){ return camera; }

		virtual ComponentProperties Serialize() const override;

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override;

	private:
		Camera camera;
		Vector3 lastPosition;
		Quaternion lastRotation;
		float lastAspect;

		static ComponentCollection<CameraComponent> componentCollection;
	};
}

#endif //!GADGET_CAMERA_COMPONENT_H