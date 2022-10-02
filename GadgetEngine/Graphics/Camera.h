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

		void CalculateViewMatrix();
		void CalculateProjectionMatrix();
		
		Vector3 GetPosition() const{ return position; }
		Quaternion GetRotation() const{ return rotation; }
		Matrix4 GetViewMatrix() const;
		Matrix4 GetProjectionMatrix() const;
		Rect GetViewportRect() const;
		Projection GetCurrentProjection() const;
		float GetFOV() const;
		float GetNearPlane() const;
		float GetFarPlane() const;

		void SetPosition(const Vector3& position_){ position = position_; CalculateViewMatrix(); }
		void SetRotation(const Quaternion& rotation_){ rotation = rotation_; CalculateViewMatrix(); }
		void SetViewportRect(const Rect& rect_);
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

		//TODO - We'll probably store these elsewhere later but for now it's useful to have these
		Vector3 position;
		Quaternion rotation;
	};
}

#endif //!GADGET_CAMERA_H