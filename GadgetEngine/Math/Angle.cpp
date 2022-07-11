#include "Angle.h"

#include <assert.h>

#include "Math.h"

using namespace Gadget;

//----------DEGREES----------//

constexpr Degree::Degree(float angle_) : angle(angle_){}

constexpr Degree::Degree(const Radian& r_) : angle(r_.ToDegreesValue()){}

constexpr Degree& Degree::operator =(float a_){
	*this = Degree(a_);
	return *this;
}

constexpr Degree& Degree::operator =(Radian& r_){
	*this = r_.ToDegrees();
	return *this;
}

constexpr Radian Degree::ToRadians() const{
	return Radian(ToRadiansValue());
}

constexpr float Degree::ToRadiansValue() const{
	return Math::DegreesToRadians(angle);
}

constexpr Degree Degree::operator -() const{ return Degree(-angle); }

constexpr Degree Degree::operator +(float a_) const{ return angle + a_; }
constexpr Degree Degree::operator +(const Radian& r_) const{ return *this + r_.ToDegrees(); }
constexpr Degree Degree::operator +(const Degree& d_) const{ return angle + d_.Get(); }

constexpr Degree Degree::operator -(float a_) const{ return angle - a_; }
constexpr Degree Degree::operator -(const Radian& r_) const{ return *this - r_.ToDegrees(); }
constexpr Degree Degree::operator -(const Degree& d_) const{ return angle - d_.Get(); }

constexpr Degree Degree::operator *(float a_) const{ return angle * a_; }
constexpr Degree Degree::operator *(const Radian& r_) const{ return *this * r_.ToDegrees(); }
constexpr Degree Degree::operator *(const Degree& d_) const{ return angle * d_.Get(); }

constexpr Degree Degree::operator /(float a_) const{
	assert(a_ != 0.0f);
	return angle / a_;
}

constexpr Degree Degree::operator /(const Radian& r_) const{ return *this / r_.ToDegrees(); }

constexpr Degree Degree::operator /(const Degree& d_) const{
	assert(d_.Get() != 0.0f);
	return angle / d_.Get();
}

constexpr void Degree::operator +=(float a_){ *this = *this + a_; }
constexpr void Degree::operator +=(const Radian& r_){ *this = *this + r_; }
constexpr void Degree::operator +=(const Degree& d_){ *this = *this + d_; }

constexpr void Degree::operator -=(float a_){ *this = *this - a_; }
constexpr void Degree::operator -=(const Radian& r_){ *this = *this - r_; }
constexpr void Degree::operator -=(const Degree& d_){ *this = *this - d_; }

constexpr void Degree::operator *=(float a_){ *this = *this * a_; }
constexpr void Degree::operator *=(const Radian& r_){ *this = *this * r_; }
constexpr void Degree::operator *=(const Degree& d_){ *this = *this * d_; }

constexpr void Degree::operator /=(float a_){ *this = *this / a_; }
constexpr void Degree::operator /=(const Radian& r_){ *this = *this / r_; }
constexpr void Degree::operator /=(const Degree& d_){ *this = *this / d_; }

constexpr bool Degree::operator ==(float a_) const{ return angle == a_; }
constexpr bool Degree::operator ==(const Radian& r_) const{ return *this == r_.ToDegrees(); }
constexpr bool Degree::operator ==(const Degree& d_) const{ return angle == d_.Get(); }

constexpr bool Degree::operator !=(float a_) const{ return angle != a_; }
constexpr bool Degree::operator !=(const Radian& r_) const{ return *this != r_.ToDegrees(); }
constexpr bool Degree::operator !=(const Degree& d_) const{ return angle != d_.Get(); }

constexpr bool Degree::operator >(float a_) const{ return angle > a_; }
constexpr bool Degree::operator >(const Radian& r_) const{ return *this > r_.ToDegrees(); }
constexpr bool Degree::operator >(const Degree& d_) const{ return angle > d_.Get(); }

constexpr bool Degree::operator <(float a_) const{ return angle < a_; }
constexpr bool Degree::operator <(const Radian& r_) const{ return *this < r_.ToDegrees(); }
constexpr bool Degree::operator <(const Degree& d_) const{ return angle < d_.Get(); }

constexpr bool Degree::operator >=(float a_) const{ return angle >= a_; }
constexpr bool Degree::operator >=(const Radian& r_) const{ return *this >= r_.ToDegrees(); }
constexpr bool Degree::operator >=(const Degree& d_) const{ return angle >= d_.Get(); }

constexpr bool Degree::operator <=(float a_) const{ return angle <= a_; }
constexpr bool Degree::operator <=(const Radian& r_) const{ return *this <= r_.ToDegrees(); }
constexpr bool Degree::operator <=(const Degree& d_) const{ return angle <= d_.Get(); }

constexpr Degree::operator float() const{ return Get(); }

//----------RADIANS----------//

constexpr Radian::Radian(float angle_) : angle(angle_){}

constexpr Radian::Radian(const Degree& d) : angle(d.ToRadiansValue()){}

constexpr Radian& Radian::operator =(float a_){
	*this = Radian(a_);
	return *this;
}

constexpr Radian& Radian::operator =(Degree& d_){
	*this = d_.ToRadians();
	return *this;
}

constexpr Degree Radian::ToDegrees() const{
	return Degree(ToDegreesValue());
};

constexpr float Radian::ToDegreesValue() const{
	return Math::RadiansToDegrees(angle);
}

constexpr Radian Radian::operator -() const{ return Radian(-angle); }

constexpr Radian Radian::operator +(float a_) const{ return angle + a_; }
constexpr Radian Radian::operator +(const Radian& r_) const{ return *this + r_.Get(); }
constexpr Radian Radian::operator +(const Degree& d_) const{ return angle + d_.ToRadians(); }

constexpr Radian Radian::operator -(float a_) const{ return angle - a_; }
constexpr Radian Radian::operator -(const Radian& r_) const{ return *this - r_.Get(); }
constexpr Radian Radian::operator -(const Degree& d_) const{ return angle - d_.ToRadians(); }

constexpr Radian Radian::operator *(float a_) const{ return angle * a_; }
constexpr Radian Radian::operator *(const Radian& r_) const{ return *this * r_.Get(); }
constexpr Radian Radian::operator *(const Degree& d_) const{ return angle * d_.ToRadians(); }

constexpr Radian Radian::operator /(float a_) const{
	assert(a_ != 0.0f);
	return angle / a_;
}

constexpr Radian Radian::operator /(const Radian& r_) const{
	assert(r_.Get() != 0.0f);
	return *this / r_.Get();
}

constexpr Radian Radian::operator /(const Degree& d_) const{ return angle / d_.ToRadians(); }

constexpr void Radian::operator +=(float a_){ *this = *this + a_; }
constexpr void Radian::operator +=(const Radian& r_){ *this = *this + r_; }
constexpr void Radian::operator +=(const Degree& d_){ *this = *this + d_; }

constexpr void Radian::operator -=(float a_){ *this = *this - a_; }
constexpr void Radian::operator -=(const Radian& r_){ *this = *this - r_; }
constexpr void Radian::operator -=(const Degree& d_){ *this = *this - d_; }

constexpr void Radian::operator *=(float a_){ *this = *this * a_; }
constexpr void Radian::operator *=(const Radian& r_){ *this = *this * r_; }
constexpr void Radian::operator *=(const Degree& d_){ *this = *this * d_; }

constexpr void Radian::operator /=(float a_){ *this = *this / a_; }
constexpr void Radian::operator /=(const Radian& r_){ *this = *this / r_; }
constexpr void Radian::operator /=(const Degree& d_){ *this = *this / d_; }

constexpr bool Radian::operator ==(float a_) const{ return angle == a_; }
constexpr bool Radian::operator ==(const Radian& r_) const{ return *this == r_.Get(); }
constexpr bool Radian::operator ==(const Degree& d_) const{ return angle == d_.ToRadians(); }

constexpr bool Radian::operator !=(float a_) const{ return angle != a_; }
constexpr bool Radian::operator !=(const Radian& r_) const{ return *this != r_.Get(); }
constexpr bool Radian::operator !=(const Degree& d_) const{ return angle != d_.ToRadians(); }

constexpr bool Radian::operator >(float a_) const{ return angle > a_; }
constexpr bool Radian::operator >(const Radian& r_) const{ return *this > r_.Get(); }
constexpr bool Radian::operator >(const Degree& d_) const{ return angle > d_.ToRadians(); }

constexpr bool Radian::operator <(float a_) const{ return angle < a_; }
constexpr bool Radian::operator <(const Radian& r_) const{ return *this < r_.Get(); }
constexpr bool Radian::operator <(const Degree& d_) const{ return angle < d_.ToRadians(); }

constexpr bool Radian::operator >=(float a_) const{ return angle >= a_; }
constexpr bool Radian::operator >=(const Radian& r_) const{ return *this >= r_.Get(); }
constexpr bool Radian::operator >=(const Degree& d_) const{ return angle >= d_.ToRadians(); }

constexpr bool Radian::operator <=(float a_) const{ return angle <= a_; }
constexpr bool Radian::operator <=(const Radian& r_) const{ return *this <= r_.Get(); }
constexpr bool Radian::operator <=(const Degree& d_) const{ return angle <= d_.ToRadians(); }

constexpr Radian::operator float() const{ return Get(); }