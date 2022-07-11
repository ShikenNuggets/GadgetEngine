#ifndef GADGET_QUATERNION_H
#define GADGET_QUATERNION_H

#include "Matrix.h"

namespace Gadget{
	class Quaternion{
	public:
		float w;
		float x;
		float y;
		float z;

		explicit constexpr Quaternion(float w_ = 1.0f, float x_ = 0.0f, float y = 0.0f, float z_ = 0.0f);
		constexpr Quaternion(float w_, const Vector3& v_);
		explicit constexpr Quaternion(const Vector4& v_);

		static inline constexpr Quaternion Identity(){ return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); }

		constexpr Quaternion operator +(const Quaternion& q_) const;
		constexpr Quaternion operator *(const Quaternion& q_) const;
		constexpr Quaternion operator *(float f_) const;
		constexpr Quaternion operator /(float f_) const;

		constexpr void operator +=(const Quaternion& q_);
		constexpr void operator *=(const Quaternion& q_);
		constexpr void operator *=(float f_);
		constexpr void operator /=(float f_);

		constexpr float SquaredMagnitude() const;
		float Magnitude() const;
		Quaternion Normalized() const;
		void Normalize();
		constexpr Quaternion Conjugate() const;
		constexpr Quaternion Inverse() const;

		static inline constexpr float Dot(const Quaternion& a_, const Quaternion& b_){
			return Math::Dot4D(/*A*/ a_.w, a_.x, a_.y, a_.z, /*B*/ b_.w, b_.x, b_.y, b_.z);
		}

		static Quaternion Rotate(Angle angle_, const Vector3& axis_);
		static Angle GetRotationAngle(const Quaternion& q_);
		static Vector3 GetRotationAxis(const Quaternion& q_);
		static Quaternion LookAt(const Vector3& source_, const Vector3& destination_);

		static Quaternion Lerp(const Quaternion& q1_, const Quaternion& q2_, float t_);
		static Quaternion Slerp(const Quaternion& q1_, const Quaternion& q2_, float t_);

		constexpr Matrix3 ToMatrix3() const;
		constexpr Matrix4 ToMatrix4() const;
		Euler ToEuler() const;

		std::string ToString() const;
	};
}

#endif //!GADGET_QUATERNION_H