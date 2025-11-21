#include "Angle.h"

#include "Math/Math.h"

using namespace Gadget;

//----------DEGREES----------//

Degree::Degree(const Radian& r_) : angle(r_.ToDegreesValue()){}

Degree& Degree::operator =(Radian& r_){
	GADGET_BASIC_ASSERT(r_.IsValid());

	*this = r_.ToDegrees();
	return *this;
}

Radian Degree::ToRadians() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Radian(ToRadiansValue());
}

float Degree::ToRadiansValue() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Math::DegreesToRadians(angle);
}

Degree Degree::operator /(float a_) const{ return Math::SafeDivide(angle, a_); }

Degree Degree::operator /(const Degree& d_) const{ return angle / d_.Get(); }

void Degree::operator /=(float a_){ *this = *this / a_; }
void Degree::operator /=(const Degree& d_){ *this = *this / d_; }

bool Degree::IsValid() const{ return Math::IsValidNumber(angle); }

//----------RADIANS----------//

Radian::Radian(const Degree& d) : angle(d.ToRadiansValue()){}

Radian& Radian::operator =(Degree& d_){
	GADGET_BASIC_ASSERT(d_.IsValid());
	*this = d_.ToRadians();
	return *this;
}

Degree Radian::ToDegrees() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Degree(ToDegreesValue());
};

float Radian::ToDegreesValue() const{
	GADGET_BASIC_ASSERT(IsValid());
	return Math::RadiansToDegrees(angle);
}

Radian Radian::operator /(float a_) const{ return Math::SafeDivide(angle, a_); }

Radian Radian::operator /(const Radian& r_) const{ return *this / r_.Get(); }

void Radian::operator /=(float a_){ *this = *this / a_; }
void Radian::operator /=(const Radian& r_){ *this = *this / r_; }

bool Radian::IsValid() const{ return Math::IsValidNumber(angle); }