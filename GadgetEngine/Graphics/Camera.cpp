#include "Camera.h"

#include "App.h"

using namespace Gadget;

Camera::Camera(Projection projection_, const Rect& viewRect_) : view(), projection(), fov(45.0f), aspect(1.0f), nearPlane(0.1f), farPlane(1000.0f), currentProjection(projection_), viewRect(viewRect_), position(), rotation(){
	aspect = App::GetInstance()->GetAspectRatio();

	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

void Camera::CalculateViewMatrix(){
	view = Matrix4::Identity();
	view *= rotation.ToMatrix4().Inverse();
	view *= Matrix4::Translate(position).Inverse();
}

void Camera::CalculateProjectionMatrix(){
	GADGET_ASSERT(currentProjection == Projection::Orthographic || currentProjection == Projection::Perspective, "Invalid projection mode!");
	GADGET_ASSERT(aspect > 0.0f, "Invalid aspect ratio of " + std::to_string(aspect) + "!");
	GADGET_ASSERT(nearPlane <= farPlane, "Near Plane [ " + std::to_string(nearPlane) + " ] should always be less than Far Plane [ " + std::to_string(farPlane) + " ]!");

	switch(currentProjection){
		case Projection::Perspective:
			projection = Matrix4::Perspective(fov, aspect, nearPlane, farPlane);
			break;
		case Projection::Orthographic:
			projection = Matrix4::Orthographic(-10.0f, 10.0f * aspect, -10.0f, 10.0f / aspect, -100.0f, 100.0f);
			break;
		default:
			Debug::Log("Invalid projection mode!", Debug::Error, __FILE__, __LINE__);
			break;
	}
}

Matrix4 Camera::GetViewMatrix() const{
	return view;
}

Matrix4 Camera::GetProjectionMatrix() const{
	return projection;
}

Rect Camera::GetViewportRect() const{
	return viewRect;
}

Camera::Projection Camera::GetCurrentProjection() const{
	return currentProjection;
}

float Camera::GetFOV() const{
	return fov;
}

float Camera::GetNearPlane() const{
	return nearPlane;
}

float Camera::GetFarPlane() const{
	return farPlane;
}

void Camera::SetViewportRect(const Rect& rect_){
	viewRect = rect_;
}

void Camera::SetProjection(Projection projection_){
	currentProjection = projection_;
	CalculateProjectionMatrix();
}

void Camera::SetFOV(float fov_){
	fov = fov_;
	CalculateProjectionMatrix();
}

void Camera::SetAspect(float aspect_){
	aspect = aspect_;
	CalculateProjectionMatrix();
}

void Camera::SetNearPlane(float near_){
	nearPlane = near_;
	CalculateProjectionMatrix();
}

void Camera::SetFarPlane(float far_){
	farPlane = far_;
	CalculateProjectionMatrix();
}