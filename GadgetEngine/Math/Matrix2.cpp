#include "Matrix.h"

#include "Math/Math.h"

using namespace Gadget;

Matrix2::Matrix2() : m(){
	*this = Identity();
}

Matrix2::Matrix2(float x1_, float x2_, float y1_, float y2_) : m(){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x1_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x2_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y1_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y2_));

	m[0] = x1_;
	m[1] = x2_;
	m[2] = y1_;
	m[3] = y2_;
}

Matrix2::Matrix2(float fill_) : m(){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(fill_));

	for(auto& i : m){
		i = fill_;
	}
}

Matrix2::Matrix2(const Matrix3& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[2] = m_[3];
	m[1] = m_[1];	m[3] = m_[4];
}

Matrix2::Matrix2(const Matrix4& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[2] = m_[4];
	m[1] = m_[1];	m[3] = m_[5];
}

Matrix2::Matrix2(const Matrix4x3& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[2] = m_[3];
	m[1] = m_[1];	m[3] = m_[4];
}

float& Matrix2::operator [](unsigned int i_){
	GADGET_ASSERT(i_ < mat2Size, "Invalid array access! " + std::to_string(i_) + " must be less than " + std::to_string(mat2Size) + "!");
	return m[i_];
}

Matrix2 Matrix2::operator +(const Matrix2& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	Matrix2 result = Matrix2(0.0f);
	for(unsigned int i = 0; i < mat2Size; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix2 Matrix2::operator -(const Matrix2& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	Matrix2 result = Matrix2(0.0f);
	for(unsigned int i = 0; i < mat2Size; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix2 Matrix2::operator *(float s_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(s_));

	return Matrix2(m[0] * s_, m[1] * s_, m[2] * s_, m[3] * s_);
}

Matrix2 Matrix2::operator *(const Matrix2& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	return Matrix2(
		//COLUMN 1
		Math::Dot2D(/*A*/ m[0], m[2], /*B*/ m_[0], m_[1]),
		Math::Dot2D(/*A*/ m[1], m[3], /*B*/ m_[0], m_[1]),
		//COLUMN 2
		Math::Dot2D(/*A*/ m[0], m[2], /*B*/ m_[2], m_[3]),
		Math::Dot2D(/*A*/ m[1], m[3], /*B*/ m_[2], m_[3])
	);
}

Matrix2 Matrix2::operator /(const float s_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(s_));

	return Matrix2(
		Math::SafeDivide(m[0], s_),
		Math::SafeDivide(m[1], s_),
		Math::SafeDivide(m[2], s_),
		Math::SafeDivide(m[3], s_)
	);
}

void Matrix2::operator +=(const Matrix2& m_){ *this = *this + m_; }
void Matrix2::operator -=(const Matrix2& m_){ *this = *this - m_; }
void Matrix2::operator *=(float s_){ *this = *this * s_; }
void Matrix2::operator *=(const Matrix2& m_){ *this = *this * m_; }
void Matrix2::operator /=(float s_){ *this = *this / s_; }

Matrix2::operator float*(){ return static_cast<float*>(&m[0]); }
Matrix2::operator const float*() const{ return static_cast<const float*>(&m[0]); }

Matrix2 Matrix2::Transpose() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix2(	m[0], m[2],
					m[1], m[3]);
}

float Matrix2::Determinant() const{
	GADGET_BASIC_ASSERT(IsValid());
	return (m[0] * m[3]) - (m[2] * m[1]);
}

Matrix2 Matrix2::Inverse() const{
	GADGET_BASIC_ASSERT(IsValid());
	const float invdet = Math::SafeDivide(1.0f, Determinant());

	return Matrix2(	m[3] * invdet, -m[1] * invdet,
					-m[2] * invdet, m[0] * invdet);
}

Matrix3 Matrix2::ToMatrix3() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix3(*this);
}

Matrix4 Matrix2::ToMatrix4() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix4(*this);
}

Matrix4x3 Matrix2::ToMatrix4x3() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix4x3(*this);
}

std::string Matrix2::ToString() const{
	return	std::to_string(m[0]) + ", " + std::to_string(m[2]) + ",\n" +
			std::to_string(m[1]) + ", " + std::to_string(m[3]);
}