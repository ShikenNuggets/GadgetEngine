#include "Matrix.h"

#include "Math.h"

using namespace Gadget;

Matrix2::Matrix2() : m(){
	*this = Identity();
}

Matrix2::Matrix2(float x1_, float x2_, float y1_, float y2_){
	m[0] = x1_;
	m[1] = x2_;
	m[2] = y1_;
	m[3] = y2_;
}

Matrix2::Matrix2(float fill_) : m(){
	for(int i = 0; i < mat2Size; i++){
		m[i] = fill_;
	}
}

Matrix2::Matrix2(const Matrix3& m_){
	//These numbers look wrong. This is intentional.
	this->m[0] = m_[0]; this->m[2] = m_[2];
	this->m[1] = m_[1]; this->m[3] = m_[3];
}

Matrix2::Matrix2(const Matrix4& m_){
	//These numbers look wrong. This is intentional.
	this->m[0] = m_[0]; this->m[2] = m_[4];
	this->m[1] = m_[1]; this->m[3] = m_[5];
}

constexpr float Matrix2::operator [](int i_) const{
	_ASSERT(i_ > 0 || i_ < mat2Size);
	return m[i_];
}

constexpr float& Matrix2::operator [](int i_){
	_ASSERT(i_ > 0 || i_ < mat2Size);
	return m[i_];
}

Matrix2 Matrix2::operator +(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	for(int i = 0; i < mat2Size; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix2 Matrix2::operator -(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	for(unsigned int i = 0; i < mat2Size; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix2 Matrix2::operator *(float s_) const{
	return Matrix2(m[0] * s_, m[1] * s_, m[2] * s_, m[3] * s_);
}

Matrix2 Matrix2::operator *(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	result[0] = (m[0] * m_[0]) + (m[2] * m_[1]);
	result[2] = (m[0] * m_[2]) + (m[2] * m_[3]);
	
	result[1] = (m[1] * m_[0]) + (m[3] * m_[1]);
	result[3] = (m[1] * m_[2]) + (m[3] * m_[3]);
	
	return result;
}

Matrix2 Matrix2::operator /(const float s) const{
	return Matrix2(
		Math::SafeDivide(m[0], s),
		Math::SafeDivide(m[1], s),
		Math::SafeDivide(m[2], s),
		Math::SafeDivide(m[3], s)
	);
}

void Matrix2::operator +=(const Matrix2& m_){ *this = *this + m_; }
void Matrix2::operator -=(const Matrix2& m_){ *this = *this - m_; }
void Matrix2::operator *=(float s_){ *this = *this * s_; }
void Matrix2::operator *=(const Matrix2& m_){ *this = *this * m_; }
void Matrix2::operator /=(float s_){ *this = *this / s_; }

constexpr Matrix2::operator float*(){ return static_cast<float*>(&m[0]); }
constexpr Matrix2::operator const float*() const{ return static_cast<const float*>(&m[0]); }

Matrix2 Matrix2::Identity(){
	return Matrix2(	1.0f, 0.0f, 
					0.0f, 1.0f);
}

Matrix2 Matrix2::Transpose() const{
	return Matrix2(	m[0], m[2],
					m[1], m[3]);
}

Matrix2 Matrix2::Transpose(const Matrix2& m_){
	return m_.Transpose();
}

Matrix2 Matrix2::Inverse() const{
	float det = m[0] * m[3] - m[2] * m[1];

	float invdet = Math::SafeDivide(1.0f, det);

	return Matrix2(	m[3] * invdet, -m[1] * invdet,
					-m[2] * invdet, m[0] * invdet);
}

Matrix2 Matrix2::Inverse(const Matrix2& m_){
	return m_.Inverse();
}

Matrix3 Matrix2::ToMatrix3() const{
	Matrix3 m_ = Matrix3::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[3] = m[2];
	m_[4] = m[3];
	
	return m_;
}

Matrix3 Matrix2::ToMatrix3(const Matrix2& m_){
	return m_.ToMatrix3();
}

Matrix4 Matrix2::ToMatrix4() const{
	Matrix4 m_ = Matrix4::Identity();

	m_[0] = m[0];
	m_[1] = m[1];
	m_[4] = m[2];
	m_[5] = m[3];

	return m_;
}

Matrix4 Matrix2::ToMatrix4(const Matrix2& m_){
	return m_.ToMatrix4();
}

std::string Matrix2::ToString() const{
	std::string mString;

	mString =	std::to_string(m[0]) + ", " + std::to_string(m[2]) + ",\n" +
				std::to_string(m[1]) + ", " + std::to_string(m[3]);

	return mString;
}