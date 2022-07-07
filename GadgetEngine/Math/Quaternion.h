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

		explicit Quaternion(float w_ = 1.0f, float x_ = 0.0f, float y = 0.0f, float z_ = 0.0f);
		Quaternion(float w_, const Vector3& v_);
		explicit Quaternion(const Vector4& v_);

		static inline Quaternion Identity(){ return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); }

		Quaternion operator +(const Quaternion& q_) const;
		Quaternion operator *(const Quaternion& q_) const;
		Quaternion operator *(float f_) const;
		Quaternion operator /(float f_) const;

		void operator +=(const Quaternion& q_);
		void operator *=(const Quaternion& q_);
		void operator *=(float f_);
		void operator /=(float f_);

		constexpr float SquaredMagnitude() const;
		static constexpr float SquaredMagnitude(const Quaternion& q_);
		float Magnitude() const;
		static float Magnitude(const Quaternion& q_);
		Quaternion Normalized() const;
		void Normalize();
		static Quaternion Normalized(const Quaternion& q_);
		Quaternion Conjugate() const;
		Quaternion Inverse() const;
		static constexpr float Dot(const Quaternion& a_, const Quaternion& b_);

		static Quaternion Rotate(Angle angle_, const Vector3& axis_);
		static Angle GetRotationAngle(const Quaternion& q_);
		static Vector3 GetRotationAxis(const Quaternion& q_);
		static Quaternion LookAt(const Vector3& source_, const Vector3& destination_);

		static Quaternion Lerp(const Quaternion& q1_, const Quaternion& q2_, float t_);
		static Quaternion Slerp(const Quaternion& q1_, const Quaternion& q2_, float t_);

		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Quaternion& q_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Quaternion& q_);
		/*Euler ToEuler() const;
		static Euler ToEuler(const Quaternion& q_);*/

		std::string ToString() const;
	};
}

#endif //!GADGET_QUATERNION_H