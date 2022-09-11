#ifndef GADGET_CAMERA_H
#define GADGET_CAMERA_H

#include "Graphics/ViewportRect.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

namespace Gadget{
	class Camera{
	public:
		enum class Projection{
			Orthographic,
			Perspective
		};

		Camera(Projection projection_ = Projection::Perspective, const Rect& viewRect_ = ViewportRect::Fullscreen);

		void CalculateViewMatrix(const Vector3& cameraPos_, const Quaternion& cameraRot_);
		void CalculateProjectionMatrix();
		
		Matrix4 GetViewMatrix() const;
		Matrix4 GetProjectionMatrix() const;
		Rect GetViewportRect() const;
		Projection GetCurrentProjection() const;
		float GetFOV() const;
		float GetNearPlane() const;
		float GetFarPlane() const;

		void SetViewportRect(Rect rect_);
		void SetProjection(Projection projection_);
		void SetFOV(float fov_);
		void SetAspect(float aspect_);
		void SetNearPlane(float near_);
		void SetFarPlane(float far_);

	private:
		Matrix4 view;
		Matrix4 projection;
		float fov;
		float aspect;
		float nearPlane;
		float farPlane;

		Rect viewRect;
		Projection currentProjection;
	};
}

#endif //!GADGET_CAMERA_H