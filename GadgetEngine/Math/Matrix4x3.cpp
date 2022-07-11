#include "Matrix.h"

#include "Quaternion.h"

using namespace Gadget;

constexpr Matrix4x3::Matrix4x3() : m(){
	*this = Identity();
}

constexpr Matrix4x3::Matrix4x3(float x1_, float x2_, float x3_,
				float y1_, float y2_, float y3_,
				float z1_, float z2_, float z3_,
				float w1_, float w2_, float w3_) : m(){
	m[0] = x1_; m[3] = y1_; m[6] = z1_; m[9] = w1_;
	m[1] = x2_; m[4] = y2_; m[7] = z2_; m[10] = w2_;
	m[2] = x3_; m[5] = y3_; m[8] = z3_; m[11] = w3_;
}

constexpr Matrix4x3::Matrix4x3(float fill_) : m(){
	for(int i = 0; i < mat4x3Size; i++){
		m[i] = fill_;
	}
}

constexpr Matrix4x3::Matrix4x3(const Matrix2& m_) : m(){
	m[0] = m_[0];	m[3] = m_[2];	m[6] = 0.0f;	m[9] = 0.0f;
	m[1] = m_[1];	m[4] = m_[3];	m[7] = 0.0f;	m[10] = 0.0f;
	m[2] = 0.0f;	m[5] = 0.0f;	m[8] = 1.0f;	m[11] = 0.0f;
}

constexpr Matrix4x3::Matrix4x3(const Matrix3& m_) : m(){
	m[0] = m_[0];	m[3] = m_[3];	m[6] = m_[6];	m[9] = 0.0f;
	m[1] = m_[1];	m[4] = m_[4];	m[7] = m_[7];	m[10] = 0.0f;
	m[2] = m_[2];	m[5] = m_[5];	m[8] = m_[8];	m[11] = 0.0f;
}

constexpr Matrix4x3::Matrix4x3(const Matrix4& m_) : m(){
	m[0] = m_[0];	m[3] = m_[4];	m[6] = m_[8];	m[9] = m_[12];
	m[1] = m_[1];	m[4] = m_[5];	m[7] = m_[9];	m[10] = m_[13];
	m[2] = m_[2];	m[5] = m_[6];	m[8] = m_[10];	m[11] = m_[14];
}

constexpr float Matrix4x3::operator [](unsigned int i_) const{
	_ASSERT(i_ < mat4x3Size); //TODO - Custom assert macro
	return m[i_];
}

constexpr float& Matrix4x3::operator [](unsigned int i_){
	_ASSERT(i_ < mat4x3Size); //TODO - Custom assert macro
	return m[i_];
}

constexpr Matrix4x3 Matrix4x3::operator +(const Matrix4x3& m_) const{
	Matrix4x3 result = Matrix4x3(0.0f);
	for(unsigned int i = 0; i < mat4x3Size; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

constexpr Matrix4x3 Matrix4x3::operator -(const Matrix4x3& m_) const{
	Matrix4x3 result = Matrix4x3(0.0f);
	for(unsigned int i = 0; i < mat4x3Size; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

constexpr Matrix4x3 Matrix4x3::operator *(float s_) const{
	return Matrix4x3(m[0] * s_, m[1] * s_, m[2] * s_, m[3] * s_, m[4] * s_, m[5] * s_, m[6] * s_, m[7] * s_, m[8] * s_, m[9] * s_, m[10] * s_, m[11] * s_);
}

constexpr Vector3 Matrix4x3::operator *(const Vector3& v_) const{
	return Vector3(
		Math::Dot4D(/*A*/ m[0], m[3], m[6], m[9], /*B*/ v_.x, v_.y, v_.z, 1.0f),
		Math::Dot4D(/*A*/ m[1], m[4], m[7], m[10], /*B*/ v_.x, v_.y, v_.z, 1.0f),
		Math::Dot4D(/*A*/ m[2], m[5], m[8], m[11], /*B*/ v_.x, v_.y, v_.z, 1.0f)
	);
}

constexpr Matrix4x3 Matrix4x3::operator /(const float s) const{
	return Matrix4x3(
		Math::SafeDivide(m[0], s),
		Math::SafeDivide(m[1], s),
		Math::SafeDivide(m[2], s),
		Math::SafeDivide(m[3], s),
		Math::SafeDivide(m[4], s),
		Math::SafeDivide(m[5], s),
		Math::SafeDivide(m[6], s),
		Math::SafeDivide(m[7], s),
		Math::SafeDivide(m[8], s),
		Math::SafeDivide(m[9], s),
		Math::SafeDivide(m[10], s),
		Math::SafeDivide(m[11], s)
	);
}

constexpr void Matrix4x3::operator +=(const Matrix4x3& m_){ *this = *this + m_; }
constexpr void Matrix4x3::operator -=(const Matrix4x3& m_){ *this = *this - m_; }
constexpr void Matrix4x3::operator *=(float s_){ *this = *this * s_; }
constexpr void Matrix4x3::operator /=(float s_){ *this = *this / s_; }

constexpr Matrix4x3::operator float*(){ return static_cast<float*>(&m[0]); }
constexpr Matrix4x3::operator const float*() const{ return static_cast<const float*>(&m[0]); }

Matrix4x3 Matrix4x3::Rotate(float angle_, const Vector3& v_){
	//TODO - This is convenient but not particularly efficient
	return Matrix4::Rotate(angle_, v_).ToMatrix4x3();
}

constexpr Vector3 Matrix4x3::GetTranslation() const{
	return Vector3(m[9], m[10], m[11]);
}

Quaternion Matrix4x3::GetRotation() const{
	//TODO - This is convenient but not particularly efficient
	return ToMatrix4().GetRotation();
}

constexpr Vector3 Matrix4x3::GetScale() const{
	return Vector3(m[0], m[4], m[8]);
}

constexpr Matrix2 Matrix4x3::ToMatrix2() const{
	return Matrix2(*this);
}

constexpr Matrix3 Matrix4x3::ToMatrix3() const{
	return Matrix3(*this);
}

constexpr Matrix4 Matrix4x3::ToMatrix4() const{
	return Matrix4(*this);
}

Euler Matrix4x3::ToEuler() const{
	return Matrix3(*this).ToEuler();
}

Quaternion Matrix4x3::ToQuaternion() const{
	return Matrix3(*this).ToQuaternion();
}

std::string Matrix4x3::ToString() const{
	return	std::to_string(m[0]) + ", " + std::to_string(m[3]) + ", " + std::to_string(m[6]) + ", " + std::to_string(m[9]) + ",\n" +
			std::to_string(m[1]) + ", " + std::to_string(m[4]) + ", " + std::to_string(m[7]) + ", " + std::to_string(m[10]) + ",\n" +
			std::to_string(m[2]) + ", " + std::to_string(m[5]) + ", " + std::to_string(m[8]) + ", " + std::to_string(m[11]);
}