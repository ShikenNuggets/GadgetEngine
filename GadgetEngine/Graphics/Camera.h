#ifndef GADGET_CAMERA_H
#define GADGET_CAMERA_H

#include "Graphics/ViewportRect.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

namespace Gadget{
	class Camera{
	public:
		enum class Projection : uint8_t{
			Orthographic = 0,
			Perspective,

			Projection_MAX //Do not put any values below this!
		};

		Camera(const Vector3& position_ = Vector3::Zero(), const Quaternion& rotation_ = Quaternion::Identity(), Projection projection_ = Projection::Perspective, const Rect& viewRect_ = ViewportRect::Fullscreen);
		~Camera();

		void CalculateViewMatrix(const Vector3& position_, const Quaternion& rotation_);
		void CalculateProjectionMatrix();
		
		Matrix4 GetViewMatrix() const;
		Matrix4 GetProjectionMatrix() const;
		Rect GetViewportRect() const;
		Projection GetCurrentProjection() const;
		float GetFOV() const;
		float GetNearPlane() const;
		float GetFarPlane() const;
		float GetOrthoHeight() const;

		void SetViewportRect(const Rect& rect_);
		void SetProjection(Projection projection_);
		void SetFOV(float fov_);
		void SetAspect(float aspect_);
		void SetNearPlane(float near_);
		void SetFarPlane(float far_);
		void SetOrthoHeight(float height_);

		void OnResize();

	private:
		Matrix4 view;
		Matrix4 projection;
		float fov;
		float aspect;
		float nearPlane;
		float farPlane;
		float orthoHeight;

		Rect viewRect;
		Projection currentProjection;
	};
}

#endif //!GADGET_CAMERA_H