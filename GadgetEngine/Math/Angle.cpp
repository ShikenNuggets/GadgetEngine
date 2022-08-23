#include "Angle.h"

#include <assert.h>

#include "Math.h"

using namespace Gadget;

//----------DEGREES----------//

Degree::Degree(const Radian& r_) : angle(r_.ToDegreesValue()){}

Degree& Degree::operator =(Radian& r_){
	*this = r_.ToDegrees();
	return *this;
}

Radian Degree::ToRadians() const{
	return Radian(ToRadiansValue());
}

float Degree::ToRadiansValue() const{
	return Math::DegreesToRadians(angle);
}

Degree Degree::operator /(float a_) const{ return Math::SafeDivide(angle, a_); }

Degree Degree::operator /(const Degree& d_) const{ return angle / d_.Get(); }

void Degree::operator /=(float a_){ *this = *this / a_; }
void Degree::operator /=(const Degree& d_){ *this = *this / d_; }

//----------RADIANS----------//

Radian::Radian(const Degree& d) : angle(d.ToRadiansValue()){}

Radian& Radian::operator =(Degree& d_){
	*this = d_.ToRadians();
	return *this;
}

Degree Radian::ToDegrees() const{ return Degree(ToDegreesValue()); };

float Radian::ToDegreesValue() const{ return Math::RadiansToDegrees(angle); }

Radian Radian::operator /(float a_) const{ return Math::SafeDivide(angle, a_); }

Radian Radian::operator /(const Radian& r_) const{ return *this / r_.Get(); }

void Radian::operator /=(float a_){ *this = *this / a_; }
void Radian::operator /=(const Radian& r_){ *this = *this / r_; }