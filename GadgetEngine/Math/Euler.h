#ifndef GADGET_EULER_H
#define GADGET_EULER_H

#include <string>

#include "Angle.h"

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

		explicit constexpr Euler(Angle x_ = 0.0f, Angle y_ = 0.0f, Angle z_ = 0.0f);
		explicit constexpr Euler(const Vector3& v_);

		constexpr Euler operator -() const;
		constexpr Euler operator +(const Euler& e_) const;
		constexpr Euler operator -(const Euler& e_) const;
		constexpr Euler operator *(float f) const;
		constexpr Euler operator /(float f) const;
		constexpr void operator +=(const Euler& e_);
		constexpr void operator -=(const Euler& e_);
		constexpr void operator *=(float f);
		constexpr void operator /=(float f);

		constexpr Matrix3 ToMatrix3() const;
		constexpr Matrix4 ToMatrix4() const;
		Quaternion ToQuaternion() const;

		std::string ToString() const;
	};
}

#endif //!GADGET_EULER_H