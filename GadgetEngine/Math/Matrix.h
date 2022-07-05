#ifndef GADGET_MATRIX_H
#define GADGET_MATRIX_H

#include "Vector.h"

namespace Gadget{
	class Matrix2;
	class Matrix3;
	class Matrix4;
	class Matrix4x3;

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

		constexpr float operator [](int i_) const; //This allows us to get elements
		constexpr float& operator [](int i_); //This allows us to set elements

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

		constexpr operator float* ();
		constexpr operator const float* () const;

		static Matrix2 Identity();
		Matrix2 Transpose() const;
		static Matrix2 Transpose(const Matrix2& m_);
		constexpr float Determinant() const;
		static constexpr float Determinant(const Matrix2& m_);
		Matrix2 Inverse() const;
		static Matrix2 Inverse(const Matrix2& m_);

		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Matrix2& m_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Matrix2& m_);
		Matrix4x3 ToMatrix4x3() const;
		static Matrix4x3 ToMatrix4x3(const Matrix2& m_);

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

		constexpr float operator [](int i_) const; //This allows us to get elements
		constexpr float& operator [](int i_); //This allows us to set elements

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

		constexpr operator float* ();
		constexpr operator const float* () const;

		static Matrix3 Identity();
		Matrix3 Transpose() const;
		static Matrix3 Transpose(const Matrix3& m_);
		constexpr float Determinant() const;
		static constexpr float Determinant(const Matrix3& m_);
		Matrix3 Inverse() const;
		static Matrix3 Inverse(const Matrix3& m_);

		Matrix2 ToMatrix2() const;
		static Matrix2 ToMatrix2(const Matrix3& m_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Matrix3& m_);
		Matrix4x3 ToMatrix4x3() const;
		static Matrix4x3 ToMatrix4x3(const Matrix3& m_);

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

		constexpr float operator [](int i_) const; //This allows us to get elements
		constexpr float& operator [](int i_); //This allows us to set elements

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

		constexpr operator float* ();
		constexpr operator const float* () const;

		static Matrix4 Identity();
		Matrix4 Transpose() const;
		static Matrix4 Transpose(const Matrix4& m_);
		Matrix4 Inverse() const;
		static Matrix4 Inverse(const Matrix4& m_);

		static Matrix4 Translate(const Vector3& v_);
		static Matrix4 Rotate(float angle_, const Vector3& v_);
		static Matrix4 Scale(const Vector3& v_);

		Vector3 GetTranslation() const;
		//Quaternion GetRotation() const;
		Vector3 GetScale() const;

		static Matrix4 LookAt(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ, float upX, float upY, float upZ);
		static Matrix4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

		static Matrix4 Orthographic(float left_, float right_, float bottom_, float top_);
		static Matrix4 Orthographic(float left_, float right_, float bottom_, float top_, float near_, float far_);
		static Matrix4 UnOrtho(const Matrix4& ortho);
		static Matrix4 ViewportNDC(int w_, int h_);

		Matrix2 ToMatrix2() const;
		static Matrix2 ToMatrix2(const Matrix4& m_);
		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Matrix4& m_);
		Matrix4x3 ToMatrix4x3() const;
		static Matrix4x3 ToMatrix4x3(const Matrix4& m_);

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

		constexpr float operator [](int i_) const; //This allows us to get elements
		constexpr float& operator [](int i_); //This allows us to set elements

		Matrix4x3 operator +(const Matrix4x3& m_) const;
		Matrix4x3 operator -(const Matrix4x3& m_) const;
		Matrix4x3 operator *(float s_) const;
		Vector3 operator *(const Vector3& v_) const;
		Vector4 operator *(const Vector4& v_) const;
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

		constexpr operator float* ();
		constexpr operator const float* () const;

		static Matrix4x3 Identity();

		static Matrix4x3 Translate(const Vector3& v_);
		static Matrix4x3 Rotate(float angle_, const Vector3& v_);
		static Matrix4x3 Scale(const Vector3& v_);

		Vector3 GetTranslation() const;
		//Quaternion GetRotation() const;
		Vector3 GetScale() const;

		Matrix2 ToMatrix2() const;
		static Matrix2 ToMatrix2(const Matrix4x3& m_);
		Matrix3 ToMatrix3() const;
		static Matrix3 ToMatrix3(const Matrix4x3& m_);
		Matrix4 ToMatrix4() const;
		static Matrix4 ToMatrix4(const Matrix4x3& m_);

		std::string ToString() const;

	private:
		static constexpr int mat4x3Size = 12;
		float m[mat4x3Size];
	};
}

#endif //!MATRIX_H