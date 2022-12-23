#ifndef GADGET_VECTOR_H
#define GADGET_VECTOR_H

#include <string>

#include "Math.h"

namespace Gadget{
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Vector2{
		float x;
		float y;

		explicit constexpr Vector2(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_){}

		static inline constexpr Vector2 Zero(){ return Vector2(0.0f, 0.0f); }
		static inline constexpr Vector2 Fill(float v_){ return Vector2(v_, v_); }

		inline constexpr Vector2 operator -() const{ return Vector2(-x, -y); }
		inline constexpr Vector2 operator +(const Vector2& v_) const{ return Vector2(x + v_.x, y + v_.y); }
		inline constexpr Vector2 operator -(const Vector2& v_) const{ return Vector2(x - v_.x, y - v_.y); }
		inline constexpr Vector2 operator *(float s_) const{ return Vector2(x * s_, y * s_); }

		inline constexpr Vector2 operator /(float s_) const{ return Vector2(Math::SafeDivide(x, s_), Math::SafeDivide(y, s_)); }

		inline friend constexpr Vector2 operator *(float s_, const Vector2& v_){ return v_ * s_; }

		inline constexpr void operator +=(const Vector2& v_){ *this = *this + v_; }
		inline constexpr void operator -=(const Vector2& v_){ *this = *this - v_; }
		inline constexpr void operator *=(float s_){ *this = *this * s_; }
		inline constexpr void operator /=(float s_){ *this = *this / s_; }

		inline constexpr float SquaredMagnitude() const{ return (x * x) + (y * y); }
		inline float Magnitude() const{ return Math::Sqrt(SquaredMagnitude()); }
		inline Vector2 Normalized() const{ return *this / Magnitude(); }
		inline void Normalize(){ *this = Normalized(); }
		static inline constexpr float Dot(const Vector2& a_, const Vector2& b_){ return (a_.x * b_.x) + (a_.y * b_.y); }
		static inline constexpr Vector2 Hadamard(const Vector2& a_, const Vector2& b_){ return Vector2(a_.x * b_.x, a_.y * b_.y); }

		inline std::string ToString() const{ return std::to_string(x) + ", " + std::to_string(y); }

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		inline constexpr operator const float* () const{ return static_cast<const float*>(&x); }
		inline constexpr operator float* (){ return static_cast<float*>(&x); }

		inline constexpr bool IsNear(const Vector2& b_){ return Math::Near(x, b_.x) && Math::Near(y, b_.y); }
	};

	struct Vector3{
		float x;
		float y;
		float z;

		explicit constexpr Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_){}

		static inline constexpr Vector3 Zero(){ return Vector3(0.0f, 0.0f, 0.0f); }
		static inline constexpr Vector3 Fill(float v_){ return Vector3(v_, v_, v_); }

		inline constexpr Vector3 operator -() const{ return Vector3(-x, -y, -z); }
		inline constexpr Vector3 operator +(const Vector2& v_) const{ return Vector3(x + v_.x, y + v_.y, z); }
		inline constexpr Vector3 operator +(const Vector3& v_) const{ return Vector3(x + v_.x, y + v_.y, z + v_.z); }
		inline constexpr Vector3 operator -(const Vector2& v_) const{ return Vector3(x - v_.x, y - v_.y, z); }
		inline constexpr Vector3 operator -(const Vector3& v_) const{ return Vector3(x - v_.x, y - v_.y, z - v_.z); }
		inline constexpr Vector3 operator *(float s_) const{ return Vector3(x * s_, y * s_, z * s_); }

		inline constexpr Vector3 operator /(float s_) const{ return Vector3(Math::SafeDivide(x, s_), Math::SafeDivide(y, s_), Math::SafeDivide(z, s_)); }

		inline friend constexpr Vector3 operator *(float s_, const Vector3& v_){ return v_ * s_; }

		inline constexpr void operator +=(const Vector2& v_){ *this = *this + v_; }
		inline constexpr void operator +=(const Vector3& v_){ *this = *this + v_; }
		inline constexpr void operator -=(const Vector2& v_){ *this = *this - v_; }
		inline constexpr void operator -=(const Vector3& v_){ *this = *this - v_; }
		inline constexpr void operator *=(float s_){ *this = *this * s_; }
		inline constexpr void operator /=(float s_){ *this = *this / s_; }

		inline constexpr float SquaredMagnitude() const{ return (x * x) + (y * y) + (z * z); }
		inline float Magnitude() const{ return Math::Sqrt(SquaredMagnitude()); }
		inline Vector3 Normalized() const{ return *this / Magnitude(); }
		inline void Normalize(){ *this = Normalized(); }
		static inline constexpr float Dot(const Vector3& a_, const Vector3& b_){ return(a_.x * b_.x + a_.y * b_.y + a_.z * b_.z); }
		static inline constexpr Vector3 Hadamard(const Vector3& a_, const Vector3& b_){ return Vector3(a_.x * b_.x, a_.y * b_.y, a_.z * b_.z); }

		static inline constexpr Vector3 Cross(const Vector3& a_, const Vector3& b_){
			return Vector3(	a_.y * b_.z - a_.z * b_.y,
							a_.z * b_.x - a_.x * b_.z,
							a_.x * b_.y - a_.y * b_.x);
		}

		static inline constexpr Vector3 Lerp(const Vector3& a_, const Vector3& b_, float t_){ return a_ + t_ * (b_ - a_); }

		static inline constexpr Vector3 Rotate(const Vector3& n_, float theta_, const Vector3& v_){ return v_ * cos(theta_) + Vector3::Dot(v_, n_) * n_ * (1.0f - cos(theta_)) + Vector3::Cross(n_, v_) * sin(theta_); }
		//Reflect off a normal  
		static inline constexpr Vector3 Reflect(const Vector3& v_, const Vector3& n_){ return ((2.0f * Vector3::Dot(v_, n_)) * n_ - v_); }

		inline std::string ToString() const{ return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z); }

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		inline constexpr operator const float* () const{ return static_cast<const float*>(&x); }
		inline constexpr operator float* (){ return static_cast<float*>(&x); }

		inline constexpr bool IsNear(const Vector3& b_){ return Math::Near(x, b_.x) && Math::Near(y, b_.y) && Math::Near(z, b_.z); }

		static inline constexpr Vector3 Up(){ return Vector3(0.0f, 1.0f, 0.0f); };
		static inline constexpr Vector3 Forward(){ return Vector3(0.0f, 0.0f, -1.0f); };
		static inline constexpr Vector3 Right(){ return Vector3(1.0f, 0.0f, 0.0f); }
	};

	struct Vector4{
		float x;
		float y;
		float z;
		float w;

		explicit constexpr Vector4(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 0.0f) : x(x_), y(y_), z(z_), w(w_){}

		static inline constexpr Vector4 Zero(){ return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
		static inline constexpr Vector4 Fill(float v_){ return Vector4(v_, v_, v_, v_); }

		inline constexpr Vector4 operator -() const{ return Vector4(-x, -y, -z, -w); }
		inline constexpr Vector4 operator +(const Vector2& v_) const{ return Vector4(x + v_.x, y + v_.y, z, w); }
		inline constexpr Vector4 operator +(const Vector3& v_) const{ return Vector4(x + v_.x, y + v_.y, z + v_.z, w); }
		inline constexpr Vector4 operator +(const Vector4& v_) const{ return Vector4(x + v_.x, y + v_.y, z + v_.z, w + v_.w); }
		inline constexpr Vector4 operator -(const Vector2& v_) const{ return Vector4(x - v_.x, y - v_.y, z, w); }
		inline constexpr Vector4 operator -(const Vector3& v_) const{ return Vector4(x - v_.x, y - v_.y, z - v_.z, w); }
		inline constexpr Vector4 operator -(const Vector4& v_) const{ return Vector4(x - v_.x, y - v_.y, z - v_.z, w - v_.w); }
		inline constexpr Vector4 operator *(float s_) const{ return Vector4(x * s_, y * s_, z * s_, w * s_); }

		inline constexpr Vector4 operator /(float s_) const{ return Vector4(Math::SafeDivide(x, s_), Math::SafeDivide(y, s_), Math::SafeDivide(z, s_), Math::SafeDivide(w, s_)); }

		inline friend constexpr Vector4 operator *(float s_, const Vector4& v_){ return v_ * s_; }

		inline constexpr void operator +=(const Vector2& v_){ *this = *this + v_; }
		inline constexpr void operator +=(const Vector3& v_){ *this = *this + v_; }
		inline constexpr void operator +=(const Vector4& v_){ *this = *this + v_; }
		inline constexpr void operator -=(const Vector2& v_){ *this = *this - v_; }
		inline constexpr void operator -=(const Vector3& v_){ *this = *this - v_; }
		inline constexpr void operator -=(const Vector4& v_){ *this = *this - v_; }
		inline constexpr void operator *=(float s_){ *this = *this * s_; }
		inline constexpr void operator /=(float s_){ *this = *this / s_; }

		inline constexpr float SquaredMagnitude() const{ return (x * x) + (y * y) + (z * z) + (w * w); }
		inline float Magnitude() const{ return Math::Sqrt(SquaredMagnitude()); }
		inline Vector4 Normalized() const{ return *this / Magnitude(); }
		inline void Normalize(){ *this = Normalized(); }
		static inline constexpr float Dot(const Vector4& a_, const Vector4& b_){ return (a_.x * b_.x) + (a_.y * b_.y) + (a_.z * b_.z) + (a_.w * a_.w); }
		static inline constexpr Vector4 Hadamard(const Vector4& a_, const Vector4& b_){ return Vector4(a_.x * b_.x, a_.y * b_.y, a_.z * b_.z, a_.w * b_.w); }

		inline std::string ToString() const{ return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w); }

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		inline constexpr operator const float* () const{ return static_cast<const float*>(&x); }
		inline constexpr operator float* (){ return static_cast<float*>(&x); }

		inline constexpr bool IsNear(const Vector4& b_){ return Math::Near(x, b_.x) && Math::Near(y, b_.y) && Math::Near(z, b_.z) && Math::Near(w, b_.w); }
	};
}

#endif //!GADGET_VECTOR_H