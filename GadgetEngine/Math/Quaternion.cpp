#include "Quaternion.h"

#include "Math/Euler.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

using namespace Gadget;

constexpr Quaternion::Quaternion(const float w_, const Vector3& v_) : w(w_), x(v_.x), y(v_.y), z(v_.z){}

constexpr Quaternion::Quaternion(const Vector4& v_) : w(v_.w), x(v_.x), y(v_.y), z(v_.z){}

float Quaternion::Magnitude() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Math::Sqrt(SquaredMagnitude());
}

Quaternion Quaternion::Normalized() const{
	GADGET_BASIC_ASSERT(IsValid());
	return *this / Magnitude();
}

void Quaternion::Normalize(){
	GADGET_BASIC_ASSERT(IsValid());
	*this = Normalized();
}

Quaternion Quaternion::Rotate(Angle angle_, const Vector3& axis_){
	GADGET_BASIC_ASSERT(angle_.IsValid());
	GADGET_BASIC_ASSERT(axis_.IsValid());

	return Quaternion(
		Math::CosR(angle_.ToRadians() / 2.0f),
		axis_.x * Math::SinR(angle_.ToRadians() / 2.0f),
		axis_.y * Math::SinR(angle_.ToRadians() / 2.0f),
		axis_.z * Math::SinR(angle_.ToRadians() / 2.0f)
	);
}

Angle Quaternion::GetRotationAngle(const Quaternion& q_){
	GADGET_BASIC_ASSERT(q_.IsValid());
	return Math::Acos(q_.w) * 2.0f;
}

Vector3 Quaternion::GetRotationAxis(const Quaternion& q_){
	GADGET_BASIC_ASSERT(q_.IsValid());

	Vector3 v = Vector3(q_.x, q_.y, q_.z);
	return v / Math::SinR((GetRotationAngle(q_) / 2.0f).ToRadians());
}

Quaternion Quaternion::LookAt(const Vector3& source_, const Vector3& destination_){
	GADGET_BASIC_ASSERT(source_.IsValid());
	GADGET_BASIC_ASSERT(destination_.IsValid());

	Vector3 forwardVector = (destination_ - source_).Normalized();

	float dot = Vector3::Dot(Vector3::Forward(), forwardVector);

	if(Math::Abs(dot - (-1.0f)) < Math::NearZero){
		return Quaternion(Math::Pi, Vector3::Up());
	}
	
	if(Math::Abs(dot - (1.0f)) < Math::NearZero){
		return Quaternion::Identity();
	}

	Angle rotAngle = Math::Acos(dot);
	Vector3 rotAxis = Vector3::Cross(Vector3::Forward(), forwardVector).Normalized();
	return Quaternion::Rotate(rotAngle.Get(), rotAxis);
}

Quaternion Quaternion::Lerp(const Quaternion& q1_, const Quaternion& q2_, float t_){
	GADGET_BASIC_ASSERT(q1_.IsValid());
	GADGET_BASIC_ASSERT(q2_.IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(t_));

	return (q1_ * (1 - t_) + q2_ * t_).Normalized();
}

Quaternion Quaternion::Slerp(const Quaternion& q1_, const Quaternion& q2_, float t_){
	GADGET_BASIC_ASSERT(q1_.IsValid());
	GADGET_BASIC_ASSERT(q2_.IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(t_));

	if(t_ <= 0.0f){
		return q1_;
	}else if(t_ >= 1.0f){
		return q2_;
	}

	const Quaternion q1 = q1_.Normalized();
	const Quaternion q2 = q2_.Normalized();

	const float dot = Quaternion::Dot(q1, q2);
	const Angle theta = Math::Acos(dot);
	const float sinTheta = Math::Sin(theta);

	//Do a regular interpolation if theta is too small
	//TODO - This cutoff is somewhat arbitrary and I don't know how it was chosen, or how necessary it really is
	if(theta < 0.001f || sinTheta < 0.001f){
		return Lerp(q1, q2, t_);
	}

	//Final Result
	const Quaternion qA = q1 * Math::Sin((1.0f - t_) * theta) / sinTheta;
	const Quaternion qB = q2 * Math::Sin(t_ * theta) / sinTheta;
	return qA + qB;
}

Matrix3 Quaternion::ToMatrix3() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix3(ToMatrix4());
}

Matrix4 Quaternion::ToMatrix4() const{
	GADGET_BASIC_ASSERT(IsValid());

	//TODO - Validate this
	//squared w,x,y,z
	//const float w2 = w * w;
	const float x2 = x * x;
	const float y2 = y * y;
	const float z2 = z * z;

	Matrix4 mat = Matrix4::Identity();
	mat[0] = 1 - 2 * y2 - 2 * z2;
	mat[4] = 2 * x * y - 2 * z * w ;
	mat[8] = 2 * x * z + 2 * y * w;

	mat[1] = 2 * x * y + 2 * z * w;
	mat[5] = 1 - 2 * x2 - 2 * z2;
	mat[9] = 2 * y * z  - 2 * x * w;

	mat[2] = 2 * x * z - 2 * y * w;
	mat[6] = 2 * y * z + 2 * x * w;
	mat[10] = 1 - 2 * x2 - 2 * y2;
	return mat;
}

Euler Quaternion::ToEuler() const{
	GADGET_BASIC_ASSERT(IsValid());

	float heading = 0.0f;
	float attitude = 0.0f;
	float bank = 0.0f;

	float sqw = w * w;
	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = (x * y) + (z * w);

	if(test > 0.499f * unit){ // singularity at north pole
		heading = 2.0f * atan2(x, w);
		attitude = Math::Pi / 2.0f;
		bank = 0.0f;
		return Euler(Math::RadiansToDegrees(bank), Math::RadiansToDegrees(heading), Math::RadiansToDegrees(attitude));
	}else if (test < -0.499f * unit) { // singularity at south pole
		heading = -2.0f * atan2(x, w);
		attitude = -Math::Pi / 2.0f;
		bank = 0.0f;
		return Euler(Math::RadiansToDegrees(bank), Math::RadiansToDegrees(heading), Math::RadiansToDegrees(attitude));
	}

	heading = atan2(2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw);
	attitude = asin(2.0f * test / unit);
	bank = atan2(2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw);
	return Euler(Math::RadiansToDegrees(bank), Math::RadiansToDegrees(heading), Math::RadiansToDegrees(attitude));
}

std::string Quaternion::ToString() const{
	return std::string(std::to_string(w) + ", (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
}