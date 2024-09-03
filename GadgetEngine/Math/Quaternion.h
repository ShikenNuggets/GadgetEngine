#ifndef GADGET_QUATERNION_H
#define GADGET_QUATERNION_H

#include "Math/Math.h"
#include "Math/Vector.h"

namespace Gadget{
	//Forward declarations
	class Euler;
	class Matrix2;
	class Matrix3;
	class Matrix4;
	class Matrix4x3;
	struct Vector2;
	struct Vector3;
	struct Vector4;

	class Quaternion{
	public:
		float w;
		float x;
		float y;
		float z;

		explicit constexpr Quaternion(float w_ = 1.0f, float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : w(w_), x(x_), y(y_), z(z_){}
		explicit constexpr Quaternion(float w_, const Vector3& v_);
		explicit constexpr Quaternion(const Vector4& v_);

		static inline constexpr Quaternion Identity(){ return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); }

		inline constexpr Quaternion operator -() const{ return Quaternion(-w, -x, -y, -z); }
		inline constexpr Quaternion operator +(const Quaternion& q_) const{ return Quaternion(w + q_.w, x + q_.x, y + q_.y, z + q_.z); }

		inline constexpr Quaternion operator *(const Quaternion& q_) const{
			return Quaternion(
				-x * q_.x - y * q_.y - z * q_.z + w * q_.w,
				x * q_.w + y * q_.z - z * q_.y + w * q_.x,
				-x * q_.z + y * q_.w + z * q_.x + w * q_.y,
				x * q_.y - y * q_.x + z * q_.w + w * q_.z
			);
		}

		inline constexpr Vector3 operator *(const Vector3& v_) const{
			Vector3 u = Vector3(x, y, z);
			float s = w;

			return 2.0f * Vector3::Dot(u, v_) * u + (s * s - Vector3::Dot(u, u)) * v_ + 2.0f * s * Vector3::Cross(u, v_);
		}
		
		inline constexpr Quaternion operator *(float f_) const{ return Quaternion(w * f_, x * f_, y * f_, z * f_); }
		inline constexpr Quaternion operator /(float f_) const{ return Quaternion(Math::SafeDivide(w, f_), Math::SafeDivide(x, f_), Math::SafeDivide(y, f_), Math::SafeDivide(z, f_)); }

		inline constexpr void operator +=(const Quaternion& q_){ *this = *this + q_; }
		inline constexpr void operator *=(const Quaternion& q_){ *this = *this * q_; }
		inline constexpr void operator *=(float f_){ *this = *this * f_; }
		inline constexpr void operator /=(float f_){ *this = *this / f_; }

		inline constexpr float SquaredMagnitude() const{ return (w * w) + (x * x) + (y * y) + (z * z); }
		float Magnitude() const;
		Quaternion Normalized() const;
		void Normalize();
		inline constexpr Quaternion Conjugate() const{ return Quaternion(w, -x, -y, -z); }

		inline constexpr Quaternion Inverse() const{
			const Quaternion conjugate = Conjugate();
			return conjugate / conjugate.SquaredMagnitude();
		}

		static inline constexpr float Dot(const Quaternion& a_, const Quaternion& b_){
			return Math::Dot4D(/*A*/ a_.w, a_.x, a_.y, a_.z, /*B*/ b_.w, b_.x, b_.y, b_.z);
		}

		static Quaternion Rotate(Angle angle_, const Vector3& axis_);
		static Angle GetRotationAngle(const Quaternion& q_);
		static Vector3 GetRotationAxis(const Quaternion& q_);
		static Quaternion LookAt(const Vector3& source_, const Vector3& destination_);

		static Quaternion Lerp(const Quaternion& q1_, const Quaternion& q2_, float t_);
		static Quaternion Slerp(const Quaternion& q1_, const Quaternion& q2_, float t_);

		Matrix3 ToMatrix3() const;
		Matrix4 ToMatrix4() const;
		Euler ToEuler() const;

		std::string ToString() const;

		inline constexpr bool IsNear(const Quaternion& b_) const{ return Math::Near(x, b_.x) && Math::Near(y, b_.y) && Math::Near(z, b_.z) && Math::Near(w, b_.w); }
		inline bool IsValid() const{ return Math::IsValidNumber(x) && Math::IsValidNumber(y) && Math::IsValidNumber(z) && Math::IsValidNumber(w); }
	};
}

#endif //!GADGET_QUATERNION_H