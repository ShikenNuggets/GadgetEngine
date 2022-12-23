#ifndef GADGET_MATRIX_H
#define GADGET_MATRIX_H

#include <string>

#include "Debug.h"
#include "Vector.h"

namespace Gadget{
	//Forward declarations
	class Euler;
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
		Matrix2();
		Matrix2(float x1_, float x2_, float y1_, float y2_);
		explicit Matrix2(float fill_);
		explicit Matrix2(const Matrix3& m_);
		explicit Matrix2(const Matrix4& m_);
		explicit Matrix2(const Matrix4x3& m_);

		//This allows us to get elements
		inline constexpr float operator [](unsigned int i_) const{
			GADGET_ASSERT(i_ < mat2Size, "Invalid array access! " + std::to_string(i_) + " must be less than " + std::to_string(mat2Size) + "!");
			return m[i_];
		}

		float& operator [](unsigned int i_); //This allows us to set elements

		Matrix2 operator +(const Matrix2& m_) const;
		Matrix2 operator -(const Matrix2& m_) const;
		Matrix2 operator *(float s_) const;
		Matrix2 operator *(const Matrix2& m_) const;
		Matrix2 operator /(float s_) const;
		void operator +=(const Matrix2& m_);
		void operator -=(const Matrix2& m_);
		void operator *=(float s_);
		void operator *=(const Matrix2& m_);
		void operator /=(float s_);

		operator float* ();
		operator const float* () const;

		static inline Matrix2 Identity(){
			return Matrix2(	1.0f, 0.0f,
							0.0f, 1.0f);
		}

		Matrix2 Transpose() const;
		float Determinant() const;
		Matrix2 Inverse() const;

		Matrix3 ToMatrix3() const;
		Matrix4 ToMatrix4() const;
		Matrix4x3 ToMatrix4x3() const;

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
		Matrix3();
		Matrix3(float x1_, float x2_, float x3_,
				float y1_, float y2_, float y3_,
				float z1_, float z2_, float z3_);
		explicit Matrix3(float fill_);
		explicit Matrix3(const Matrix2& m_);
		explicit Matrix3(const Matrix4& m_);
		explicit Matrix3(const Matrix4x3& m_);

		Matrix3& operator =(const Matrix4& m_);

		constexpr float operator [](unsigned int i_) const{
			GADGET_ASSERT(i_ < mat3Size, "Invalid array access! " + std::to_string(i_) + " must be less than " + std::to_string(mat3Size) + "!");
			return m[i_];
		}

		float& operator [](unsigned int i_); //This allows us to set elements

		Matrix3 operator +(const Matrix3& m_) const;
		Matrix3 operator -(const Matrix3& m_) const;
		Matrix3 operator *(float s_) const;
		Matrix3 operator *(const Matrix3& m_) const;
		Matrix3 operator /(float s_) const;
		void operator +=(const Matrix3& m_);
		void operator -=(const Matrix3& m_);
		void operator *=(float s_);
		void operator *=(const Matrix3& m_);
		void operator /=(float s_);

		operator float* ();
		operator const float* () const;

		static inline Matrix3 Identity(){
			return Matrix3(	1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f);
		}

		Matrix3 Transpose() const;
		float Determinant() const;
		Matrix3 Inverse() const;

		Matrix2 ToMatrix2() const;
		Matrix4 ToMatrix4() const;
		Matrix4x3 ToMatrix4x3() const;
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
		Matrix4();
		Matrix4(float x1_, float x2_, float x3_, float x4_,
				float y1_, float y2_, float y3_, float y4_,
				float z1_, float z2_, float z3_, float z4_,
				float w1_, float w2_, float w3_, float w4_);
		explicit Matrix4(float fill_);
		explicit Matrix4(const Matrix2& m_);
		explicit Matrix4(const Matrix3& m_);
		explicit Matrix4(const Matrix4x3& m_);

		constexpr float operator [](unsigned int i_) const{
			GADGET_ASSERT(i_ < mat4Size, "Invalid array access! " + std::to_string(i_) + " must be less than " + std::to_string(mat4Size) + "!");
			return m[i_];
		}

		float& operator [](unsigned int i_); //This allows us to set elements

		Matrix4 operator +(const Matrix4& m_) const;
		Matrix4 operator -(const Matrix4& m_) const;
		Matrix4 operator *(float s_) const;
		Matrix4 operator *(const Matrix4& m_) const;
		Vector3 operator *(const Vector3& v_) const;
		Vector4 operator *(const Vector4& v_) const;
		Matrix4 operator /(float s_) const;
		void operator +=(const Matrix4& m_);
		void operator -=(const Matrix4& m_);
		void operator *=(float s_);
		void operator *=(const Matrix4& m_);
		void operator /=(float s_);

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

		static inline Matrix4 Identity(){
			return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4 Transpose() const;
		Matrix4 Inverse() const;

		static inline Matrix4 Translate(const Vector3& v_){
			return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							v_.x, v_.y, v_.z, 1.0f);
		}

		static Matrix4 Rotate(float angle_, const Vector3& v_);

		static inline Matrix4 Scale(const Vector3& v_){
			return Matrix4(	v_.x, 0.0f, 0.0f, 0.0f,
							0.0f, v_.y, 0.0f, 0.0f,
							0.0f, 0.0f, v_.z, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		Vector3 GetTranslation() const;
		Quaternion GetRotation() const;
		Vector3 GetScale() const;

		static Matrix4 LookAt(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ, float upX, float upY, float upZ);
		static Matrix4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

		//These could be constexpr, but static constexpr implies inline which is not necessarily desirable here
		static Matrix4 Orthographic(float left_, float right_, float bottom_, float top_);
		static Matrix4 Orthographic(float left_, float right_, float bottom_, float top_, float near_, float far_);
		static Matrix4 Perspective(float fov_, float aspect_, float nearPlane_, float farPlane_);
		static Matrix4 UnOrtho(const Matrix4& ortho);
		static Matrix4 ViewportNDC(int w_, int h_);

		Matrix2 ToMatrix2() const;
		Matrix3 ToMatrix3() const;
		Matrix4x3 ToMatrix4x3() const;
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
		Matrix4x3();
		Matrix4x3(float x1_, float x2_, float x3_,
			float y1_, float y2_, float y3_,
			float z1_, float z2_, float z3_,
			float w1_, float w2_, float w3_);
		explicit Matrix4x3(float fill_);
		explicit Matrix4x3(const Matrix2& m_);
		explicit Matrix4x3(const Matrix3& m_);
		explicit Matrix4x3(const Matrix4& m_);

		constexpr float operator [](unsigned int i_) const{
			GADGET_ASSERT(i_ < mat4x3Size, "Invalid array access! " + std::to_string(i_) + " must be less than " + std::to_string(mat4x3Size) + "!");
			return m[i_];
		}

		float& operator [](unsigned int i_); //This allows us to set elements

		Matrix4x3 operator +(const Matrix4x3& m_) const;
		Matrix4x3 operator -(const Matrix4x3& m_) const;
		Matrix4x3 operator *(float s_) const;
		Vector3 operator *(const Vector3& v_) const;
		Matrix4x3 operator /(float s_) const;
		void operator +=(const Matrix4x3& m_);
		void operator -=(const Matrix4x3& m_);
		void operator *=(float s_);
		void operator /=(float s_);

		constexpr bool operator ==(const Matrix4x3& m_) {
			for (int i = 0; i < 16; i++) {
				if (m[i] != m_.m[i]) {
					return false;
				}
			}

			return true;
		}

		operator float* ();
		operator const float* () const;

		static inline Matrix4x3 Identity(){
			return Matrix4x3(1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f,
							0.0f, 0.0f, 0.0f);
		}

		static inline Matrix4x3 Translate(const Vector3& v_){
			return Matrix4x3(1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f,
							v_.x, v_.y, v_.z);
		}

		static Matrix4x3 Rotate(float angle_, const Vector3& v_);

		static inline Matrix4x3 Scale(const Vector3& v_){
			return Matrix4x3(v_.x, 0.0f, 0.0f,
							0.0f, v_.y, 0.0f,
							0.0f, 0.0f, v_.z,
							0.0f, 0.0f, 0.0f);
		}

		Vector3 GetTranslation() const;
		Quaternion GetRotation() const;
		Vector3 GetScale() const;

		Matrix2 ToMatrix2() const;
		Matrix3 ToMatrix3() const;
		Matrix4 ToMatrix4() const;
		Euler ToEuler() const;
		Quaternion ToQuaternion() const;

		std::string ToString() const;

	private:
		static constexpr int mat4x3Size = 12;
		float m[mat4x3Size];
	};
}

#endif //!MATRIX_H