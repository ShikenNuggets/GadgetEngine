#include "Euler.h"

#include "Math.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Debug.h"

using namespace Gadget;

constexpr Euler::Euler(Angle x_, Angle y_, Angle z_) : x(x_), y(y_), z(z_){}

constexpr Euler::Euler(const Vector3& v_) : x(v_.x), y(v_.y), z(v_.z){}

constexpr Euler Euler::operator -() const{
	return Euler(-x, -y, -z);
}

constexpr Euler Euler::operator +(const Euler& e_) const{
	return Euler(x + e_.x, y + e_.y, z + e_.z);
}

constexpr Euler Euler::operator -(const Euler& e_) const{
	return Euler(x - e_.x, y - e_.y, z - e_.z);
}

constexpr Euler Euler::operator *(float f_) const{
	return Euler(x * f_, y * f_, z * f_);
}

constexpr Euler Euler::operator /(float f_) const{
	return Euler(Math::SafeDivide(x, f_), Math::SafeDivide(y, f_));
}

constexpr void Euler::operator +=(const Euler& e_){ *this = *this + e_; }

constexpr void Euler::operator -=(const Euler& e_){ *this = *this - e_; }

constexpr void Euler::operator *=(float f_){ *this = *this * f_; }

constexpr void Euler::operator /=(float f_){ *this = *this / f_; }

constexpr Matrix3 Euler::ToMatrix3() const{
	return Matrix3(ToMatrix4());
}

constexpr Matrix4 Euler::ToMatrix4() const{
	return	Matrix4::Rotate(y, Vector3(0.0f, 1.0f, 0.0f)) *
			Matrix4::Rotate(z, Vector3(0.0f, 0.0f, 1.0f)) * 
			Matrix4::Rotate(x, Vector3(1.0f, 0.0f, 0.0f));
}

Quaternion Euler::ToQuaternion() const{
	float bank = x.ToRadians();
	float heading = y.ToRadians();
	float attitude = z.ToRadians();

	float c1 = Math::CosR(heading / 2.0f);
	float c2 = Math::CosR(attitude / 2.0f);
	float c3 = Math::CosR(bank / 2.0f);

	float s1 = Math::SinR(heading / 2.0f);
	float s2 = Math::SinR(attitude / 2.0f);
	float s3 = Math::SinR(bank / 2.0f);

	float newW = c1 * c2 * c3 - s1 * s2 * s3;
	float newX = s1 * s2 * c3 + c1 * c2 * s3;
	float newY = s1 * c2 * c3 + c1 * s2 * s3;
	float newZ = c1 * s2 * c3 - s1 * c2 * s3;

	return Quaternion(newW, newX, newY, newZ).Normalized();
}

std::string Euler::ToString() const{
	return std::string(std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z));
}