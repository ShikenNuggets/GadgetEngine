#include "Angle.h"

#include <assert.h>

#include "Math.h"

using namespace Gadget;

//----------DEGREES----------//

Degree::Degree(float angle_) : angle(angle_){}

Degree::Degree(const Radian& r){
	*this = r.ToDegrees();
}

Degree& Degree::operator =(float a_){
	*this = Degree(a_);
	return *this;
}

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

Degree Degree::operator -() const{ return Degree(-angle); }

Degree Degree::operator +(float a_) const{ return angle + a_; }
Degree Degree::operator +(const Radian& r_) const{ return *this + r_.ToDegrees(); }
Degree Degree::operator +(const Degree& d_) const{ return angle + d_.Get(); }

Degree Degree::operator -(float a_) const{ return angle - a_; }
Degree Degree::operator -(const Radian& r_) const{ return *this - r_.ToDegrees(); }
Degree Degree::operator -(const Degree& d_) const{ return angle - d_.Get(); }

Degree Degree::operator *(float a_) const{ return angle * a_; }
Degree Degree::operator *(const Radian& r_) const{ return *this * r_.ToDegrees(); }
Degree Degree::operator *(const Degree& d_) const{ return angle * d_.Get(); }

Degree Degree::operator /(float a_) const{
	assert(a_ != 0.0f);
	return angle / a_;
}

Degree Degree::operator /(const Radian& r_) const{ return *this / r_.ToDegrees(); }

Degree Degree::operator /(const Degree& d_) const{
	assert(d_.Get() != 0.0f);
	return angle / d_.Get();
}

void Degree::operator +=(float a_){ *this = *this + a_; }
void Degree::operator +=(const Radian& r_){ *this = *this + r_; }
void Degree::operator +=(const Degree& d_){ *this = *this + d_; }

void Degree::operator -=(float a_){ *this = *this - a_; }
void Degree::operator -=(const Radian& r_){ *this = *this - r_; }
void Degree::operator -=(const Degree& d_){ *this = *this - d_; }

void Degree::operator *=(float a_){ *this = *this * a_; }
void Degree::operator *=(const Radian& r_){ *this = *this * r_; }
void Degree::operator *=(const Degree& d_){ *this = *this * d_; }

void Degree::operator /=(float a_){ *this = *this / a_; }
void Degree::operator /=(const Radian& r_){ *this = *this / r_; }
void Degree::operator /=(const Degree& d_){ *this = *this / d_; }

bool Degree::operator ==(float a_) const{ return angle == a_; }
bool Degree::operator ==(const Radian& r_) const{ return *this == r_.ToDegrees(); }
bool Degree::operator ==(const Degree& d_) const{ return angle == d_.Get(); }

bool Degree::operator !=(float a_) const{ return angle != a_; }
bool Degree::operator !=(const Radian& r_) const{ return *this != r_.ToDegrees(); }
bool Degree::operator !=(const Degree& d_) const{ return angle != d_.Get(); }

bool Degree::operator >(float a_) const{ return angle > a_; }
bool Degree::operator >(const Radian& r_) const{ return *this > r_.ToDegrees(); }
bool Degree::operator >(const Degree& d_) const{ return angle > d_.Get(); }

bool Degree::operator <(float a_) const{ return angle < a_; }
bool Degree::operator <(const Radian& r_) const{ return *this < r_.ToDegrees(); }
bool Degree::operator <(const Degree& d_) const{ return angle < d_.Get(); }

bool Degree::operator >=(float a_) const{ return angle >= a_; }
bool Degree::operator >=(const Radian& r_) const{ return *this >= r_.ToDegrees(); }
bool Degree::operator >=(const Degree& d_) const{ return angle >= d_.Get(); }

bool Degree::operator <=(float a_) const{ return angle <= a_; }
bool Degree::operator <=(const Radian& r_) const{ return *this <= r_.ToDegrees(); }
bool Degree::operator <=(const Degree& d_) const{ return angle <= d_.Get(); }

constexpr Degree::operator float() const{ return Get(); }

//----------RADIANS----------//

Radian::Radian(float angle_) : angle(angle_){}

Radian::Radian(const Degree& d){
	*this = d.ToRadians();
}

Radian& Radian::operator =(float a_){
	*this = Radian(a_);
	return *this;
}

Radian& Radian::operator =(Degree& d_){
	*this = d_.ToRadians();
	return *this;
}

Degree Radian::ToDegrees() const{
	return Degree(ToDegreesValue());
};

float Radian::ToDegreesValue() const{
	return Math::RadiansToDegrees(angle);
}

Radian Radian::operator -() const{ return Radian(-angle); }

Radian Radian::operator +(float a_) const{ return angle + a_; }
Radian Radian::operator +(const Radian& r_) const{ return *this + r_.Get(); }
Radian Radian::operator +(const Degree& d_) const{ return angle + d_.ToRadians(); }

Radian Radian::operator -(float a_) const{ return angle - a_; }
Radian Radian::operator -(const Radian& r_) const{ return *this - r_.Get(); }
Radian Radian::operator -(const Degree& d_) const{ return angle - d_.ToRadians(); }

Radian Radian::operator *(float a_) const{ return angle * a_; }
Radian Radian::operator *(const Radian& r_) const{ return *this * r_.Get(); }
Radian Radian::operator *(const Degree& d_) const{ return angle * d_.ToRadians(); }

Radian Radian::operator /(float a_) const{
	assert(a_ != 0.0f);
	return angle / a_;
}

Radian Radian::operator /(const Radian& r_) const{
	assert(r_.Get() != 0.0f);
	return *this / r_.Get();
}

Radian Radian::operator /(const Degree& d_) const{ return angle / d_.ToRadians(); }

void Radian::operator +=(float a_){ *this = *this + a_; }
void Radian::operator +=(const Radian& r_){ *this = *this + r_; }
void Radian::operator +=(const Degree& d_){ *this = *this + d_; }

void Radian::operator -=(float a_){ *this = *this - a_; }
void Radian::operator -=(const Radian& r_){ *this = *this - r_; }
void Radian::operator -=(const Degree& d_){ *this = *this - d_; }

void Radian::operator *=(float a_){ *this = *this * a_; }
void Radian::operator *=(const Radian& r_){ *this = *this * r_; }
void Radian::operator *=(const Degree& d_){ *this = *this * d_; }

void Radian::operator /=(float a_){ *this = *this / a_; }
void Radian::operator /=(const Radian& r_){ *this = *this / r_; }
void Radian::operator /=(const Degree& d_){ *this = *this / d_; }

bool Radian::operator ==(float a_) const{ return angle == a_; }
bool Radian::operator ==(const Radian& r_) const{ return *this == r_.Get(); }
bool Radian::operator ==(const Degree& d_) const{ return angle == d_.ToRadians(); }

bool Radian::operator !=(float a_) const{ return angle != a_; }
bool Radian::operator !=(const Radian& r_) const{ return *this != r_.Get(); }
bool Radian::operator !=(const Degree& d_) const{ return angle != d_.ToRadians(); }

bool Radian::operator >(float a_) const{ return angle > a_; }
bool Radian::operator >(const Radian& r_) const{ return *this > r_.Get(); }
bool Radian::operator >(const Degree& d_) const{ return angle > d_.ToRadians(); }

bool Radian::operator <(float a_) const{ return angle < a_; }
bool Radian::operator <(const Radian& r_) const{ return *this < r_.Get(); }
bool Radian::operator <(const Degree& d_) const{ return angle < d_.ToRadians(); }

bool Radian::operator >=(float a_) const{ return angle >= a_; }
bool Radian::operator >=(const Radian& r_) const{ return *this >= r_.Get(); }
bool Radian::operator >=(const Degree& d_) const{ return angle >= d_.ToRadians(); }

bool Radian::operator <=(float a_) const{ return angle <= a_; }
bool Radian::operator <=(const Radian& r_) const{ return *this <= r_.Get(); }
bool Radian::operator <=(const Degree& d_) const{ return angle <= d_.ToRadians(); }

constexpr Radian::operator float() const{ return Get(); }