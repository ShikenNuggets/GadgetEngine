#ifndef GADGET_EULER_H
#define GADGET_EULER_H

#include <string>

#include "Angle.h"
#include "Math.h"

namespace Gadget{
	//Forward Declarations
	struct Vector3;
	class Matrix3;
	class Matrix4;
	class Matrix4x3;
	class Quaternion;

	class Euler{
	public:
		Angle x;
		Angle y;
		Angle z;

		explicit constexpr Euler(Angle x_ = 0.0f, Angle y_ = 0.0f, Angle z_ = 0.0f) : x(x_), y(y_), z(z_){}
		explicit Euler(const Vector3& v_);

		inline constexpr Euler operator -() const{ return Euler(-x, -y, -z); }
		inline constexpr Euler operator +(const Euler& e_) const{ return Euler(x + e_.x, y + e_.y, z + e_.z); }
		inline constexpr Euler operator -(const Euler& e_) const{ return Euler(x - e_.x, y - e_.y, z - e_.z); }
		inline constexpr Euler operator *(float f_) const{ return Euler(x * f_, y * f_, z * f_); }
		inline constexpr Euler operator /(float f_) const{ return Euler(Math::SafeDivide(x, f_), Math::SafeDivide(y, f_)); }
		inline constexpr void operator +=(const Euler& e_){ *this = *this + e_; }
		inline constexpr void operator -=(const Euler& e_){ *this = *this - e_; }
		inline constexpr void operator *=(float f_){ *this = *this * f_; }
		inline constexpr void operator /=(float f_){ *this = *this / f_; }

		Matrix3 ToMatrix3() const;
		Matrix4 ToMatrix4() const;
		Quaternion ToQuaternion() const;

		inline bool IsValid() const{ return Math::IsValidNumber(x) && Math::IsValidNumber(y) && Math::IsValidNumber(z); }

		std::string ToString() const;
	};
}

#endif //!GADGET_EULER_H