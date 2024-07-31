#include "Euler.h"

#include "Debug.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

using namespace Gadget;

Euler::Euler(const Vector3& v_) : x(v_.x), y(v_.y), z(v_.z){}

Matrix3 Euler::ToMatrix3() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Matrix3(ToMatrix4());
}

Matrix4 Euler::ToMatrix4() const{
	GADGET_BASIC_ASSERT(IsValid());

	return	Matrix4::Rotate(y, Vector3(0.0f, 1.0f, 0.0f)) *
			Matrix4::Rotate(z, Vector3(0.0f, 0.0f, 1.0f)) * 
			Matrix4::Rotate(x, Vector3(1.0f, 0.0f, 0.0f));
}

Quaternion Euler::ToQuaternion() const{
	GADGET_BASIC_ASSERT(IsValid());

	const float bank = x.ToRadians();
	const float heading = y.ToRadians();
	const float attitude = z.ToRadians();

	const float c1 = Math::CosR(heading / 2.0f);
	const float c2 = Math::CosR(attitude / 2.0f);
	const float c3 = Math::CosR(bank / 2.0f);

	const float s1 = Math::SinR(heading / 2.0f);
	const float s2 = Math::SinR(attitude / 2.0f);
	const float s3 = Math::SinR(bank / 2.0f);

	const float newW = c1 * c2 * c3 - s1 * s2 * s3;
	const float newX = s1 * s2 * c3 + c1 * c2 * s3;
	const float newY = s1 * c2 * c3 + c1 * s2 * s3;
	const float newZ = c1 * s2 * c3 - s1 * c2 * s3;

	return Quaternion(newW, newX, newY, newZ).Normalized();
}

std::string Euler::ToString() const{
	return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}