#ifndef GADGET_MATRIX_H
#define GADGET_MATRIX_H

#include "Vector.h"
#include "Euler.h"

namespace Gadget{
	class Matrix2;
	class Matrix3;
	class Matrix4;
	class Matrix4x3;
	class Quaternion;

	//Column Major Order
	//[0][2]
	//[1][3]
	class Matrix2{
	public:
		constexpr Matrix2();
		constexpr Matrix2(float x1_, float x2_, float y1_, float y2_);
		explicit constexpr Matrix2(float fill_);
		explicit constexpr Matrix2(const Matrix3& m_);
		explicit constexpr Matrix2(const Matrix4& m_);
		explicit constexpr Matrix2(const Matrix4x3& m_);

		constexpr float operator [](unsigned int i_) const; //This allows us to get elements
		constexpr float& operator [](unsigned int i_); //This allows us to set elements

		constexpr Matrix2 operator +(const Matrix2& m_) const;
		constexpr Matrix2 operator -(const Matrix2& m_) const;
		constexpr Matrix2 operator *(float s_) const;
		constexpr Matrix2 operator *(const Matrix2& m_) const;
		constexpr Matrix2 operator /(float s_) const;
		constexpr void operator +=(const Matrix2& m_);
		constexpr void operator -=(const Matrix2& m_);
		constexpr void operator *=(float s_);
		constexpr void operator *=(const Matrix2& m_);
		constexpr void operator /=(float s_);

		constexpr operator float* ();
		constexpr operator const float* () const;

		static inline constexpr Matrix2 Identity(){
			return Matrix2(	1.0f, 0.0f,
							0.0f, 1.0f);
		}

		constexpr Matrix2 Transpose() const;
		constexpr float Determinant() const;
		constexpr Matrix2 Inverse() const;

		constexpr Matrix3 ToMatrix3() const;
		constexpr Matrix4 ToMatrix4() const;
		constexpr Matrix4x3 ToMatrix4x3() const;

		std::string ToString() const;

	private:
		static constexpr int mat2Size = 4;
		float m[mat2Size];
	};

	//Column Major Order
	//[0][3][6]
	//[1][4][7]
	//[2][5][8]
	class Matrix3{
	public:
		constexpr Matrix3();
		constexpr Matrix3(float x1_, float x2_, float x3_,
				float y1_, float y2_, float y3_,
				float z1_, float z2_, float z3_);
		explicit constexpr Matrix3(float fill_);
		explicit constexpr Matrix3(const Matrix2& m_);
		explicit constexpr Matrix3(const Matrix4& m_);
		explicit constexpr Matrix3(const Matrix4x3& m_);

		constexpr Matrix3& operator =(const Matrix4& m_);

		constexpr float operator [](unsigned int i_) const; //This allows us to get elements
		constexpr float& operator [](unsigned int i_); //This allows us to set elements

		constexpr Matrix3 operator +(const Matrix3& m_) const;
		constexpr Matrix3 operator -(const Matrix3& m_) const;
		constexpr Matrix3 operator *(float s_) const;
		constexpr Matrix3 operator *(const Matrix3& m_) const;
		constexpr Matrix3 operator /(float s_) const;
		constexpr void operator +=(const Matrix3& m_);
		constexpr void operator -=(const Matrix3& m_);
		constexpr void operator *=(float s_);
		constexpr void operator *=(const Matrix3& m_);
		constexpr void operator /=(float s_);

		constexpr operator float* ();
		constexpr operator const float* () const;

		static inline constexpr Matrix3 Identity(){
			return Matrix3(	1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f);
		}

		constexpr Matrix3 Transpose() const;
		constexpr float Determinant() const;
		constexpr Matrix3 Inverse() const;

		constexpr Matrix2 ToMatrix2() const;
		constexpr Matrix4 ToMatrix4() const;
		constexpr Matrix4x3 ToMatrix4x3() const;
		Euler ToEuler() const;
		Quaternion ToQuaternion() const;

		std::string ToString() const;

	private:
		static constexpr int mat3Size = 9;
		float m[mat3Size];
	};

	//Column Major Order
	//[00][04][08][12]
	//[01][05][09][13]
	//[02][06][10][14]
	//[03][07][11][15]
	class Matrix4{
	public:
		constexpr Matrix4();
		constexpr Matrix4(float x1_, float x2_, float x3_, float x4_,
				float y1_, float y2_, float y3_, float y4_,
				float z1_, float z2_, float z3_, float z4_,
				float w1_, float w2_, float w3_, float w4_);
		explicit constexpr Matrix4(float fill_);
		explicit constexpr Matrix4(const Matrix2& m_);
		explicit constexpr Matrix4(const Matrix3& m_);
		explicit constexpr Matrix4(const Matrix4x3& m_);

		constexpr float operator [](unsigned int i_) const; //This allows us to get elements
		constexpr float& operator [](unsigned int i_); //This allows us to set elements

		constexpr Matrix4 operator +(const Matrix4& m_) const;
		constexpr Matrix4 operator -(const Matrix4& m_) const;
		constexpr Matrix4 operator *(float s_) const;
		constexpr Matrix4 operator *(const Matrix4& m_) const;
		constexpr Vector3 operator *(const Vector3& v_) const;
		constexpr Vector4 operator *(const Vector4& v_) const;
		constexpr Matrix4 operator /(float s_) const;
		constexpr void operator +=(const Matrix4& m_);
		constexpr void operator -=(const Matrix4& m_);
		constexpr void operator *=(float s_);
		constexpr void operator *=(const Matrix4& m_);
		constexpr void operator /=(float s_);

		constexpr bool operator ==(const Matrix4& m_){
			for(int i = 0; i < 16; i++){
				if(m[i] != m_.m[i]){
					return false;
				}
			}

			return true;
		}

		inline constexpr operator float* (){ return static_cast<float*>(&m[0]); }
		inline constexpr operator const float* () const{ return static_cast<const float*>(&m[0]); }

		static inline constexpr Matrix4 Identity(){
			return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		constexpr Matrix4 Transpose() const;
		constexpr Matrix4 Inverse() const;

		static inline constexpr Matrix4 Translate(const Vector3& v_){
			return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							v_.x, v_.y, v_.z, 1.0f);
		}

		static Matrix4 Rotate(float angle_, const Vector3& v_);

		static inline constexpr Matrix4 Scale(const Vector3& v_){
			return Matrix4(	v_.x, 0.0f, 0.0f, 0.0f,
							0.0f, v_.y, 0.0f, 0.0f,
							0.0f, 0.0f, v_.z, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		constexpr Vector3 GetTranslation() const;
		Quaternion GetRotation() const;
		constexpr Vector3 GetScale() const;

		static Matrix4 LookAt(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ, float upX, float upY, float upZ);
		static Matrix4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

		//These could be constexpr, but static constexpr implies inline which is not necessarily desirable here
		static Matrix4 Orthographic(float left_, float right_, float bottom_, float top_);
		static Matrix4 Orthographic(float left_, float right_, float bottom_, float top_, float near_, float far_);
		static Matrix4 Perspective(float fov_, float aspect_, float nearPlane_, float farPlane_);
		static Matrix4 UnOrtho(const Matrix4& ortho);
		static Matrix4 ViewportNDC(int w_, int h_);

		constexpr Matrix2 ToMatrix2() const;
		constexpr Matrix3 ToMatrix3() const;
		constexpr Matrix4x3 ToMatrix4x3() const;
		Euler ToEuler() const;
		Quaternion ToQuaternion() const;

		std::string ToString() const;

	private:
		static constexpr int mat4Size = 16;
		float m[mat4Size];
	};

	//Column Major Order
	//[00][03][06][09]
	//[01][04][07][10]
	//[02][05][08][11]
	class Matrix4x3{
	public:
		constexpr Matrix4x3();
		constexpr Matrix4x3(float x1_, float x2_, float x3_,
			float y1_, float y2_, float y3_,
			float z1_, float z2_, float z3_,
			float w1_, float w2_, float w3_);
		explicit constexpr Matrix4x3(float fill_);
		explicit constexpr Matrix4x3(const Matrix2& m_);
		explicit constexpr Matrix4x3(const Matrix3& m_);
		explicit constexpr Matrix4x3(const Matrix4& m_);

		constexpr float operator [](unsigned int i_) const; //This allows us to get elements
		constexpr float& operator [](unsigned int i_); //This allows us to set elements

		constexpr Matrix4x3 operator +(const Matrix4x3& m_) const;
		constexpr Matrix4x3 operator -(const Matrix4x3& m_) const;
		constexpr Matrix4x3 operator *(float s_) const;
		constexpr Vector3 operator *(const Vector3& v_) const;
		constexpr Matrix4x3 operator /(float s_) const;
		constexpr void operator +=(const Matrix4x3& m_);
		constexpr void operator -=(const Matrix4x3& m_);
		constexpr void operator *=(float s_);
		constexpr void operator /=(float s_);

		constexpr bool operator ==(const Matrix4x3& m_) {
			for (int i = 0; i < 16; i++) {
				if (m[i] != m_.m[i]) {
					return false;
				}
			}

			return true;
		}

		constexpr operator float* ();
		constexpr operator const float* () const;

		static inline constexpr Matrix4x3 Identity(){
			return Matrix4x3(1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f,
							0.0f, 0.0f, 0.0f);
		}

		static inline constexpr Matrix4x3 Translate(const Vector3& v_){
			return Matrix4x3(1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f,
							v_.x, v_.y, v_.z);
		}

		static Matrix4x3 Rotate(float angle_, const Vector3& v_);

		static inline constexpr Matrix4x3 Scale(const Vector3& v_){
			return Matrix4x3(v_.x, 0.0f, 0.0f,
							0.0f, v_.y, 0.0f,
							0.0f, 0.0f, v_.z,
							0.0f, 0.0f, 0.0f);
		}

		constexpr Vector3 GetTranslation() const;
		Quaternion GetRotation() const;
		constexpr Vector3 GetScale() const;

		constexpr Matrix2 ToMatrix2() const;
		constexpr Matrix3 ToMatrix3() const;
		constexpr Matrix4 ToMatrix4() const;
		Euler ToEuler() const;
		Quaternion ToQuaternion() const;

		std::string ToString() const;

	private:
		static constexpr int mat4x3Size = 12;
		float m[mat4x3Size];
	};
}

#endif //!MATRIX_H