#include "Matrix.h"

#include "Math.h"

using namespace Gadget;

constexpr Matrix2::Matrix2() : m(){
	*this = Identity();
}

constexpr Matrix2::Matrix2(float x1_, float x2_, float y1_, float y2_) : m(){
	m[0] = x1_;
	m[1] = x2_;
	m[2] = y1_;
	m[3] = y2_;
}

constexpr Matrix2::Matrix2(float fill_) : m(){
	for(int i = 0; i < mat2Size; i++){
		m[i] = fill_;
	}
}

constexpr Matrix2::Matrix2(const Matrix3& m_) : m(){
	m[0] = m_[0];	m[2] = m_[3];
	m[1] = m_[1];	m[3] = m_[4];
}

constexpr Matrix2::Matrix2(const Matrix4& m_) : m(){
	m[0] = m_[0];	m[2] = m_[4];
	m[1] = m_[1];	m[3] = m_[5];
}

constexpr Matrix2::Matrix2(const Matrix4x3& m_) : m(){
	m[0] = m_[0];	m[2] = m_[3];
	m[1] = m_[1];	m[3] = m_[4];
}

constexpr float Matrix2::operator [](unsigned int i_) const{
	_ASSERT(i_ < mat2Size);
	return m[i_];
}

constexpr float& Matrix2::operator [](unsigned int i_){
	_ASSERT(i_ < mat2Size);
	return m[i_];
}

constexpr Matrix2 Matrix2::operator +(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	for(unsigned int i = 0; i < mat2Size; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

constexpr Matrix2 Matrix2::operator -(const Matrix2& m_) const{
	Matrix2 result = Matrix2(0.0f);
	for(unsigned int i = 0; i < mat2Size; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

constexpr Matrix2 Matrix2::operator *(float s_) const{
	return Matrix2(m[0] * s_, m[1] * s_, m[2] * s_, m[3] * s_);
}

constexpr Matrix2 Matrix2::operator *(const Matrix2& m_) const{
	return Matrix2(
		//COLUMN 1
		Math::Dot2D(/*A*/ m[0], m[2], /*B*/ m_[0], m_[1]),
		Math::Dot2D(/*A*/ m[1], m[3], /*B*/ m_[0], m_[1]),
		//COLUMN 2
		Math::Dot2D(/*A*/ m[0], m[2], /*B*/ m_[2], m_[3]),
		Math::Dot2D(/*A*/ m[1], m[3], /*B*/ m_[2], m_[3])
	);
}

constexpr Matrix2 Matrix2::operator /(const float s) const{
	return Matrix2(
		Math::SafeDivide(m[0], s),
		Math::SafeDivide(m[1], s),
		Math::SafeDivide(m[2], s),
		Math::SafeDivide(m[3], s)
	);
}

constexpr void Matrix2::operator +=(const Matrix2& m_){ *this = *this + m_; }
constexpr void Matrix2::operator -=(const Matrix2& m_){ *this = *this - m_; }
constexpr void Matrix2::operator *=(float s_){ *this = *this * s_; }
constexpr void Matrix2::operator *=(const Matrix2& m_){ *this = *this * m_; }
constexpr void Matrix2::operator /=(float s_){ *this = *this / s_; }

constexpr Matrix2::operator float*(){ return static_cast<float*>(&m[0]); }
constexpr Matrix2::operator const float*() const{ return static_cast<const float*>(&m[0]); }

constexpr Matrix2 Matrix2::Transpose() const{
	return Matrix2(	m[0], m[2],
					m[1], m[3]);
}

constexpr float Matrix2::Determinant() const{
	return (m[0] * m[3]) - (m[2] * m[1]);
}

constexpr Matrix2 Matrix2::Inverse() const{
	float invdet = Math::SafeDivide(1.0f, Determinant());

	return Matrix2(	m[3] * invdet, -m[1] * invdet,
					-m[2] * invdet, m[0] * invdet);
}

constexpr Matrix3 Matrix2::ToMatrix3() const{
	return Matrix3(*this);
}

constexpr Matrix4 Matrix2::ToMatrix4() const{
	return Matrix4(*this);
}

constexpr Matrix4x3 Matrix2::ToMatrix4x3() const{
	return Matrix4x3(*this);
}

std::string Matrix2::ToString() const{
	return	std::to_string(m[0]) + ", " + std::to_string(m[2]) + ",\n" +
			std::to_string(m[1]) + ", " + std::to_string(m[3]);
}