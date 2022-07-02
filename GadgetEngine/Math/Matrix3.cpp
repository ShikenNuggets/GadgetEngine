#include "Matrix.h"

using namespace Gadget;

Matrix3::Matrix3() : m(){
	*this = Identity();
}

Matrix3::Matrix3(	float x1_, float x2_, float x3_,
					float y1_, float y2_, float y3_,
					float z1_, float z2_, float z3_){
	m[0] = x1_; m[3] = y1_; m[6] = z1_;
	m[1] = x2_; m[4] = y2_; m[7] = z2_;
	m[2] = x3_; m[5] = y3_; m[8] = z3_;
}

Matrix3::Matrix3(const float fillValue) : m(){
	for(int i = 0; i < mat3Size; i++){
		m[i] = fillValue;
	}
}


Matrix3::Matrix3(const Matrix2& m_){
	this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = 0.0f;
	this->m[3] = m_[2]; this->m[4] = m_[3]; this->m[5] = 0.0f;
	this->m[6] = 0.0f;  this->m[7] = 0.0f;	this->m[8] = 1.0f;
}

Matrix3::Matrix3(const Matrix4& m_){
	this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
	this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
	this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
}

Matrix3& Matrix3::operator =(const Matrix4& m_){
	this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
	this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
	this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
	return *this;
}

constexpr float Matrix3::operator [](int i_) const{
	_ASSERT(i_ > 0 || i_ < mat3Size);
	return m[i_];
}

constexpr float& Matrix3::operator [](int i_){
	_ASSERT(i_ > 0 || i_ < mat3Size);
	return m[i_];
}

Matrix3 Matrix3::operator +(const Matrix3& m_) const{
	Matrix3 result = Matrix3(0.0f);
	for(int i = 0; i < mat3Size; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix3 Matrix3::operator -(const Matrix3& m_) const{
	Matrix3 result = Matrix3(0.0f);
	for(unsigned int i = 0; i < mat3Size; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix3 Matrix3::operator *(float s_) const{
	return Matrix3(	m[0] * s_, m[1] * s_, m[2] * s_,
					m[3] * s_, m[4] * s_, m[5] * s_,
					m[6] * s_, m[7] * s_, m[8] * s_);
}

Matrix3 Matrix3::operator *(const Matrix3& m_) const{
	Matrix3 result = Matrix3(0.0f);
	result[0] = (m[0] * m_[0]) + (m[3] * m_[1]) + (m[6] * m_[2]);
	result[3] = (m[0] * m_[3]) + (m[3] * m_[4]) + (m[6] * m_[6]);
	result[6] = (m[0] * m_[6]) + (m[3] * m_[7]) + (m[6] * m_[8]);

	result[1] = (m[1] * m_[0]) + (m[4] * m_[1]) + (m[7] * m_[2]);
	result[4] = (m[1] * m_[3]) + (m[4] * m_[4]) + (m[7] * m_[6]);
	result[7] = (m[1] * m_[6]) + (m[4] * m_[7]) + (m[7] * m_[8]);

	result[2] = (m[2] * m_[0]) + (m[5] * m_[1]) + (m[8] * m_[2]);
	result[5] = (m[2] * m_[3]) + (m[5] * m_[4]) + (m[8] * m_[6]);
	result[8] = (m[2] * m_[6]) + (m[5] * m_[7]) + (m[8] * m_[8]);

	return result;
}

Matrix3 Matrix3::operator /(float s_) const{
	return Matrix3(	Math::SafeDivide(m[0], s_), Math::SafeDivide(m[1], s_), Math::SafeDivide(m[2], s_),
					Math::SafeDivide(m[3], s_), Math::SafeDivide(m[4], s_), Math::SafeDivide(m[5], s_),
					Math::SafeDivide(m[6], s_), Math::SafeDivide(m[7], s_), Math::SafeDivide(m[8], s_));
}

void Matrix3::operator +=(const Matrix3& m_){ *this = *this + m_; }
void Matrix3::operator -=(const Matrix3& m_){ *this = *this - m_; }
void Matrix3::operator *=(float s_){ *this = *this * s_; }
void Matrix3::operator *=(const Matrix3& m_){ *this = *this * m_; }
void Matrix3::operator /=(float s_){ *this = *this / s_; }

constexpr Matrix3::operator float*(){ return static_cast<float*>(&m[0]); }

constexpr Matrix3::operator const float*() const{ return static_cast<const float*>(&m[0]); }

Matrix3 Matrix3::Identity(){
	return Matrix3(	1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 1.0f);
}

Matrix3 Matrix3::Transpose() const{
	return Matrix3(	m[0], m[3], m[6],
					m[1], m[4], m[7],
					m[2], m[5], m[8]);
}

Matrix3 Matrix3::Transpose(const Matrix3& m_){
	return m_.Transpose();
}

Matrix3 Matrix3::Inverse() const{
	float det =	m[0] * (m[4] * m[8] - m[7] * m[5]) -
				m[1] * (m[3] * m[8] - m[5] * m[6]) +
				m[2] * (m[3] * m[7] - m[4] * m[6]);

	float invdet = Math::SafeDivide(1.0f, det);

	Matrix3 inverse = Matrix3::Identity();

	inverse[0] = (m[4] * m[8] - m[7] * m[5]) * invdet;
	inverse[1] = (m[2] * m[7] - m[1] * m[8]) * invdet;
	inverse[2] = (m[1] * m[5] - m[2] * m[4]) * invdet;
	inverse[3] = (m[5] * m[6] - m[3] * m[8]) * invdet;
	inverse[4] = (m[0] * m[8] - m[2] * m[6]) * invdet;
	inverse[5] = (m[3] * m[2] - m[0] * m[5]) * invdet;
	inverse[6] = (m[3] * m[7] - m[6] * m[4]) * invdet;
	inverse[7] = (m[6] * m[1] - m[0] * m[7]) * invdet;
	inverse[8] = (m[0] * m[4] - m[3] * m[1]) * invdet;

	return inverse;
}

Matrix3 Matrix3::Inverse(const Matrix3& m_){
	return m_.Inverse();
}

Matrix2 Matrix3::ToMatrix2() const{
	Matrix2 m_ = Matrix2::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[2] = m[3];
	m_[3] = m[4];

	return m_;
}

Matrix2 Matrix3::ToMatrix2(const Matrix3& m_){
	return m_.ToMatrix2();
}

Matrix4 Matrix3::ToMatrix4() const{
	Matrix4 m_ = Matrix4::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[2] = m[2];
	m_[4] = m[3];
	m_[5] = m[4];
	m_[6] = m[5];
	m_[8] = m[6];
	m_[9] = m[7];
	m_[10] = m[8];

	return m_;
}

Matrix4 Matrix3::ToMatrix4(const Matrix3& m_){
	return m_.ToMatrix4();
}

std::string Matrix3::ToString() const{
	std::string mString;

	mString =	std::to_string(m[0]) + ", " + std::to_string(m[3]) + ", " + std::to_string(m[6]) + ",\n" +
				std::to_string(m[1]) + ", " + std::to_string(m[4]) + ", " + std::to_string(m[7]) + ",\n" +
				std::to_string(m[2]) + ", " + std::to_string(m[5]) + ", " + std::to_string(m[8]);

	return mString;
}