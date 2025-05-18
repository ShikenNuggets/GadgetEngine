#include "Matrix.h"

#include "Math/Euler.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"

using namespace Gadget;

Matrix4::Matrix4() : m(){
	*this = Identity();
}

Matrix4::Matrix4(	float x1_, float x2_, float x3_, float x4_,
					float y1_, float y2_, float y3_, float y4_,
					float z1_, float z2_, float z3_, float z4_,
					float w1_, float w2_, float w3_, float w4_) : m(){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x1_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x2_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x3_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(x4_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y1_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y2_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y3_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(y4_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z1_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z2_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z3_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(z4_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(w1_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(w2_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(w3_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(w4_));

	m[0] = x1_; m[4] = y1_; m[8] = z1_;  m[12] = w1_;
	m[1] = x2_; m[5] = y2_; m[9] = z2_;  m[13] = w2_;
	m[2] = x3_; m[6] = y3_; m[10] = z3_; m[14] = w3_;
	m[3] = x4_; m[7] = y4_; m[11] = z4_; m[15] = w4_;
}

Matrix4::Matrix4(float fill_) : m(){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(fill_));

	for(auto& i : m){
		i = fill_;
	}
}

Matrix4::Matrix4(const Matrix2& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[4] = m_[2];	m[8] = 0.0f;	m[12] = 0.0f;
	m[1] = m_[1];	m[5] = m_[3];	m[9] = 0.0f;	m[13] = 0.0f;
	m[2] = 0.0f;	m[6] = 0.0f;	m[10] = 1.0f;	m[14] = 0.0f;
	m[3] = 0.0f;	m[7] = 0.0f;	m[11] = 0.0f;	m[15] = 1.0f;
}

Matrix4::Matrix4(const Matrix3& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[4] = m_[3];	m[8] = m_[6];	m[12] = 0.0f;
	m[1] = m_[1];	m[5] = m_[4];	m[9] = m_[7];	m[13] = 0.0f;
	m[2] = m_[2];	m[6] = m_[5];	m[10] = m_[8];	m[14] = 0.0f;
	m[3] = 0.0f;	m[7] = 0.0f;	m[11] = 0.0f;	m[15] = 1.0f;
}

Matrix4::Matrix4(const Matrix4x3& m_) : m(){
	GADGET_BASIC_ASSERT(m_.IsValid());

	m[0] = m_[0];	m[4] = m_[3];	m[8] = m_[6];	m[12] = m_[9];
	m[1] = m_[1];	m[5] = m_[4];	m[9] = m_[7];	m[13] = m_[10];
	m[2] = m_[2];	m[6] = m_[5];	m[10] = m_[8];	m[14] = m_[11];
	m[3] = 0.0f;	m[7] = 0.0f;	m[11] = 0.0f;	m[15] = 1.0f;
}

float& Matrix4::operator [](unsigned int i_){
	GADGET_SLOW_ASSERT(IsValid());
	GADGET_ASSERT(i_ < mat4Size, "Invalid array access! " + std::to_string(i_) + " must be less than " + std::to_string(mat4Size) + "!");
	return m[i_];
}

Matrix4 Matrix4::operator +(const Matrix4& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	Matrix4 result = Matrix4(0.0f);
	for(unsigned int i = 0; i < mat4Size; i++){
		result[i] = m[i] + m_[i];
	}

	return result;
}

Matrix4 Matrix4::operator -(const Matrix4& m_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(m_.IsValid());

	Matrix4 result = Matrix4(0.0f);
	for(unsigned int i = 0; i < mat4Size; i++){
		result[i] = m[i] - m_[i];
	}

	return result;
}

Matrix4 Matrix4::operator *(float s_) const{
	GADGET_SLOW_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(s_));

	return Matrix4(	m[0] * s_,	m[1] * s_,	m[2] * s_,	m[3] * s_,
					m[4] * s_,	m[5] * s_,	m[6] * s_,	m[7] * s_,
					m[8] * s_,	m[9] * s_,	m[10] * s_,	m[11] * s_,
					m[12] * s_,	m[13] * s_,	m[14] * s_,	m[15] * s_
	);
}

Matrix4 Matrix4::operator*(const Matrix4& m_) const{
	GADGET_SLOW_ASSERT(IsValid());
	GADGET_SLOW_ASSERT(m_.IsValid());

	return Matrix4(
		//COLUMN 1
		Math::Dot4D(/*A*/ m[0], m[4], m[8], m[12], /*B*/ m_[0], m_[1], m_[2], m_[3]),
		Math::Dot4D(/*A*/ m[1], m[5], m[9], m[13], /*B*/ m_[0], m_[1], m_[2], m_[3]),
		Math::Dot4D(/*A*/ m[2], m[6], m[10], m[14], /*B*/ m_[0], m_[1], m_[2], m_[3]),
		Math::Dot4D(/*A*/ m[3], m[7], m[11], m[15], /*B*/ m_[0], m_[1], m_[2], m_[3]),
		//COLUMN 2
		Math::Dot4D(/*A*/ m[0], m[4], m[8], m[12], /*B*/ m_[4], m_[5], m_[6], m_[7]),
		Math::Dot4D(/*A*/ m[1], m[5], m[9], m[13], /*B*/ m_[4], m_[5], m_[6], m_[7]),
		Math::Dot4D(/*A*/ m[2], m[6], m[10], m[14], /*B*/ m_[4], m_[5], m_[6], m_[7]),
		Math::Dot4D(/*A*/ m[3], m[7], m[11], m[15], /*B*/ m_[4], m_[5], m_[6], m_[7]),
		//COLUMN 3
		Math::Dot4D(/*A*/ m[0], m[4], m[8], m[12], /*B*/ m_[8], m_[9], m_[10], m_[11]),
		Math::Dot4D(/*A*/ m[1], m[5], m[9], m[13], /*B*/ m_[8], m_[9], m_[10], m_[11]),
		Math::Dot4D(/*A*/ m[2], m[6], m[10], m[14], /*B*/ m_[8], m_[9], m_[10], m_[11]),
		Math::Dot4D(/*A*/ m[3], m[7], m[11], m[15], /*B*/ m_[8], m_[9], m_[10], m_[11]),
		//COLUMN 4
		Math::Dot4D(/*A*/ m[0], m[4], m[8], m[12], /*B*/ m_[12], m_[13], m_[14], m_[15]),
		Math::Dot4D(/*A*/ m[1], m[5], m[9], m[13], /*B*/ m_[12], m_[13], m_[14], m_[15]),
		Math::Dot4D(/*A*/ m[2], m[6], m[10], m[14], /*B*/ m_[12], m_[13], m_[14], m_[15]),
		Math::Dot4D(/*A*/ m[3], m[7], m[11], m[15], /*B*/ m_[12], m_[13], m_[14], m_[15])
	);
}

Vector3 Matrix4::operator *(const Vector3& v_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(v_.IsValid());

	return Vector3(
		Math::Dot4D(/*A*/ m[0], m[4], m[8], m[12], /*B*/ v_.x, v_.y, v_.z, 1.0f),
		Math::Dot4D(/*A*/ m[1], m[5], m[9], m[13], /*B*/ v_.x, v_.y, v_.z, 1.0f),
		Math::Dot4D(/*A*/ m[2], m[6], m[10], m[14], /*B*/ v_.x, v_.y, v_.z, 1.0f)
	);
}

Vector4 Matrix4::operator *(const Vector4& v_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(v_.IsValid());

	return Vector4(
		Math::Dot4D(/*A*/ m[0], m[4], m[8], m[12], /*B*/ v_.x, v_.y, v_.z, v_.w),
		Math::Dot4D(/*A*/ m[1], m[5], m[9], m[13], /*B*/ v_.x, v_.y, v_.z, v_.w),
		Math::Dot4D(/*A*/ m[2], m[6], m[10], m[14], /*B*/ v_.x, v_.y, v_.z, v_.w),
		Math::Dot4D(/*A*/ m[3], m[7], m[11], m[15], /*B*/ v_.x, v_.y, v_.z, v_.w)
	);
}

Matrix4 Matrix4::operator /(float s_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(Math::IsValidNumber(s_));

	return Matrix4(	Math::SafeDivide(m[0], s_), Math::SafeDivide(m[1], s_), Math::SafeDivide(m[2], s_), Math::SafeDivide(m[3], s_),
					Math::SafeDivide(m[4], s_), Math::SafeDivide(m[5], s_), Math::SafeDivide(m[6], s_), Math::SafeDivide(m[7], s_),
					Math::SafeDivide(m[8], s_), Math::SafeDivide(m[9], s_), Math::SafeDivide(m[10], s_), Math::SafeDivide(m[11], s_),
					Math::SafeDivide(m[12], s_), Math::SafeDivide(m[13], s_), Math::SafeDivide(m[14], s_), Math::SafeDivide(m[15], s_)
	);
}

void Matrix4::operator +=(const Matrix4& m_){ *this = *this + m_; }
void Matrix4::operator -=(const Matrix4& m_){ *this = *this - m_; }
void Matrix4::operator *=(float s_){ *this = *this * s_; }
void Matrix4::operator *=(const Matrix4& m_){ *this = *this * m_; }
void Matrix4::operator /=(float s_){ *this = *this / s_; }

Matrix4 Matrix4::Transpose() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix4(	m[0], m[4], m[8], m[12],
					m[1], m[5], m[9], m[13],
					m[2], m[6], m[10], m[14],
					m[3], m[7], m[11], m[15]);

}

Matrix4 Matrix4::Inverse() const{
	GADGET_BASIC_ASSERT(IsValid());

	Matrix4 inverseM;
	inverseM[0] =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
	inverseM[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
	inverseM[2] =  m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
	inverseM[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
	inverseM[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
	inverseM[5] =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
	inverseM[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
	inverseM[7] =  m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
	inverseM[8] =  m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
	inverseM[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
	inverseM[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
	inverseM[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
	inverseM[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
	inverseM[13] =  m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
	inverseM[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
	inverseM[15] =  m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

	const float determinant = m[0] * inverseM[0] + m[1] * inverseM[4] + m[2] * inverseM[8] + m[3] * inverseM[12];
	inverseM /= determinant;

	return inverseM;
}

Matrix4 Matrix4::Rotate(float angle_, const Vector3& v_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(angle_));
	GADGET_BASIC_ASSERT(v_.IsValid());

	Vector3 rotAxis(v_.x, v_.y, v_.z);

	rotAxis.Normalize();
	const float tempAngle = Math::DegreesToRadians(angle_);
	const float cosang = cos(tempAngle);
	const float sinang = sin(tempAngle);
	const float cosm = (1.0f - cosang);

	Matrix4 m;

	m[0] = (rotAxis.x * rotAxis.x * cosm) + cosang;
	m[1] = (rotAxis.x * rotAxis.y * cosm) + (rotAxis.z * sinang);
	m[2] = (rotAxis.x * rotAxis.z * cosm) - (rotAxis.y * sinang);
	m[3] = 0.0;
	m[4] = (rotAxis.x * rotAxis.y * cosm) - (rotAxis.z * sinang);
	m[5] = (rotAxis.y * rotAxis.y * cosm) + cosang;
	m[6] = (rotAxis.y * rotAxis.z * cosm) + (rotAxis.x * sinang);
	m[7] = 0.0;
	m[8] = (rotAxis.x * rotAxis.z * cosm) + (rotAxis.y * sinang);
	m[9] = (rotAxis.y * rotAxis.z * cosm) - (rotAxis.x * sinang);
	m[10] = (rotAxis.z * rotAxis.z * cosm) + cosang;
	m[11] = 0.0;
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
	return m;
}

Vector3 Matrix4::GetTranslation() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Vector3(m[12], m[13], m[14]);
}

Quaternion Matrix4::GetRotation() const{
	GADGET_BASIC_ASSERT(IsValid());
	return ToQuaternion();
}

Vector3 Matrix4::GetScale() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Vector3(m[0], m[5], m[10]);
}

Matrix4 Matrix4::LookAt(float eyeX_, float eyeY_, float eyeZ_,
	float atX_, float atY_, float atZ_,
	float upX_, float upY_, float upZ_){

	GADGET_BASIC_ASSERT(Math::IsValidNumber(eyeX_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(eyeY_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(eyeZ_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(atX_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(atY_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(atZ_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(upX_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(upY_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(upZ_));

	const Vector3 at(atX_, atY_, atZ_);
	Vector3 up(upX_, upY_, upZ_);
	const Vector3 eye(eyeX_, eyeY_, eyeZ_);

	Matrix4 result;

	const Vector3 forward = (at - eye).Normalized();
	up.Normalize();
	const Vector3 side = Vector3::Cross(forward, up).Normalized();
	up = Vector3::Cross(side,forward);

	result[0] = side.x;
	result[1] = side.y;
	result[2] = side.z;
	result[3] = 0.0;

	result[4] = up.x;
	result[5] = up.y;
	result[6] = up.z;
	result[7] = 0.0;

	result[8]  = -forward.x;
	result[9]  = -forward.y;
	result[10] = -forward.z;
	result[11] = 0.0;

	result[12] = -Vector3::Dot(side,eye);	
	result[13] = -Vector3::Dot(up,eye);
	result[14] =  Vector3::Dot(forward,eye);
	result[15] = 1.0;

	GADGET_BASIC_ASSERT(result.IsValid());
	return result;
}

Matrix4 Matrix4::LookAt(const Vector3& eye_, const Vector3& at_, const Vector3& up_){
	GADGET_BASIC_ASSERT(eye_.IsValid());
	GADGET_BASIC_ASSERT(at_.IsValid());
	GADGET_BASIC_ASSERT(up_.IsValid());

	return LookAt(eye_.x, eye_.y, eye_.z, at_.x, at_.y, at_.z, up_.x, up_.y, up_.z);
}

Matrix4 Matrix4::Orthographic(float left_, float right_, float bottom_, float top_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(left_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(right_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(bottom_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(top_));

	Matrix4 ortho = Matrix4(0.0f);

	ortho[0] = 2.0f / (right_ - left_);
	ortho[5] = 2.0f / (top_ - bottom_);
	ortho[10] = -2.0f / (1.0f - (-1.0f));
	ortho[12] = -(right_ + left_) / (right_ - left_);
	ortho[13] = -(top_ + bottom_) / (top_ - bottom_);
	ortho[14] = -(1.0f + -1.0f) / (1.0f - (-1.0f));
	ortho[15] = 1.0f;

	GADGET_BASIC_ASSERT(ortho.IsValid());
	return ortho;
}

Matrix4 Matrix4::Orthographic(float left_, float right_, float bottom_, float top_, float near_, float far_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(left_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(right_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(bottom_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(top_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(near_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(far_));

	Matrix4 ortho = Matrix4(0.0f);

	ortho[0] = 2.0f / (right_ - left_);
	ortho[5] = 2.0f / (top_ - bottom_);
	ortho[10] = -2.0f / (far_ - near_);
	ortho[12] = -(right_ + left_) / (right_ - left_);
	ortho[13] = -(top_ + bottom_) / (top_ - bottom_);
	ortho[14] = -(far_ + near_) / (far_ - near_);
	ortho[15] = 1.0f;

	GADGET_BASIC_ASSERT(ortho.IsValid());
	return ortho;
}

//TODO - Validate this
Matrix4 Matrix4::Perspective(float fov_, float aspect_, float nearPlane_, float farPlane_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(fov_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(aspect_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(nearPlane_));
	GADGET_BASIC_ASSERT(Math::IsValidNumber(farPlane_));

	const float cot = 1.0f / Math::Tan(fov_ * 0.5f);
	//Don't forget, this looks row centric but it really is a column matrix - right-hand rule rules
	Matrix4 result(cot / aspect_, 0.0f, 0.0f, 0.0f,
		0.0f, cot, 0.0f, 0.0f,
		0.0f, 0.0f, (nearPlane_ + farPlane_) / (nearPlane_ - farPlane_), -1.0,
		0.0, 0.0, (2.0f * nearPlane_ * farPlane_) / (nearPlane_ - farPlane_), 0.0);
	GADGET_BASIC_ASSERT(result.IsValid());
	return result;
}

//The orthographic projection matrix is linear and affine but nothing else so there is has no inverse
//Therefore, it is labeled singular or non-invertable.
//I would still like to back transform from screen space to world space though
//Here's my unOrtho - It undoes what the orthographic matrix creates
//Multiply screen coordinates by this matrix and you should get x and y world coordinates
Matrix4 Matrix4::UnOrtho(const Matrix4& ortho){
	GADGET_BASIC_ASSERT(ortho.IsValid());

	Matrix4 m;
	m[0] = 1.0f / ortho[0];
	m[5] = 1.0f/ ortho[5];
	m[10] = 1.0f / ortho[10];
	m[12] = -ortho[12] * m[0];
	m[13] = -ortho[13] * m[5];
	m[14] = -ortho[14] * m[10];
	m[15] = 1.0f;
	GADGET_BASIC_ASSERT(m.IsValid());
	return m;
}

Matrix4 Matrix4::ViewportNDC(int w_, int h_){
	const float minZ = 0.0f;
	const float maxZ = 1.0f;
	
	const Matrix4 m1 = Scale(Vector3(1.0f, -1.0f, 1.0f));
	const Matrix4 m2 = Scale(Vector3(static_cast<float>(w_) / 2.0f, static_cast<float>(h_) / 2.0f, maxZ - minZ));
	const Matrix4 m3 = Translate(Vector3(static_cast<float>(w_) / 2.0f, static_cast<float>(h_) / 2.0f, minZ));
	return m3 * m2 * m1;
}

Matrix2 Matrix4::ToMatrix2() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix2(*this);
}

Matrix3 Matrix4::ToMatrix3() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix3(*this);
}

Matrix4x3 Matrix4::ToMatrix4x3() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix4x3(*this);
}

Euler Matrix4::ToEuler() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix3(*this).ToEuler();
}

Quaternion Matrix4::ToQuaternion() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix3(*this).ToQuaternion();
}

std::string Matrix4::ToString() const{
	return	std::to_string(m[0]) + ", " + std::to_string(m[4]) + ", " + std::to_string(m[8]) + ", " + std::to_string(m[12]) + ",\n" +
			std::to_string(m[1]) + ", " + std::to_string(m[5]) + ", " + std::to_string(m[9]) + ", " + std::to_string(m[13]) + ",\n" +
			std::to_string(m[2]) + ", " + std::to_string(m[6]) + ", " + std::to_string(m[10]) + ", " + std::to_string(m[14]) + ",\n" + 
			std::to_string(m[3]) + ", " + std::to_string(m[7]) + ", " + std::to_string(m[11]) + ", " + std::to_string(m[15]);
}