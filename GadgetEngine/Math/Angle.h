#ifndef GADGET_ANGLE_H
#define GADGET_ANGLE_H

namespace Gadget{
	//Forward declarations
	class Degree;
	class Radian;

	typedef Degree Angle; //Changing this will break everything but will allow you to use Radians by default if you desire

	class Degree{
	public:
		constexpr Degree(float angle_ = 0.0f);
		explicit constexpr Degree(const Radian& r_);
		constexpr Degree& operator =(float a_);
		constexpr Degree& operator =(Radian& r_);

		constexpr Radian ToRadians() const;
		constexpr float ToRadiansValue() const;

		constexpr float Get() const{ return angle; }

		constexpr Degree operator -() const;
		constexpr Degree operator +(float a_) const;
		constexpr Degree operator +(const Radian& r_) const;
		constexpr Degree operator +(const Degree& d_) const;
		constexpr Degree operator -(float a_) const;
		constexpr Degree operator -(const Radian& r_) const;
		constexpr Degree operator -(const Degree& d_) const;
		constexpr Degree operator *(float a_) const;
		constexpr Degree operator *(const Radian& r_) const;
		constexpr Degree operator *(const Degree& d_) const;
		constexpr Degree operator /(float a_) const;
		constexpr Degree operator /(const Radian& r_) const;
		constexpr Degree operator /(const Degree& d_) const;

		inline friend constexpr Degree operator +(float a_, const Degree& d_){ return d_ + a_; }
		inline friend constexpr Degree operator -(float a_, const Degree& d_){ return d_ - a_; }
		inline friend constexpr Degree operator *(float a_, const Degree& d_){ return d_ * a_; }
		inline friend constexpr Degree operator /(float a_, const Degree& d_){ return d_ / a_; }

		constexpr void operator +=(float a_);
		constexpr void operator +=(const Radian& r_);
		constexpr void operator +=(const Degree& d_);
		constexpr void operator -=(float a_);
		constexpr void operator -=(const Radian& r_);
		constexpr void operator -=(const Degree& d_);
		constexpr void operator *=(float a_);
		constexpr void operator *=(const Radian& r_);
		constexpr void operator *=(const Degree& d_);
		constexpr void operator /=(float a_);
		constexpr void operator /=(const Radian& r_);
		constexpr void operator /=(const Degree& d_);

		constexpr bool operator ==(float a_) const;
		constexpr bool operator ==(const Radian& r_) const;
		constexpr bool operator ==(const Degree& d_) const;
		constexpr bool operator !=(float a_) const;
		constexpr bool operator !=(const Radian& r_) const;
		constexpr bool operator !=(const Degree& d_) const;
		constexpr bool operator >(float a_) const;
		constexpr bool operator >(const Radian& r_) const;
		constexpr bool operator >(const Degree& d_) const;
		constexpr bool operator <(float a_) const;
		constexpr bool operator <(const Radian& r_) const;
		constexpr bool operator <(const Degree& d_) const;
		constexpr bool operator >=(float a_) const;
		constexpr bool operator >=(const Radian& r_) const;
		constexpr bool operator >=(const Degree& d_) const;
		constexpr bool operator <=(float a_) const;
		constexpr bool operator <=(const Radian& r_) const;
		constexpr bool operator <=(const Degree& d_) const;

		inline friend constexpr bool operator ==(float a_, const Degree& d_)	{ return d_ == a_; }
		inline friend constexpr bool operator !=(float a_, const Degree& d_)	{ return d_ != a_; }
		inline friend constexpr bool operator >(float a_, const Degree& d_)		{ return d_ > a_; }
		inline friend constexpr bool operator <(float a_, const Degree& d_)		{ return d_ < a_; }
		inline friend constexpr bool operator >=(float a_, const Degree& d_)	{ return d_ >= a_; }
		inline friend constexpr bool operator <=(float a_, const Degree& d_)	{ return d_ <= a_; }

		constexpr operator float() const;

	private:
		float angle;
	};

	class Radian{
	public:
		constexpr Radian(float angle_ = 0.0f);
		explicit constexpr Radian(const Degree& d);
		constexpr Radian& operator =(float a_);
		constexpr Radian& operator =(Degree& r_);

		constexpr Degree ToDegrees() const;
		constexpr float ToDegreesValue() const;

		constexpr float Get() const{ return angle; }

		constexpr Radian operator -() const;
		constexpr Radian operator +(float a_) const;
		constexpr Radian operator +(const Radian& r_) const;
		constexpr Radian operator +(const Degree& d_) const;
		constexpr Radian operator -(float a_) const;
		constexpr Radian operator -(const Radian& r_) const;
		constexpr Radian operator -(const Degree& d_) const;
		constexpr Radian operator *(float a_) const;
		constexpr Radian operator *(const Radian& r_) const;
		constexpr Radian operator *(const Degree& d_) const;
		constexpr Radian operator /(float a_) const;
		constexpr Radian operator /(const Radian& r_) const;
		constexpr Radian operator /(const Degree& d_) const;

		inline friend constexpr Radian operator +(float a_, const Radian& d_){ return d_ + a_; }
		inline friend constexpr Radian operator -(float a_, const Radian& d_){ return d_ - a_; }
		inline friend constexpr Radian operator *(float a_, const Radian& d_){ return d_ * a_; }
		inline friend constexpr Radian operator /(float a_, const Radian& d_){ return d_ / a_; }

		constexpr void operator +=(float a_);
		constexpr void operator +=(const Radian& r_);
		constexpr void operator +=(const Degree& d_);
		constexpr void operator -=(float a_);
		constexpr void operator -=(const Radian& r_);
		constexpr void operator -=(const Degree& d_);
		constexpr void operator *=(float a_);
		constexpr void operator *=(const Radian& r_);
		constexpr void operator *=(const Degree& d_);
		constexpr void operator /=(float a_);
		constexpr void operator /=(const Radian& r_);
		constexpr void operator /=(const Degree& d_);

		constexpr bool operator ==(float a_) const;
		constexpr bool operator ==(const Radian& r_) const;
		constexpr bool operator ==(const Degree& d_) const;
		constexpr bool operator !=(float a_) const;
		constexpr bool operator !=(const Radian& r_) const;
		constexpr bool operator !=(const Degree& d_) const;
		constexpr bool operator >(float a_) const;
		constexpr bool operator >(const Radian& r_) const;
		constexpr bool operator >(const Degree& d_) const;
		constexpr bool operator <(float a_) const;
		constexpr bool operator <(const Radian& r_) const;
		constexpr bool operator <(const Degree& d_) const;
		constexpr bool operator >=(float a_) const;
		constexpr bool operator >=(const Radian& r_) const;
		constexpr bool operator >=(const Degree& d_) const;
		constexpr bool operator <=(float a_) const;
		constexpr bool operator <=(const Radian& r_) const;
		constexpr bool operator <=(const Degree& d_) const;

		inline friend constexpr bool operator ==(float a_, const Radian& d_)	{ return d_ == a_; }
		inline friend constexpr bool operator !=(float a_, const Radian& d_)	{ return d_ != a_; }
		inline friend constexpr bool operator >(float a_, const Radian& d_)		{ return d_ > a_; }
		inline friend constexpr bool operator <(float a_, const Radian& d_)		{ return d_ < a_; }
		inline friend constexpr bool operator >=(float a_, const Radian& d_)	{ return d_ >= a_; }
		inline friend constexpr bool operator <=(float a_, const Radian& d_)	{ return d_ <= a_; }

		constexpr operator float() const;

	private:
		float angle;
	};
}

#endif //!GADGET_ANGLE_H