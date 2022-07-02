#ifndef GADGET_VECTOR_H
#define GADGET_VECTOR_H

#include <iostream>
#include <string>

#include "Math.h"

namespace Gadget{
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Vector2{
		float x;
		float y;

		explicit Vector2(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_){}

		static inline Vector2 Zero(){ return Vector2(0.0f, 0.0f); }
		static inline Vector2 Fill(float v_){ return Vector2(v_, v_); }

		inline Vector2 operator -() const{ return Vector2(-x, -y); }
		inline Vector2 operator +(const Vector2& v_) const{ return Vector2(x + v_.x, y + v_.y); }
		inline Vector2 operator -(const Vector2& v_) const{ return Vector2(x - v_.x, y - v_.y); }
		inline Vector2 operator *(float s_) const{ return Vector2(x * s_, y * s_); }

		inline Vector2 operator /(float s_) const{ return Vector2(Math::SafeDivide(x, s_), Math::SafeDivide(y, s_)); }

		inline friend Vector2 operator *(float s_, const Vector2& v_){ return v_ * s_; }

		inline void operator +=(const Vector2& v_){ *this = *this + v_; }
		inline void operator -=(const Vector2& v_){ *this = *this - v_; }
		inline void operator *=(float s_){ *this = *this * s_; }
		inline void operator /=(float s_){ *this = *this / s_; }

		inline constexpr float SquaredMagnitude() const{ return (x * x) + (y * y); }
		static inline constexpr float SquaredMagnitude(const Vector2& v_){ return v_.SquaredMagnitude(); }
		inline float Magnitude() const{ return Math::Sqrt(SquaredMagnitude()); }
		static inline float Magnitude(const Vector2& v_){ return v_.Magnitude(); }
		inline Vector2 Normalized() const{ return *this / Magnitude(); }
		inline void Normalize(){ *this = Normalized(); }
		static inline Vector2 Normalized(const Vector2& v_){ return v_.Normalized(); }
		static inline float Dot(const Vector2& a_, const Vector2& b_){ return (a_.x * b_.x) + (a_.y * b_.y); }
		static inline Vector2 Hadamard(const Vector2& a_, const Vector2& b_){ return Vector2(a_.x * b_.x, a_.y * b_.y); }

		inline std::string ToString() const{ return std::to_string(x) + ", " + std::to_string(y); }

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		inline constexpr operator const float* () const{ return static_cast<const float*>(&x); }
		inline constexpr operator float* (){ return static_cast<float*>(&x); }
	};

	struct Vector3{
		float x;
		float y;
		float z;

		explicit Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_){}

		static inline Vector3 Zero(){ return Vector3(0.0f, 0.0f, 0.0f); }
		static inline Vector3 Fill(float v_){ return Vector3(v_, v_, v_); }

		inline Vector3 operator -() const{ return Vector3(-x, -y, -z); }
		inline Vector3 operator +(const Vector2& v_) const{ return Vector3(x + v_.x, y + v_.y, z); }
		inline Vector3 operator +(const Vector3& v_) const{ return Vector3(x + v_.x, y + v_.y, z + v_.z); }
		inline Vector3 operator -(const Vector2& v_) const{ return Vector3(x - v_.x, y - v_.y, z); }
		inline Vector3 operator -(const Vector3& v_) const{ return Vector3(x - v_.x, y - v_.y, z - v_.z); }
		inline Vector3 operator *(float s_) const{ return Vector3(x * s_, y * s_, z * s_); }

		inline Vector3 operator /(float s_) const{ return Vector3(Math::SafeDivide(x, s_), Math::SafeDivide(y, s_), Math::SafeDivide(z, s_)); }

		inline friend Vector3 operator *(float s_, const Vector3& v_){ return v_ * s_; }

		inline void operator +=(const Vector2& v_){ *this = *this + v_; }
		inline void operator +=(const Vector3& v_){ *this = *this + v_; }
		inline void operator -=(const Vector2& v_){ *this = *this - v_; }
		inline void operator -=(const Vector3& v_){ *this = *this - v_; }
		inline void operator *=(float s_){ *this = *this * s_; }
		inline void operator /=(float s_){ *this = *this / s_; }

		inline constexpr float SquaredMagnitude() const{ return (x * x) + (y * y) + (z * z); }
		static inline constexpr float SquaredMagnitude(const Vector3& v_){ return v_.SquaredMagnitude(); }
		inline float Magnitude() const{ return Math::Sqrt(SquaredMagnitude()); }
		static inline float Magnitude(const Vector3& v_){ return v_.Magnitude(); }
		inline Vector3 Normalized() const{ return *this / Magnitude(); }
		inline void Normalize(){ *this = Normalized(); }
		static inline Vector3 Normalized(const Vector3& v_){ return v_.Normalized(); }
		static inline constexpr float Dot(const Vector3& a_, const Vector3& b_){ return(a_.x * b_.x + a_.y * b_.y + a_.z * b_.z); }
		static inline Vector3 Hadamard(const Vector3& a_, const Vector3& b_){ return Vector3(a_.x * b_.x, a_.y * b_.y, a_.z * b_.z); }

		static inline Vector3 Cross(const Vector3& a_, const Vector3& b_){
			return Vector3(	a_.y * b_.z - a_.z * b_.y,
							a_.z * b_.x - a_.x * b_.z,
							a_.x * b_.y - a_.y * b_.x);
		}

		static inline Vector3 Lerp(const Vector3& a_, const Vector3& b_, float t_){ return a_ + t_ * (b_ - a_); }

		static inline Vector3 Rotate(const Vector3& n_, float theta_, const Vector3& v_){ return v_ * cos(theta_) + Vector3::Dot(v_, n_) * n_ * (1.0f - cos(theta_)) + Vector3::Cross(n_, v_) * sin(theta_); }
		//Reflect off a normal  
		static inline Vector3 Reflect(const Vector3& v_, const Vector3& n_){ return ((2.0f * Vector3::Dot(v_, n_)) * n_ - v_); }

		inline std::string ToString() const{ return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z); }

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		inline constexpr operator const float* () const{ return static_cast<const float*>(&x); }
		inline constexpr operator float* (){ return static_cast<float*>(&x); }

		static inline const Vector3 Up(){ return Vector3(0.0f, 1.0f, 0.0f); };
		static inline const Vector3 Forward(){ return Vector3(0.0f, 0.0f, -1.0f); };
		static inline const Vector3 Right(){ return Vector3(1.0f, 0.0f, 0.0f); }
	};

	struct Vector4{
		float x;
		float y;
		float z;
		float w;

		explicit Vector4(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 0.0f) : x(x_), y(y_), z(z_), w(w_){}

		static inline Vector4 Zero(){ return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
		static inline Vector4 Fill(float v_){ return Vector4(v_, v_, v_, v_); }

		inline Vector4 operator -() const{ return Vector4(-x, -y, -z, -w); }
		inline Vector4 operator +(const Vector2& v_) const{ return Vector4(x + v_.x, y + v_.y, z, w); }
		inline Vector4 operator +(const Vector3& v_) const{ return Vector4(x + v_.x, y + v_.y, z + v_.z, w); }
		inline Vector4 operator +(const Vector4& v_) const{ return Vector4(x + v_.x, y + v_.y, z + v_.z, w + v_.w); }
		inline Vector4 operator -(const Vector2& v_) const{ return Vector4(x - v_.x, y - v_.y, z, w); }
		inline Vector4 operator -(const Vector3& v_) const{ return Vector4(x - v_.x, y - v_.y, z - v_.z, w); }
		inline Vector4 operator -(const Vector4& v_) const{ return Vector4(x - v_.x, y - v_.y, z - v_.z, w - v_.w); }
		inline Vector4 operator *(float s_) const{ return Vector4(x * s_, y * s_, z * s_, w * s_); }

		inline Vector4 operator /(float s_) const{ return Vector4(Math::SafeDivide(x, s_), Math::SafeDivide(y, s_), Math::SafeDivide(z, s_), Math::SafeDivide(w, s_)); }

		inline friend Vector4 operator *(float s_, const Vector4& v_){ return v_ * s_; }

		inline void operator +=(const Vector2& v_){ *this = *this + v_; }
		inline void operator +=(const Vector3& v_){ *this = *this + v_; }
		inline void operator +=(const Vector4& v_){ *this = *this + v_; }
		inline void operator -=(const Vector2& v_){ *this = *this - v_; }
		inline void operator -=(const Vector3& v_){ *this = *this - v_; }
		inline void operator -=(const Vector4& v_){ *this = *this - v_; }
		inline void operator *=(float s_){ *this = *this * s_; }
		inline void operator /=(float s_){ *this = *this / s_; }

		inline constexpr float SquaredMagnitude() const{ return (x * x) + (y * y) + (z * z) + (w * w); }
		static inline constexpr float SquaredMagnitude(const Vector4& v_){ return v_.SquaredMagnitude(); }
		inline float Magnitude() const{ return Math::Sqrt(SquaredMagnitude()); }
		static inline float Magnitude(const Vector4& v_){ return v_.Magnitude(); }
		inline Vector4 Normalized() const{ return *this / Magnitude(); }
		inline void Normalize(){ *this = Normalized(); }
		static inline Vector4 Normalized(const Vector4& v_){ return v_.Normalized(); }
		static inline constexpr float Dot(const Vector4& a_, const Vector4& b_){ return (a_.x * b_.x) + (a_.y * b_.y) + (a_.z * b_.z) + (a_.w * a_.w); }
		static inline Vector4 Hadamard(const Vector4& a_, const Vector4& b_){ return Vector4(a_.x * b_.x, a_.y * b_.y, a_.z * b_.z, a_.w * b_.w); }

		inline std::string ToString() const{ return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w); }

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		inline constexpr operator const float* () const{ return static_cast<const float*>(&x); }
		inline constexpr operator float* (){ return static_cast<float*>(&x); }
	};
}

#endif //!GADGET_VECTOR_H