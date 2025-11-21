#include "Matrix.h"

#include "Math/Euler.h"
#include "Math/Quaternion.h"

using namespace Gadget;

Matrix3::Matrix3() : m(){
	*this = Identity();
}

Matrix3::Matrix3(	float x1_, float x2_, float x3_,
					float y1_, float y2_, float y3_,
					float z1_, float z2_, float z3_) : m(){
	GADGET_BASIC_ASSERT(x1_);
	GADGET_BASIC_ASSERT(x2_);
	GADGET_BASIC_ASSERT(x3_);
	GADGET_BASIC_ASSERT(y1_);
	GADGET_BASIC_ASSERT(y2_);
	GADGET_BASIC_ASSERT(y3_);
	GADGET_BASIC_ASSERT(z1_);
	GADGET_BASIC_ASSERT(z2_);
	GADGET_BASIC_ASSERT(z3_);

	m[0] = x1_; m[3] = y1_; m[6] = z1_;
	m[1] = x2_; m[4] = y2_; m[7] = z2_;
	m[2] = x3_; m[5] = y3_; m[8] = z3_;
}

Matrix3::Matrix3(const float fill_) : m(){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(fill_));

	for(auto& i : m){
		i = fill_;
	}
}

Matrix3::Matrix3(const Matrix2& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[1] = m_[1];	m[2] = 0.0f;
	m[3] = m_[2];	m[4] = m_[3];	m[5] = 0.0f;
	m[6] = 0.0f;	m[7] = 0.0f;	m[8] = 1.0f;
}

Matrix3::Matrix3(const Matrix4& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[1] = m_[1];	m[2] = m_[2];
	m[3] = m_[4];	m[4] = m_[5];	m[5] = m_[6];
	m[6] = m_[8];	m[7] = m_[9];	m[8] = m_[10];
}

Matrix3::Matrix3(const Matrix4x3& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[1] = m_[1];	m[2] = m_[2];
	m[3] = m_[3];	m[4] = m_[4];	m[5] = m_[5];
	m[6] = m_[6];	m[7] = m_[7];	m[8] = m_[8];
}

Matrix3& Matrix3::operator =(const Matrix4& m_){
	GADGET_BASIC_ASSERT(m_.IsValid());

	this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
	this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
	this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
	return *this;
}

float& Matrix3::operator [](unsigned int i_){
	GADGET_ASSERT(i_ < mat3Size, "Invalid array access! " + std::to_string(i_) + " must be less than " + std::to_string(mat3Size) + "!");
	return m[i_];
}

Matrix3 Matrix3::operator +(const Matrix3& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	Matrix3 result = Matrix3(0.0f);
	for(unsigned int i = 0; i < mat3Size; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix3 Matrix3::operator -(const Matrix3& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	Matrix3 result = Matrix3(0.0f);
	for(unsigned int i = 0; i < mat3Size; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix3 Matrix3::operator *(float s_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(s_));

	return Matrix3(	m[0] * s_, m[1] * s_, m[2] * s_,
					m[3] * s_, m[4] * s_, m[5] * s_,
					m[6] * s_, m[7] * s_, m[8] * s_);
}

Matrix3 Matrix3::operator *(const Matrix3& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	return Matrix3(
		//COLUMN 1
		Math::Dot3D(/*A*/ m[0], m[3], m[6], /*B*/ m_[0], m_[1], m_[2]),
		Math::Dot3D(/*A*/ m[1], m[4], m[7], /*B*/ m_[0], m_[1], m_[2]),
		Math::Dot3D(/*A*/ m[2], m[5], m[8], /*B*/ m_[0], m_[1], m_[2]),
		//COLUMN 2
		Math::Dot3D(/*A*/ m[0], m[3], m[6], /*B*/ m_[3], m_[4], m_[5]),
		Math::Dot3D(/*A*/ m[1], m[4], m[7], /*B*/ m_[3], m_[4], m_[5]),
		Math::Dot3D(/*A*/ m[2], m[5], m[8], /*B*/ m_[3], m_[4], m_[5]),
		//COLUMN 3
		Math::Dot3D(/*A*/ m[0], m[3], m[6], /*B*/ m_[6], m_[7], m_[8]),
		Math::Dot3D(/*A*/ m[1], m[4], m[7], /*B*/ m_[6], m_[7], m_[8]),
		Math::Dot3D(/*A*/ m[2], m[5], m[8], /*B*/ m_[6], m_[7], m_[8])
	);
}

Matrix3 Matrix3::operator /(float s_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(s_));

	return Matrix3(	Math::SafeDivide(m[0], s_), Math::SafeDivide(m[1], s_), Math::SafeDivide(m[2], s_),
					Math::SafeDivide(m[3], s_), Math::SafeDivide(m[4], s_), Math::SafeDivide(m[5], s_),
					Math::SafeDivide(m[6], s_), Math::SafeDivide(m[7], s_), Math::SafeDivide(m[8], s_));
}

void Matrix3::operator +=(const Matrix3& m_){ *this = *this + m_; }
void Matrix3::operator -=(const Matrix3& m_){ *this = *this - m_; }
void Matrix3::operator *=(float s_){ *this = *this * s_; }
void Matrix3::operator *=(const Matrix3& m_){ *this = *this * m_; }
void Matrix3::operator /=(float s_){ *this = *this / s_; }

Matrix3::operator float*(){ return static_cast<float*>(&m[0]); }

Matrix3::operator const float*() const{ return static_cast<const float*>(&m[0]); }

Matrix3 Matrix3::Transpose() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix3(	m[0], m[3], m[6],
					m[1], m[4], m[7],
					m[2], m[5], m[8]);
}

float Matrix3::Determinant() const{
	GADGET_BASIC_ASSERT(IsValid());
	return	m[0] * (m[4] * m[8] - m[7] * m[5]) -
			m[1] * (m[3] * m[8] - m[5] * m[6]) +
			m[2] * (m[3] * m[7] - m[4] * m[6]);
}

Matrix3 Matrix3::Inverse() const{
	GADGET_BASIC_ASSERT(IsValid());

	const float invdet = Math::SafeDivide(1.0f, Determinant());

	return Matrix3(
		(m[4] * m[8] - m[7] * m[5]) * invdet,
		(m[2] * m[7] - m[1] * m[8]) * invdet,
		(m[1] * m[5] - m[2] * m[4]) * invdet,
		(m[5] * m[6] - m[3] * m[8]) * invdet,
		(m[0] * m[8] - m[2] * m[6]) * invdet,
		(m[3] * m[2] - m[0] * m[5]) * invdet,
		(m[3] * m[7] - m[6] * m[4]) * invdet,
		(m[6] * m[1] - m[0] * m[7]) * invdet,
		(m[0] * m[4] - m[3] * m[1]) * invdet
	);
}

Matrix2 Matrix3::ToMatrix2() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix2(*this);
}

Matrix4 Matrix3::ToMatrix4() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix4(*this);
}

Matrix4x3 Matrix3::ToMatrix4x3() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix4x3(*this);
}

Euler Matrix3::ToEuler() const{
	GADGET_BASIC_ASSERT(IsValid());

	Angle heading = Angle(0.0f);
	Angle attitude = Math::Asin(m[1]);
	Angle bank = Angle(0.0f);

	if(m[1] >= 0.99f || m[1] <= -0.99f){
		heading = Math::Atan2(m[6], m[8]);
	}else{
		heading = Math::Atan2(-m[2], m[0]);
		attitude = Math::Asin(m[1]);
		bank = Math::Atan2(-m[7], m[4]);
	}

	return Euler(bank, heading, attitude);
}

Quaternion Matrix3::ToQuaternion() const{
	GADGET_BASIC_ASSERT(IsValid());

	//TODO - This feels inefficient
	Quaternion q = Quaternion(
		Math::Sqrt(std::max(0.0f, 1.0f + m[0] + m[4] + m[8])) / 2.0f,
		Math::Sqrt(std::max(0.0f, 1.0f + m[0] - m[4] - m[8])) / 2.0f,
		Math::Sqrt(std::max(0.0f, 1.0f - m[0] + m[4] - m[8])) / 2.0f,
		Math::Sqrt(std::max(0.0f, 1.0f - m[0] - m[4] + m[8])) / 2.0f
	);

	if(m[5] - m[7] != 0.0f){
		q.x = std::copysign(q.x, m[5] - m[7]);
	}else{
		q.x = 0.0f;
	}

	if(m[6] - m[2] != 0.0f){
		q.y = std::copysign(q.y, m[6] - m[2]);
	}else{
		q.y = 0.0f;
	}

	if(m[1] - m[3] != 0.0f){
		q.z = std::copysign(q.z, m[1] - m[3]);
	}else{
		q.z = 0.0f;
	}

	return Quaternion(q.w, q.x, q.y, q.z).Normalized();
}

std::string Matrix3::ToString() const{
	return	std::to_string(m[0]) + ", " + std::to_string(m[3]) + ", " + std::to_string(m[6]) + ",\n" +
			std::to_string(m[1]) + ", " + std::to_string(m[4]) + ", " + std::to_string(m[7]) + ",\n" +
			std::to_string(m[2]) + ", " + std::to_string(m[5]) + ", " + std::to_string(m[8]);
}