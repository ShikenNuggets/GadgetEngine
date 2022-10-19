#ifndef GADGET_CAMERA_COMPONENT_H
#define GADGET_CAMERA_COMPONENT_H

#include "Game/Component.h"
#include "Graphics/Camera.h"

namespace Gadget{
	class CameraComponent : public Component{
	public:
		CameraComponent(GameObject* parent_, Camera::Projection projection_ = Camera::Projection::Perspective, const Rect& viewRect_ = ViewportRect::Fullscreen);
		virtual ~CameraComponent() override;

		Matrix4 GetUpdatedViewMatrix();

		const Camera& GetCamera(){ return camera; }

	private:
		Camera camera;
		Vector3 lastPosition;
		Quaternion lastRotation;
	};
}

#endif //!GADGET_CAMERA_COMPONENT_H