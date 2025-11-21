#include "Camera.h"

#include "App.h"
#include "Events/EventHandler.h"

using namespace Gadget;

static constexpr float gDefaultFOV = 45.0f;
static constexpr float gDefaultNearPlane = 0.1f;
static constexpr float gDefaultFarPlane = 1000.0f;
static constexpr float gDefaultOrthographicHeight = 12.0f;

Camera::Camera(const Vector3& position_, const Quaternion& rotation_, Projection projection_, const Rect& viewRect_) : view(), projection(), fov(gDefaultFOV), aspect(1.0f), nearPlane(gDefaultNearPlane), farPlane(gDefaultFarPlane), orthoHeight(gDefaultOrthographicHeight), viewRect(viewRect_), currentProjection(projection_){
	GADGET_BASIC_ASSERT(position_.IsValid());
	GADGET_BASIC_ASSERT(rotation_.IsValid());
	GADGET_BASIC_ASSERT(projection_ < Projection::Projection_MAX);
	GADGET_BASIC_ASSERT(viewRect_.IsValid());
	
	aspect = App::GetAspectRatio();

	CalculateViewMatrix(position_, rotation_);
	CalculateProjectionMatrix();

	EventHandler::GetInstance()->RegisterCallback(EventType::WindowResize, this, [&]([[maybe_unused]] const Event& e){ OnResize(); });
}

Camera::~Camera(){
	EventHandler::GetInstance()->UnregisterCallbacks(this);
}

void Camera::CalculateViewMatrix(const Vector3& position_, const Quaternion& rotation_){
	GADGET_BASIC_ASSERT(position_.IsValid());
	GADGET_BASIC_ASSERT(rotation_.IsValid());

	view = Matrix4::Identity();
	view *= rotation_.ToMatrix4().Inverse();
	view *= Matrix4::Translate(position_).Inverse();
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
			projection = Matrix4::Orthographic(-(orthoHeight * aspect) / 2.0f, (orthoHeight * aspect) / 2.0f, -orthoHeight / 2.0f, orthoHeight / 2.0f, -100.0f, 100.0f);
			break;
		default:
			Debug::Log("Invalid projection mode!", Debug::Error, __FILE__, __LINE__);
			break;
	}

	GADGET_BASIC_ASSERT(projection.IsValid());
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

float Camera::GetOrthoHeight() const{
	return orthoHeight;
}

void Camera::SetViewportRect(const Rect& rect_){
	GADGET_BASIC_ASSERT(rect_.IsValid());
	viewRect = rect_;
}

void Camera::SetProjection(Projection projection_){
	GADGET_BASIC_ASSERT(projection_ < Projection::Projection_MAX);
	currentProjection = projection_;
	CalculateProjectionMatrix();
}

void Camera::SetFOV(float fov_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(fov_) && fov_ > 0.0f);
	fov = fov_;
	CalculateProjectionMatrix();
}

void Camera::SetAspect(float aspect_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(aspect_));
	aspect = aspect_;
	CalculateProjectionMatrix();
}

void Camera::SetNearPlane(float near_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(near_));
	nearPlane = near_;
	CalculateProjectionMatrix();
}

void Camera::SetFarPlane(float far_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(far_));
	farPlane = far_;
	CalculateProjectionMatrix();
}

void Camera::SetOrthoHeight(float height_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(height_));
	orthoHeight = height_;
}

void Camera::OnResize(){
	aspect = App::GetAspectRatio();
	CalculateProjectionMatrix();
}