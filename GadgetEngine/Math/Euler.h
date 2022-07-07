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

		explicit Euler(Angle x_ = 0.0f, Angle y_ = 0.0f, Angle z_ = 0.0f);
		explicit Euler(const Vector3& v_);

		Euler operator -() const;
		Euler operator +(const Euler& e_) const;
		Euler operator -(const Euler& e_) const;
		Euler operator *(float f) const;
		Euler operator /(float f) const;
		void operator +=(const Euler& e_);
		void operator -=(const Euler& e_);
		void operator *=(float f);
		void operator /=(float f);

		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Euler& e_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Euler& e_);
		Quaternion ToQuaternion() const;
		static Quaternion ToQuaternion(const Euler& e_);

		std::string ToString() const;
	};
}

#endif //!GADGET_EULER_H